#include "CustomShips.h"
#include "freetype.h"
#include <algorithm>

CustomShipSelect CustomShipSelect::instance = CustomShipSelect();



void CustomShipSelect::AddShip(std::string& name, bool typeB, bool typeC)
{
    ShipDefinition def = ShipDefinition();
    def.name = name;
    def.typeB = typeB;
    def.typeC = typeC;

    blueprintNames.push_back( def );
}


void CustomShipSelect::ParseShipsNode(rapidxml::xml_node<char> *node)
{
    // parse <ships> node in hyperspace.xml
    for (auto child = node->first_node(); child; child = child->next_sibling())
    {
        if (child->first_attribute("name"))
        {
            std::string name = std::string(child->first_attribute("name")->value());

            bool typeB = false;
            bool typeC = false;

            if (child->first_attribute("b"))
            {
                typeB = EventsParser::ParseBoolean(child->first_attribute("b")->value());
            }
            if (child->first_attribute("c"))
            {
                typeC = EventsParser::ParseBoolean(child->first_attribute("c")->value());
            }

            AddShip(name, typeB, typeC);
        }
    }
}

void CustomShipSelect::OnInit(ShipSelect* shipSelect_)
{

    maxShipPage = std::ceil(blueprintNames.size() / 10.f);

    if (maxShipPage <= 0)
        maxShipPage = 0;
    else if (!initialized)
    {
        int i = 0;
        for (auto const &x: blueprintNames)
        {
            // create and initialize ShipButtons for each of the blueprints
            int curPage = i / 10;

            ShipButton* aButton = new ShipButton(100 + i, 0);
            ShipButton* bButton;
            ShipButton* cButton;

            int onPage = i % 10;


            bButton = new ShipButton(100 + i, 1);

            if (!x.typeB)
            {
                bButton->bNoExist = true;
                bButton->bActive = false;
            }

            cButton = new ShipButton(100 + i, 2);

            if (!x.typeC)
            {
                cButton->bNoExist = true;
                cButton->bActive = false;
            }

            //aButton->bShipLocked = true;
            //aButton->bLayoutLocked = true;


            ShipButtonList* buttonList = new ShipButtonList(curPage, 100 + i, aButton, bButton, cButton);
            shipButtons.push_back(buttonList);


            Point pos = Point(0, 0);
            if (onPage < 4)
            {
                pos = Point(136 + 205 * onPage, 161);
            }
            else if (onPage < 8)
            {
                pos = Point(136 + 205 * (onPage - 4), 161 + 177);
            }
            else if (onPage == 8)
            {
                pos = Point(136 + 205 * 3 + 225, 161);
            }
            else if (onPage == 9)
            {
                pos = Point(136 + 205 * 3 + 225, 161 + 177);
            }

            //if (!x.hasAchievements)

            int oldY = pos.y;

            if (!aButton->bShipLocked)
                pos.y = oldY + 20;

            aButton->OnInit("customizeUI/ship_list_button", pos);

            if (!(bButton->bNoExist || bButton->bShipLocked))
                pos.y = oldY + 20;
            else
                pos.y = oldY;

            bButton->OnInit("customizeUI/ship_list_button", pos);

            if (!(cButton->bNoExist || bButton->bShipLocked))
                pos.y = oldY + 20;
            else
                pos.y = oldY;

            cButton->OnInit("customizeUI/ship_list_button", pos);


            i++;
        }
    }




    std::string buttonImg("customizeUI/button_ship_arrow");
    leftButton = new Button();
    leftButton->OnInit(buttonImg, 1100, 119);

    rightButton = new Button();
    rightButton->OnInit(buttonImg, 1135, 119);

    rightButton->bMirror = true;

    if (maxShipPage == 0)
    {
        leftButton->bActive = false;
        rightButton->bActive = false;
    }


    shipSelect = shipSelect_;
    oldShipButtons = shipSelect->shipButtons;

    initialized = true;
}

