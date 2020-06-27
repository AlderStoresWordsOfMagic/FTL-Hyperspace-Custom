#include "CustomAugments.h"
#include "Global.h"
#include <boost/lexical_cast.hpp>

CustomAugmentManager CustomAugmentManager::instance = CustomAugmentManager();

void CustomAugmentManager::ParseCustomAugmentNode(rapidxml::xml_node<char>* node)
{
    try
    {
        for (auto child = node->first_node(); child; child = child->next_sibling())
        {
            if (strcmp(child->name(), "aug") == 0)
            {
                const std::string& augName = child->first_attribute("name")->value();
                AugmentDefinition* augDef = new AugmentDefinition();

                augDef->name = augName;
                augDef->functions = std::map<std::string, std::pair<float, bool>>();

                for (auto functionNode = child->first_node(); functionNode; functionNode = functionNode->next_sibling())
                {
                    if (strcmp(functionNode->name(), "function") == 0)
                    {
                        const std::string& functionName = functionNode->first_attribute("name")->value();

                        if (functionName != augName)
                        {
                            float functionValue = G_->GetBlueprints()->GetAugmentValue(functionName);
                            bool preferHigher = true;

                            if (functionNode->first_attribute("value"))
                            {
                                functionValue = boost::lexical_cast<float>(functionNode->first_attribute("value")->value());
                            }
                            if (functionNode->first_attribute("preferHigher"))
                            {
                                preferHigher = EventsParser::ParseBoolean(functionNode->first_attribute("preferHigher")->value());
                            }


                            augDef->functions[functionName] = std::pair<float, bool>(functionValue, preferHigher);
                        }
                    }
                }

                augDefs[augName] = augDef;
            }
        }
    }
    catch (...)
    {
        MessageBoxA(NULL, "Error parsing <augments> in hyperspace.xml", "Error", MB_ICONERROR);
    }
}

std::map<std::string, std::pair<float, bool>> CustomAugmentManager::GetPotentialAugments(const std::string& name)
{
    auto ret = std::map<std::string, std::pair<float, bool>>();


    for (auto const& i: augDefs)
    {
        auto val = i.second->functions.find(name);
        if (val != i.second->functions.end())
        {
            ret[i.second->name] = val->second;
        }
    }

    return ret;
}


//======================

HOOK_METHOD_PRIORITY(ShipObject, HasAugmentation, 2000, (const std::string& name) -> int)
{
    auto ship = G_->GetShipManager(iShipId);
    std::map<std::string, int> augList;

    if (ship == NULL)
    {
        augList = G_->GetShipInfo(iShipId)->augList;
    }
    else
    {
        auto ex = SM_EX(G_->GetShipManager(iShipId));
        augList = ex->GetAugmentList();
    }


    AugmentBlueprint* augBlueprint = G_->GetBlueprints()->GetAugmentBlueprint(name);

    int augCount = 0;

    if (augList.count(name) > 0)
    {
        augCount = augList.at(name);
    }

    CustomAugmentManager* customAug = CustomAugmentManager::GetInstance();
    std::map<std::string, std::pair<float, bool>> potentialAugs = customAug->GetPotentialAugments(name);



    for (auto const& x: potentialAugs)
    {
        if (augList.count(x.first))
        {
            augCount += augList.at(x.first);
        }
    }


    return augCount;
}

HOOK_METHOD_PRIORITY(ShipObject, HasEquipment, 2000, (const std::string& name) -> int)
{
    ItemBlueprint* bp = G_->GetBlueprints()->GetItemBlueprint(name);


        auto ship = G_->GetShipManager(iShipId);
        std::map<std::string, int> augList;

        if (ship == NULL)
        {
            augList = G_->GetShipInfo(iShipId)->augList;
        }
        else
        {
            auto ex = SM_EX(G_->GetShipManager(iShipId));
            augList = ex->GetAugmentList();
        }

        int augCount = 0;

        if (augList.count(name) > 0)
        {
            augCount = augList.at(name);
        }

        CustomAugmentManager* customAug = CustomAugmentManager::GetInstance();
        std::map<std::string, std::pair<float, bool>> potentialAugs = customAug->GetPotentialAugments(name);



        for (auto const& x: potentialAugs)
        {
            if (augList.count(x.first))
            {
                augCount += augList.at(x.first);
            }
        }

    if (augCount >= 0)
        return augCount;

    return super(name);


}

