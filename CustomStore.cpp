#include "CustomStore.h"
#include "Store_Extend.h"
#include <boost/lexical_cast.hpp>

CustomStore* CustomStore::instance = new CustomStore();

void CustomStore::ParseStoreNode(rapidxml::xml_node<char>* node)
{
    for (auto storeNode = node->first_node(); storeNode; storeNode = storeNode->next_sibling())
    {
        std::string name = storeNode->name();
        std::string val = storeNode->value();
        if (name == "freeDrones")
        {
            for (auto droneNode = storeNode->first_node(); droneNode; droneNode = droneNode->next_sibling())
            {
                freeDrones.push_back(droneNode->name());
            }
        }
        if (name == "customStore")
        {
            std::string id = storeNode->first_attribute("id")->value();
            StoreDefinition def = StoreDefinition();

            for (auto cStoreNode = storeNode->first_node(); cStoreNode; cStoreNode = cStoreNode->next_sibling())
            {
                // do xml parsing for custom stores here

                if (name == "addCategory")
                {
                    for (auto categoryNode = cStoreNode->first_node(); categoryNode; categoryNode = categoryNode->next_sibling())
                    {
                        std::string categoryName = categoryNode->name();
                        if (categoryName == "WEAPONS")
                        {
                            StoreCategory storeCategory = StoreCategory();
                            storeCategory.categoryType = CategoryType::WEAPONS;
                            for (auto weaponNode = categoryNode->first_node(); weaponNode; weaponNode = weaponNode->next_sibling())
                            {
                                std::string weaponNodeName = weaponNode->name();
                                std::string weaponVal = weaponNode->value();
                                for (auto blueprintNode = weaponNode->first_node(); blueprintNode; blueprintNode = blueprintNode->next_sibling())
                                {
                                    StoreItem item = StoreItem();
                                    std::string blueprintNodeName = blueprintNode->name();
                                    std::string blueprintNodeVal = blueprintNode->value();
                                    if (blueprintNodeName == "bundledItems")
                                    {

                                    }
                                    else if (blueprintNodeName == "bundledResources")
                                    {

                                    }
                                    else if (blueprintNodeName == "stock")
                                    {
                                        item.stock == boost::lexical_cast<int>(blueprintNode->value());
                                    }
                                    else if (blueprintNodeName == "currency")
                                    {
                                        if (blueprintNodeVal == "CREW")
                                        {
                                            item.individualCurrency = Currency::CREW;
                                        }
                                    }
                                }
                            }


                            def.categories.push_back(storeCategory);
                        }
                        else if (categoryName == "DRONES")
                        {

                        }
                        else if (categoryName == "AUGMENTS")
                        {

                        }
                        else if (categoryName == "CREW")
                        {

                        }
                        else if (categoryName == "SYSTEMS")
                        {

                        }
                    }
                }
            }

            storeDefs[id] = def;


        }
    }
}

HOOK_METHOD(SystemStoreBox, constructor, (ShipManager *shopper, Equipment *equip, int sys) -> void)
{
    super(shopper, equip, sys);

    if (itemId != 4)
    {
        return;
    }

    if (CustomStore::instance->freeDrones.size() <= 0)
        return;

    auto bp = G_->GetBlueprints()->GetDroneBlueprint(freeBlueprint);

    int oldPrice = bp->desc.cost / 2;

    DroneBlueprint *newBlueprint = G_->GetBlueprints()->GetDroneBlueprint(CustomStore::instance->freeDrones[random32() % CustomStore::instance->freeDrones.size()]);
    freeBlueprint = newBlueprint->name;

    desc.cost -= oldPrice;
    desc.cost += newBlueprint->desc.cost / 2;
}

void StoreComplete::OnInit(const StoreDefinition& def, ShipManager *ship, Equipment *equip, int level)
{
    orig->worldLevel = level;
    orig->shopper = ship;

    for (auto i : def.resources)
    {
        ItemStoreBox* box = new ItemStoreBox(ship, i.type);

        box->count = i.minCount + (random32() % (i.maxCount - i.minCount + 1));
        if (i.modifiedPrice)
        {
            if (i.flatModifier)
            {
                box->desc.cost += i.modifier;
            }
            else
            {
                box->desc.cost *= i.modifier;
            }
        }
        if (i.price != -1)
        {
            box->desc.cost = i.price;
        }

        resourceBoxes.push_back(box);
    }

    SetPositions();
}

void StoreComplete::SetPositions()
{
    orig->box = G_->GetResources()->GetImageId("storeUI/store_buy_main.png");

    orig->infoBoxLoc.x = orig->position.x + 600;
    orig->infoBoxLoc.y = orig->position.y;
    orig->infoBox.location = orig->infoBoxLoc;




    int resourceBoxOffsets[3] = {0, 50, 99};

    for (int i = 0; i < resourceBoxes.size(); i++)
    {
        auto box = resourceBoxes[i];

        box->SetPosition(orig->position.x + 18, orig->position.y + 83 + resourceBoxOffsets[i]);
    }
}