void CustomShipSelect::OnRender(bool renderSelect)
{
    if (shipSelect->tutorial.bOpen)
    {
        GL_Color tint(0.5f, 0.5f, 0.5f, 1.f);
        CSurface::GL_SetColorTint(tint);
    }

    if (renderSelect)
    {
        for (auto const &x: shipButtons)
        {
            if (x->GetPage() == shipPage - 1)
            {
                ShipButton* button = x->GetButton(shipSelect->currentType);

                if (button)
                    button->OnRender();
            }
        }
    }
    else
    {
        leftButton->OnRender();
        rightButton->OnRender();

        char buf[128];



        sprintf(buf, "%d/%d", GetCurrentPage() + 1, GetMaxPages() + 1 );
        std::string text(buf);

        CSurface::GL_SetColor(COLOR_BUTTON_TEXT);
        CSurface::GL_RemoveColorTint();
        freetype::easy_printRightAlign(63, 1097, 118, text);
    }



    shipSelect->infoBox.OnRender();

    if (shipSelect->tutorial.bOpen)
    {
        CSurface::GL_RemoveColorTint();
    }
}

void CustomShipSelect::MouseClick()
{
    if (leftButton->bActive && leftButton->bHover)
    {
        if (shipPage == 0)
        {
            SwitchPage(maxShipPage);
        }
        else
        {
            SwitchPage(shipPage - 1);
        }
    }

    if (rightButton->bActive && rightButton->bHover)
    {
        if (shipPage == maxShipPage)
        {
            SwitchPage(0);
        }
        else
        {
            SwitchPage(shipPage + 1);
        }
    }
}

void CustomShipSelect::SwitchPage(int page)
{
    if (shipPage == page)
        return;


    if (shipPage == 0)
    {
        LeaveFirstPage();
    }

    if (page == 0)
    {
        EnterFirstPage();
    }

    shipPage = page;
}

void CustomShipSelect::LeaveFirstPage()
{
    for (auto const &x: oldShipButtons)
    {
        x->bActive = false;
    }

}

void CustomShipSelect::EnterFirstPage()
{
    for (auto const &x: oldShipButtons)
    {
        if (!x->bNoExist)
            x->bActive = true;
    }
}