HOOK_METHOD_PRIORITY(ShipObject, GetAugmentationValue, 1000, (const std::string& name) -> float)
{
    AugmentBlueprint* augBlueprint = G_->GetBlueprints()->GetAugmentBlueprint(name);

    auto ship = G_->GetShipManager(iShipId);
    std::map<std::string, int> augList;

    if (ship == NULL)
    {
        augList = G_->GetShipInfo(iShipId)->augList;
    }
    else
    {
        auto ex = SM_EX(G_->GetShipManager(iShipId));
        augList = ex->GetAugmentList();
    }
    int augCount = 0;

    if (augList.count(name) > 0)
    {
        augCount = augList.at(name);
    }


    float augValue = augBlueprint->value * augCount;

    CustomAugmentManager* customAug = CustomAugmentManager::GetInstance();
    std::map<std::string, std::pair<float, bool>> potentialAugs = customAug->GetPotentialAugments(name);


    float highestValue = augBlueprint->value;
    for (auto const& x: potentialAugs)
    {
        if (augList.count(x.first))
        {
            augCount += augList.at(x.first);
            augValue += x.second.first * augList.at(x.first);

            if ((x.second.second && x.second.first > highestValue) || (!x.second.second && x.second.first < highestValue))
            {
                highestValue = x.second.first;
            }
        }
    }

    if (augCount <= 0)
    {
        return 0.f;
    }

    return augBlueprint->stacking ? augValue : highestValue;
}

HOOK_METHOD(EquipmentBox, SetBlueprint, (InfoBox *infoBox, bool unk) -> void)
{
    super(infoBox, unk);
    CustomAugmentManager* customAug = CustomAugmentManager::GetInstance();

    if (item.augment)
    {
        if (customAug->IsAugment(item.augment->name))
        {
            std::string warn = G_->GetTextLibrary()->GetText("augment_no_effect");
            warn.append("\n");

            BlueprintManager* blueprints = G_->GetBlueprints();
            int counter = 0;
            for (auto const &x: customAug->GetAugmentDefinition(item.augment->name)->functions)
            {
                auto bp = blueprints->GetAugmentBlueprint(x.first);
                if (((x.second.second && bp->value >= item.augment->value) || (!x.second.second && bp->value <= item.augment->value)) && !item.augment->stacking)
                {
                    warn += bp->desc.title.GetText() + "\n";
                    counter++;
                }
            }

            if (counter > 0)
                infoBox->warning.assign(warn);
        }
    }
}

HOOK_METHOD(InfoBox, SetBlueprintAugment, (const AugmentBlueprint* bp) -> void)
{
    desc.title.data.assign(bp->desc.title.data);
    desc.title.isLiteral = bp->desc.title.isLiteral;

    desc.shortTitle.data.assign(bp->desc.shortTitle.data);
    desc.shortTitle.isLiteral = bp->desc.shortTitle.isLiteral;


    desc.description.data.assign(bp->desc.description.data);
    desc.description.isLiteral = bp->desc.description.isLiteral;

    desc.cost = bp->desc.cost;
    desc.rarity = bp->desc.rarity;
    desc.baseRarity = bp->desc.baseRarity;
    desc.bp = bp->desc.bp;
    desc.locked = bp->desc.locked;

    descBoxSize.x = 305;
    descBoxSize.y = 266;
    primaryBoxOffset = 67;

    primaryBox = new WindowFrame(7, 7, 323, 252);

    bDetailed = false;
}