void StoreComplete::OnRender()
{
    CSurface::GL_PushMatrix();
    if (orig->confirmBuy)
    {
        CSurface::GL_SetColorTint(COLOR_TINT);
    }

    CSurface::GL_Translate(orig->position.x, orig->position.y);

    if (orig->box)
    {
        CSurface::GL_BlitPixelImage(orig->box, 0, 0, orig->box->width_, orig->box->height_, 0, COLOR_WHITE, false);
    }

    CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

    orig->DrawBuySellTabText();
    freetype::easy_print(62, 18.f, 45.f, G_->GetTextLibrary()->GetText("store_title_items"));
    freetype::easy_print(62, 18.f, 254.f, G_->GetTextLibrary()->GetText("store_title_repair"));

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    std::string currentHullText = G_->GetTextLibrary()->GetText("store_current_hull");

    float currentHullY = 419.f;

    if (currentHullText.find('\n') == std::string::npos)
    {
        currentHullY = 427.f;
    }

    freetype::easy_printCenter(59, 73.f, currentHullY, currentHullText);

    CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

    CSurface::GL_PopMatrix();

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    std::string unavailableText = G_->GetTextLibrary()->GetText("store_unavailable");

    if (pages.size() > 0)
    {
        for (auto section : pages[currentPage].sections)
        {
            for (auto box : section.storeBoxes[section.currentSection])
            {
                box->OnRender();
            }
        }

        if (pages[currentPage].sections.size() == 1)
        {
            CSurface::GL_SetColor(COLOR_BUTTON_ON);

            freetype::easy_printCenter(52, orig->position.x + 394, orig->position.y + 336, unavailableText);
        }
    }
    else
    {
        CSurface::GL_SetColor(COLOR_BUTTON_ON);

        freetype::easy_printCenter(52, orig->position.x + 394, orig->position.y + 152, unavailableText);
        freetype::easy_printCenter(52, orig->position.x + 394, orig->position.y + 362, unavailableText);
    }

    for (auto i : resourceBoxes)
    {
        i->OnRender();
    }

    if (resourceBoxes.empty())
    {
        CSurface::GL_SetColor(COLOR_BUTTON_ON);

        freetype::easy_printCenter(52, orig->position.x + 102, orig->position.y + 152, unavailableText);
    }

    for (auto i : repairBoxes)
    {
        i->OnRender();
    }

    if (repairBoxes.empty())
    {
        CSurface::GL_SetColor(COLOR_BUTTON_ON);

        freetype::easy_printCenter(52, orig->position.x + 102, orig->position.y + 336, unavailableText);
    }

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    char buffer[64];
    sprintf(buffer, "%d", orig->shopper->ship.hullIntegrity.first);
    freetype::easy_print(0, orig->position.x + 143, orig->position.y + 432, buffer);
    orig->infoBox.OnRender();
    if (orig->confirmBuy)
    {
        CSurface::GL_RemoveColorTint();
        orig->confirmDialog.OnRender();
    }
}

void StoreComplete::NextPage()
{
    currentPage++;

    if (currentPage >= pages.size())
    {
        currentPage = 0;
    }
}

void StoreComplete::PreviousPage()
{
    currentPage--;
    if (currentPage < 0)
    {
        currentPage = pages.size() - 1;
    }
}

void StoreComplete::KeyDown(SDLKey key)
{
    if (key == SDLKey::SDLK_RIGHT)
    {
        NextPage();
    }
    if (key == SDLKey::SDLK_LEFT)
    {
        PreviousPage();
    }
}

void StoreComplete::OnLoop()
{
    for (auto i : resourceBoxes)
    {
        i->OnLoop();
    }
}

void StoreComplete::MouseClick(int x, int y)
{
    for (auto i : resourceBoxes)
    {
        i->MouseClick(x, y);
    }
}

void StoreComplete::MouseMove(int x, int y)
{
    orig->infoBox.Clear();

    for (auto i : resourceBoxes)
    {
        i->MouseMove(x, y);

        if ((i->button.bHover || i->button.bSelected) && i->count > 0)
        {
            i->SetInfoBox(orig->infoBox, 347);
        }
    }
}

HOOK_METHOD(Store, KeyDown, (SDLKey key) -> void)
{
    STORE_EX(this)->customStore->KeyDown(key);

    return;
    super(key);
}