bool CustomShipSelect::ShouldRenderButton(ShipButton *button)
{
    if (shipPage != 0 &&
            std::find(oldShipButtons.begin(), oldShipButtons.end(), button) != oldShipButtons.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CustomShipSelect::ShouldRenderArrow()
{
    return shipPage == 0;
}

bool CustomShipSelect::ShouldRenderButtonLower()
{
    return shipPage == 0;
}

void CustomShipSelect::MouseMove(int x, int y)
{
    if (shipSelect->tutorial.bOpen)
        return;
    leftButton->MouseMove(x, y, false);
    rightButton->MouseMove(x, y, false);

    selectedShip = -1;

    for (auto const &i: shipButtons)
    {
        if (i->GetPage() == shipPage - 1)
        {
            ShipButton* button = i->GetButton(shipSelect->currentType);

            if (button)
            {
                button->MouseMove(x, y);
                if (button->bHover && button->bActive)
                {
                    selectedShip = i->GetIndex();
                    lastSelectedShip = selectedShip;
                }
            }
        }
    }

    if (!FirstPage())
    {
        if (selectedShip == -1)
            shipSelect->infoBox.Clear();
        else
        {
            std::string name( blueprintNames[selectedShip].name );

            if (shipSelect->currentType == 1)
            {
                name.append("_2");
            }
            else if (shipSelect->currentType == 2)
            {
                name.append("_3");
            }

            ShipBlueprint* bp = G_->GetBlueprints()->GetShipBlueprint(name, -1);

            shipSelect->infoBox.SetDescription(&bp->desc, 360, 135, InfoBox::ExpandDir::EXPAND_DOWN);
        }
    }


    shipSelect->typeA.MouseMove(x, y, false);
    shipSelect->typeB.MouseMove(x, y, false);
    shipSelect->typeC.MouseMove(x, y, false);
}

std::string& CustomShipSelect::GetShipBlueprint(int shipId)
{
    int customIndex = shipId - 100;

    return blueprintNames[customIndex].name;
}


void CustomShipSelect::SwitchShip(ShipBuilder *builder, int type, int variant, bool force)
{
    if (type == builder->currentShipId && variant == builder->currentType && !(force))
        return;
    builder->currentType = variant;

    ShipManager *ship = new ShipManager(0);
    SM_EX(ship)->isCustomShip = true;
    builder->currentShip = ship;
    builder->currentShipId = type;
    ShipDefinition def = blueprintNames[type - 100];

    std::string name( def.name );

    if (variant == 1)
    {
        name.append("_2");
    }
    else if (variant == 2)
    {
        name.append("_3");
    }


    ShipBlueprint* bp = G_->GetBlueprints()->GetShipBlueprint(name, -1);


    if (bp)
    {
        ship->OnInit(bp, 0);
        std::string shipRealName = std::string();

        if (ship->myBlueprint.name.isLiteral)
        {
            shipRealName.assign(ship->myBlueprint.name.data);
        }
        else
        {
            TextLibrary::GetText(shipRealName, G_->GetTextLibrary(), ship->myBlueprint.name.data, G_->GetTextLibrary()->currentLanguage);
        }

        builder->currentName.swap(shipRealName);
        builder->nameInput.SetText(builder->currentName);

        builder->CreateEquipmentBoxes();
        builder->CreateSystemBoxes();
        builder->SetupShipAchievements();

        builder->ClearShipAchievements();

        builder->typeA.SetActive(variant != 0);
        builder->typeA.bRenderSelected = variant == 0;

        if (!def.typeB)
        {
            builder->typeB.bActive = false;
            builder->typeB.bRenderOff = true;
            builder->typeB.bRenderSelected = false;
        }
        else
        {
            builder->typeB.SetActive(variant != 1);

            builder->typeB.bRenderOff = false;
            builder->typeB.bRenderSelected = variant == 1;
        }

        if (!def.typeC)
        {
            builder->typeC.bActive = false;
            builder->typeC.bRenderOff = true;
            builder->typeC.bRenderSelected = false;
        }
        else
        {
            builder->typeC.SetActive(variant != 2);
            builder->typeC.bRenderOff = false;
            builder->typeC.bRenderSelected = variant == 2;
        }

        Point typeAPos(builder->typeALoc.x + builder->typeAOffset, builder->typeALoc.y);
        builder->typeA.SetLocation(typeAPos);

        Point typeBPos(builder->typeBLoc.x + builder->typeBOffset, builder->typeBLoc.y);
        builder->typeB.SetLocation(typeBPos);

        Point typeCPos(builder->typeCLoc.x + builder->typeCOffset, builder->typeCLoc.y);
        builder->typeC.SetLocation(typeCPos);


        shipSelect->Close();
    }

}

void CustomShipSelect::Open()
{
    open = true;
}

void CustomShipSelect::Close()
{
    open = false;
}

bool CustomShipSelect::CycleShipNext(ShipBuilder *builder)
{
    if (builder->currentShipId < 100)
        return true;

    int index = builder->currentShipId - 100;

    int num = blueprintNames.size();

    if (builder->currentType == 1)
    {
        if (index + 1 == num)
        {
            index = 0;
        }
        else
        {
            index++;
        }

        int counter = 0;

        while (index >= num || !blueprintNames[index].typeB)
        {
            if (index == num)
            {
                index = 0;
            }
            else
            {
                index++;
            }

            counter++;
            if (counter > 100)
            {
                printf("Infinite loop while getting next ship!");
                break;
            }
        }
    }
    else if (builder->currentType == 2)
    {
        if (index + 1 == num)
        {
            index = 0;
        }
        else
        {
            index++;
        }

        int counter = 0;

        while (index >= num || !blueprintNames[index].typeC)
        {
            if (index == num)
            {
                index = 0;
            }
            else
            {
                index++;
            }

            counter++;
            if (counter > 100)
            {
                printf("Infinite loop while getting next ship!");
                break;
            }
        }
    }
    else
    {
        if (index + 1 == num)
        {
            index = 0;
        }
        else
        {
            index++;
        }
    }

    if (index > num)
    {
        index = 0;
    }

    builder->SwitchShip(index + 100, builder->currentType);


    return false;
}

bool CustomShipSelect::CycleShipPrevious(ShipBuilder *builder)
{
    if (builder->currentShipId < 100)
        return true;


    int index = builder->currentShipId - 100;

    int num = blueprintNames.size() - 1;

    if (builder->currentType == 1)
    {
        if (index - 1 < 0)
        {
            index = num;
        }
        else
        {
            index--;
        }

        int counter = 0;


        while (index < 0 || !blueprintNames[index].typeB)
        {
            if (index < 0)
            {
                index = num;
            }
            else
            {
                index--;
            }

            counter++;
            if (counter > 100)
            {
                printf("Infinite loop while getting previous ship!");
                break;
            }
        }
    }
    else if (builder->currentType == 2)
    {
        if (index - 1 < 0)
        {
            index = num;
        }
        else
        {
            index--;
        }

        int counter = 0;

        while (index < 0 || !blueprintNames[index].typeC)
        {
            if (index < 0)
            {
                index = num;
            }
            else
            {
                index--;
            }

            counter++;
            if (counter > 100)
            {
                printf("Infinite loop while getting previous ship!");
                break;
            }
        }
    }
    else
    {
        if (index - 1 < 0)
        {
            index = num;
        }
        else
        {
            index--;
        }
    }

    if (index < 0)
    {
        index = num;
    }

    builder->SwitchShip(index + 100, builder->currentType);


    return false;
}


//==========================


HOOK_METHOD_PRIORITY(ShipManager, OnInit, 100, (ShipBlueprint *bp, int shipLevel) -> int)
{
    auto customSel = CustomShipSelect::GetInstance();

    int ret = super(bp, shipLevel);


    SM_EX(this)->isCustomShip = customSel->IsCustomShip(bp->blueprintName);

    return ret;
}

HOOK_METHOD(ShipButton, constructor, (int shipType, int shipVariant) -> void)
{
    super(shipType, shipVariant);
}

HOOK_METHOD(ResourceControl, GetImageId, (std::string& name) -> void*)
{
    return super(name);
}

HOOK_METHOD(ScoreKeeper, GetShipUnlocked, (int shipId, int shipVariant) -> bool)
{
    if (shipId >= 100)
        return true;
    return super(shipId, shipVariant);
}


HOOK_STATIC(ScoreKeeper, GetShipBlueprint, (std::string* str, ScoreKeeper* scoreKeeper, int index) -> std::string*)
{
    std::string* ret;
    if (index >= 100)
        ret = super(str, scoreKeeper, 0);
    else
        ret = super(str, scoreKeeper, index);

    if (index >= 100)
    {
        auto customSel = CustomShipSelect::GetInstance();
        str->assign(customSel->GetShipBlueprint(index));
    }


    return ret;
    //printf("%08X\n", str2);

}

HOOK_METHOD(UnlockArrow, MouseMove, (int x, int y) -> bool)
{
    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->ShouldRenderArrow())
    {
        return super(x, y);
    }

    return false;
}

HOOK_METHOD(UnlockArrow, OnRender, () -> void)
{
    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->ShouldRenderArrow())
    {
        super();
    }
}


HOOK_METHOD(ShipButton, OnRender, () -> void)
{
    auto customSel = CustomShipSelect::GetInstance();
    if (customSel->ShouldRenderButton(this))
    {
        if (customSel->ShouldRenderButtonLower() || bShipLocked || bNoExist)
        {
            super();
        }
        else
        {
            Button::OnRender();
            GL_Color white = GL_Color(1.f, 1.f, 1.f, 1.f);
            G_->GetResources()->RenderImage(iShipImage, position.x, position.y, 0, white, 1.f, false);
        }

    }


}

HOOK_METHOD(ShipSelect, MouseClick, () -> void)
{
    super();

    auto customSel = CustomShipSelect::GetInstance();
    customSel->MouseClick();
}

HOOK_METHOD(ShipSelect, MouseMove, (int x, int y) -> int)
{
    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->FirstPage())
    {
        super(x, y);
    }



    customSel->MouseMove(x, y);
}

HOOK_METHOD(ShipSelect, Open, () -> void)
{
    super();

    auto customSel = CustomShipSelect::GetInstance();
    customSel->OnInit(this);
}

HOOK_METHOD(ShipSelect, OnRender, () -> void)
{
    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->FirstPage())
    {
        super();
    }
    else
    {
        super();
        customSel->OnRender(true);
    }

    customSel->OnRender();
}