HOOK_METHOD(Store, OnRender, () -> void)
{
    //return super();

    if (STORE_EX(this)->isCustomStore)
    {
        STORE_EX(this)->customStore->OnRender();
        return;
    }

    CSurface::GL_PushMatrix();
    if (confirmBuy)
    {
        CSurface::GL_SetColorTint(COLOR_TINT);
    }

    CSurface::GL_Translate(position.x, position.y);


    CSurface::GL_BlitPixelImage(box, 0, 0, box->width_, box->height_, 0, COLOR_WHITE, false);
    CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

    DrawBuySellTabText();
    freetype::easy_print(62, 18.f, 45.f, G_->GetTextLibrary()->GetText("store_title_items"));
    freetype::easy_print(62, 18.f, 254.f, G_->GetTextLibrary()->GetText("store_title_repair"));

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    std::string currentHullText = G_->GetTextLibrary()->GetText("store_current_hull");

    float currentHullY = 419.f;

    if (currentHullText.find('\n') == std::string::npos)
    {
        currentHullY = 427.f;
    }

    freetype::easy_printCenter(59, 73.f, currentHullY, currentHullText);

    CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

    int headingIndex = bShowPage2 ? 2 : 0;

    Point pos1 = Point(198, 42);
    freetype::easy_print(62, pos1.x + 7, pos1.y + 3, headingTitle[headingIndex].GetText());

    int heading2Index = bShowPage2 ? 3 : 1;

    Point pos2 = Point(198, 251);
    freetype::easy_print(62, pos2.x + 7, pos2.y + 3, headingTitle[heading2Index].GetText());

    CSurface::GL_PopMatrix();

    if (sectionCount > 2)
    {
        Pointf pos3 = Pointf(currentButton->position.x, currentButton->position.y + 7);

        currentButton->OnRender();
        CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

        freetype::easy_printCenter(62, pos3.x + 65, pos3.y, G_->GetTextLibrary()->GetText("store_tab_page1"));
        freetype::easy_printCenter(62, pos3.x + 171, pos3.y, G_->GetTextLibrary()->GetText("store_tab_page2"));
    }

    if (bShowPage2)
    {
        for (int i = 6; i < sectionCount * 3; i++)
        {
            vStoreBoxes[i]->OnRender();
        }
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            vStoreBoxes[i]->OnRender();
        }
    }

    // appears to be for items/repair, but it's weird??
    if (3 * sectionCount + 5 > 3 * sectionCount)
    {
        for (int i = 3 * sectionCount; i < 3 * sectionCount + 5; i++)
        {
            vStoreBoxes[i]->OnRender();
        }
    }

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    char buffer[64];
    sprintf(buffer, "%d", shopper->ship.hullIntegrity.first);
    freetype::easy_print(0, position.x + 143, position.y + 432, buffer);
    infoBox.OnRender();
    if (confirmBuy)
    {
        CSurface::GL_RemoveColorTint();
        confirmDialog.OnRender();
    }
}

static bool inCreateStore = false;
std::string storeEventName;

HOOK_METHOD(WorldManager, CreateStore, (LocationEvent *event) -> void)
{
    inCreateStore = true;
    storeEventName = event->eventName;
    super(event);
    inCreateStore = false;
}

HOOK_METHOD(Store, OnInit, (ShipManager *shopper, Equipment *equip, int worldLevel) -> void)
{
    // if (custom store)
    {
        StoreComplete* newStore = new StoreComplete(this);

        StoreDefinition newDef = StoreDefinition();
        ResourceItem item = ResourceItem();
        item.minCount = 3;
        item.maxCount = 10;
        item.price = 2;
        item.type = "missiles";


        newStore->OnInit(newDef, shopper, equip, worldLevel);

        STORE_EX(this)->isCustomStore = true;
        STORE_EX(this)->customStore = newStore;

        return;
    }

    super(shopper, equip, worldLevel);
}

HOOK_METHOD(Store, RelinkShip, (ShipManager *ship, Equipment *equip) -> void)
{
    if (STORE_EX(this)->isCustomStore)
    {
        // do stuff
        return;
    }

    super(ship, equip);
}

HOOK_METHOD(Store, LoadStore, (int file, int worldLevel) -> void)
{
    if (STORE_EX(this)->isCustomStore)
    {
        // load custom store
        return;
    }

    super(file, worldLevel);
}

HOOK_METHOD(Store, OnLoop, () -> void)
{
    if (STORE_EX(this)->isCustomStore)
    {
        STORE_EX(this)->customStore->OnLoop();
        return;
    }

    super();
}

HOOK_METHOD(Store, SetPositions, () -> void)
{
    if (STORE_EX(this)->isCustomStore)
    {
        STORE_EX(this)->customStore->SetPositions();
        return;
    }

    super();
}

HOOK_METHOD(Store, MouseMove, (int x, int y) -> void)
{
    if (STORE_EX(this)->isCustomStore)
    {
        STORE_EX(this)->customStore->MouseMove(x, y);
        return;
    }

    super(x, y);
}

HOOK_METHOD(Store, MouseClick, (int x, int y) -> void)
{
    if (STORE_EX(this)->isCustomStore)
    {
        STORE_EX(this)->customStore->MouseClick(x, y);
        return;
    }

    super(x, y);
}