HOOK_METHOD(ShipBuilder, SwitchShip, (int shipType, int shipVariant) -> void)
{
    if (shipType >= 100)
    {
        auto customSel = CustomShipSelect::GetInstance();

        if (customSel->GetLastSelected() != -1)
        {
            customSel->SwitchShip(this, shipType, shipVariant);
            return;
        }
    }

    return super(shipType, shipVariant);
}

HOOK_STATIC(AchievementTracker, GetShipAchievements, (void* unk, AchievementTracker *tracker, std::string& id) -> void*)
{
    return super(unk, tracker, id);
}

HOOK_METHOD(ShipBuilder, MouseClick, (int x, int y) -> void)
{
    super(x, y);

    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->Initialized() && customSel->GetSelection() != -1)
    {
        customSel->SwitchShip(this, customSel->GetSelectedId(), shipSelect.currentType, true);
        customSel->ClearSelection();
    }
}

HOOK_METHOD(ShipBuilder, SwapType, (int variant) -> int)
{
    return super(variant);
}

HOOK_METHOD(ShipSelect, Close, () -> void)
{
    super();
    auto customSel = CustomShipSelect::GetInstance();
    customSel->Close();
}

HOOK_METHOD(ShipBuilder, CycleShipNext, () -> void)
{
    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->CycleShipNext(this))
        super();

}

HOOK_METHOD(ShipBuilder, CycleShipPrevious, () -> void)
{
    auto customSel = CustomShipSelect::GetInstance();

    if (customSel->CycleShipPrevious(this))
        super();

}

HOOK_METHOD(ShipBuilder, MouseClick, (int x, int y) -> void)
{
    if (randomButton.bActive && randomButton.bHover)
    {
        auto customSel = CustomShipSelect::GetInstance();
        if (currentShipId >= 100 && customSel->Initialized())
        {
            int id = customSel->GetRandomShipIndex();
            int variant = rand() % 3;

            if (variant == 2 && !customSel->GetShipDefinition(id).typeC)
                variant = 1;
            if (variant == 1 && !customSel->GetShipDefinition(id).typeB)
                variant = 0;

            customSel->SwitchShip(this, id + 100, variant, true);
        }
        else
        {
            super(x, y);
        }
    }
    else
    {
        super(x, y);
    }
}

HOOK_METHOD(ShipBuilder, OnLoop, () -> void)
{

    super();

    if (currentShipId >= 100)
    {
        auto customSel = CustomShipSelect::GetInstance();

        bool buttonsActive = false;

        buttonsActive = customSel->ShipCount(currentType) > 1;
        leftButton.SetActive(buttonsActive);
        rightButton.SetActive(buttonsActive);
        randomButton.SetActive(customSel->ShipCount() > 1);
    }

}


HOOK_METHOD_PRIORITY(ShipBuilder, OnRender, 1000, () -> void)
{

    bool renderAchievements = currentShipId < 100;

    if (shipSelect.bOpen)
    {
        GL_Color tint(0.25f, 0.25f, 0.25f, 1.f);
        CSurface::GL_SetColorTint(tint);
    }

    CSurface::GL_DisableBlend();
    CSurface::GL_RenderPrimitive(baseImage);

    CSurface::GL_EnableBlend();
    CSurface::GL_RenderPrimitive(shipEquipBox);
    CSurface::GL_RenderPrimitive(shipSelectBox);
    if (renderAchievements)
    {
        CSurface::GL_RenderPrimitive(shipAchBox);
    }

    CSurface::GL_RenderPrimitive(advancedButtonBox);

    CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

    TextLibrary* lib = G_->GetTextLibrary();

    std::string langTxt;
    TextLibrary::GetText(langTxt, lib, "hangar_frame_ship", lib->currentLanguage);
    freetype::easy_print(62, 19, 117, langTxt);

    TextLibrary::GetText(langTxt, lib, "hangar_frame_layout", lib->currentLanguage);
    freetype::easy_print(62, 19, 219, langTxt);

    TextLibrary::GetText(langTxt, lib, "hangar_frame_crew", lib->currentLanguage);
    freetype::easy_print(62, 19, 489, langTxt);

    TextLibrary::GetText(langTxt, lib, "equipment_frame_weapons", lib->currentLanguage);
    freetype::easy_print(62, 388, 489, langTxt);

    TextLibrary::GetText(langTxt, lib, "equipment_frame_drones", lib->currentLanguage);
    freetype::easy_print(62, 388, 599, langTxt);

    TextLibrary::GetText(langTxt, lib, "equipment_frame_augments", lib->currentLanguage);
    freetype::easy_print(62, 954, 489, langTxt);

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    if (renderAchievements)
    {
        TextLibrary::GetText(langTxt, lib, "hangar_achievements_title", lib->currentLanguage);
        freetype::easy_printCenter(13, 124, 374, langTxt);
    }



    TextLibrary::GetText(langTxt, lib, "hangar_advanced_title", lib->currentLanguage);
    freetype::easy_printCenter(13, 1109, 400, langTxt);

    CSurface::GL_SetColor(1.f, 1.f, 1.f, 1.f);
    CSurface::GL_PushMatrix();

    if (!Settings::GetDlcEnabled() && (currentShipId == 9 || currentType == 2))
    {
        CSurface::GL_SetColorTint(COLOR_TINT);
    }

    CSurface::GL_Translate(currentShip->ship.horizontal_shift + 365.f, currentShip->ship.vertical_shift + 30.f);
    currentShip->OnRender(true, false);

    CSurface::GL_PopMatrix();
    CSurface::GL_RenderPrimitive(startButtonBox);
    if (!Settings::GetDlcEnabled() && (currentShipId == 9 || currentType == 2))
    {
        CSurface::GL_RemoveColorTint();
        CSurface::GL_RenderPrimitive(enableAdvancedPrimitive);
    }


    startButton.SetActive(currentShipId != 9 || currentShipId != 2);

    for (auto &anim: animations)
    {
        anim.OnRender(1.f, COLOR_WHITE, false);
    }

    walkingMan.OnRender(1.f, COLOR_WHITE, false);

    CSurface::GL_RenderPrimitive(nameBoxPrimitive);

    for (auto &box: vEquipmentBoxes)
    {
        if (box->CanDoJob())
        {
            if (bCustomizingCrew)
            {
                if (!box->CanHoldCrew())
                {
                    box->OnRender(false);
                    box->RenderLabels(false);
                }
            }
            else
            {
                box->OnRender(false);
                box->RenderLabels(false);
            }
        }

    }

    for (auto &box: vCrewBoxes)
    {
        if (box->bCustomizing)
        {
            box->OnRender(false);
            break;
        }
    }

    for (auto &button: buttons)
    {
        button->OnRender();
    }

    for (auto &sys: sysBoxes)
    {
        sys->OnRender(true);
    }


    if (encourageShipList)
    {
        int buttonX = listButton.position.x;
        int buttonY = listButton.position.y;


        CSurface::GL_BlitPixelImage(arrow, buttonX + 75, buttonY - 4, 164, 38, 0, COLOR_WHITE, true);
        descBox->Draw(buttonX + 245, buttonY - 47);

        std::string txt;
        TextLibrary::GetText(txt, lib, "tutorial_list_open", lib->currentLanguage);

        freetype::easy_printAutoNewlines(12, buttonX + 265, buttonY - 27, 308, txt);
    }


    CSurface::GL_SetColor(COLOR_WHITE);

    Point shipNamePos = Point(273, 7);
    nameInput.OnRender(24, shipNamePos);

    // FIXED WTIH freetype_hack

    // This doesn't work because of easy_measurePrintLines returning a Pointf
    // Need a way to get the y value of the returned Pointf
    // The Pointf is returned split into eax and edx

    // FIXED WITH freetype_hack
    if (bRenaming)
    {
        std::string txt;
        TextLibrary::GetText(txt, lib, "rename", lib->currentLanguage);
        Pointf ret = freetype_hack::easy_measurePrintLines(12, 0, 0, 999, txt);
        float x = 6.f;
        float x2 = 227.f - ret.x / 2;
        if (x2 > 5)
            x = x2;
        freetype::easy_print(12, x, 65.f, txt);
    }

    if (!currentShip->HasSystem(3))
    {
        std::string txt;

        TextLibrary::GetText(txt, lib, "equipment_no_system", lib->currentLanguage);
        freetype::easy_printCenter(63, 662, 539, txt);
    }
    if (!currentShip->HasSystem(4))
    {
        std::string txt;

        TextLibrary::GetText(txt, lib, "equipment_no_system", lib->currentLanguage);
        freetype::easy_printCenter(63, 662, 649, txt);
    }

    CSurface::GL_SetColor(COLOR_WHITE);


    if (renderAchievements)
    {
        int counter = 0;
        for (auto &ach: shipAchievements)
        {
            Point pos(ach.position.x, ach.position.y);
            ach.achievement->OnRender(pos, selectedAch == counter, 1);
            counter++;
        }
    }


    CSurface::GL_RemoveColorTint();
    if (shipSelect.bOpen)
    {
        shipSelect.OnRender();
    }
    else
    {
        infoBox.OnRender();
    }

    introScreen.OnRender();
}


HOOK_METHOD_PRIORITY(MenuScreen, OnRender, 1000, () -> void)
{
    if (G_->GetWorld()->playerShip)
    {
        if (!SM_EX(G_->GetWorld()->playerShip->shipManager)->isCustomShip)
        {
            return super();
        }
    }

    if (bShowControls || G_->GetTutorialManager()->Running())
    {
        return super();
    }

    if (!menuPrimitive)
    {
        std::vector<GL_TexVertex>* texVertices = new std::vector<GL_TexVertex>();
        GL_Texture *img = mainImage;
        float unk1 = 81.f;
        float unk2 = 1.f;
        float unk3 = 1.f;

        if (img)
        {
            unk1 = 81.f / img->width_;
            unk2 = img->width_;
            unk3 = img->height_;
        }

        CSurface::AddTexVertices(texVertices, position.x, position.y, 81.f, unk3, 0, unk1, 0, 1.f);
        CSurface::AddTexVertices(texVertices, position.x + 81.f, position.y, menuWidth - 161.f, unk3, unk1, 82.f / unk2, 0, 1.f);
        CSurface::AddTexVertices(texVertices, (menuWidth + position.x) - 80.f, position.y, 80.f, unk3, 82.f / unk2, 162.f / unk2, 0, 1.f);
        menuPrimitive = CSurface::GL_CreateMultiImagePrimitive(mainImage, texVertices, COLOR_WHITE);
    }


    if (confirmDialog.bOpen)
    {
        CSurface::GL_SetColorTint(COLOR_TINT);
    }

    CSurface::GL_RenderPrimitive(menuPrimitive);

    for (auto &button : buttons)
    {
        button->OnRender();
    }

    CSurface::GL_BlitPixelImageWide(difficultyBox,
                                    statusPosition.x,
                                    statusPosition.y,
                                    difficultyWidth,
                                    72,
                                    1.f,
                                    COLOR_WHITE,
                                    false);

    CSurface::GL_BlitPixelImageWide(dlcBox,
                                    statusPosition.x + difficultyWidth + 3,
                                    statusPosition.y,
                                    dlcWidth,
                                    72,
                                    1.f,
                                    COLOR_WHITE,
                                    false);

    CSurface::GL_SetColor(COLOR_BUTTON_ON);

    freetype::easy_printCenter(13, statusPosition.x + (difficultyWidth + 1.f) / 2.f, statusPosition.y + 16.f, difficultyLabel);
    freetype::easy_printCenter(13, statusPosition.x + difficultyWidth + (dlcWidth + 1.f) / 2.f + 3.f, statusPosition.y + 16.f, dlcLabel);

    CSurface::GL_SetColor(COLOR_BUTTON_TEXT);

    freetype::easy_printCenter(62, statusPosition.x + (difficultyWidth + 1.f) / 2.f, statusPosition.y + 40, difficultyText);
    freetype::easy_printCenter(62, statusPosition.x + difficultyWidth + (dlcWidth + 1.f) / 2.f + 3.f, statusPosition.y + 40.f, dlcText);

    if (confirmDialog.bOpen)
    {
        CSurface::GL_RemoveColorTint();
        confirmDialog.OnRender();
    }
}
