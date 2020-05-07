#include "CustomCrew.h"

#include <boost/lexical_cast.hpp>
#include <algorithm>


static const std::string CREW_SKILLS[6] =
{
    "piloting",
    "engines",
    "shields",
    "weapons",
    "repair",
    "combat"
};

CustomCrewManager CustomCrewManager::instance = CustomCrewManager();


void CustomCrewManager::AddCrewDefinition(CrewDefinition crew)
{
    blueprintNames[crew.race] = crew;
}


void CustomCrewManager::ParseCrewNode(rapidxml::xml_node<char> *node)
{
    try
    {
        for (auto child = node->first_node(); child; child = child->next_sibling())
        {
            if (child->first_attribute("name"))
            {
                std::string name = std::string(child->first_attribute("name")->value());

                CrewDefinition crew;

                crew.race = name;
                crew.deathSounds = std::vector<std::string>();
                crew.shootingSounds = std::vector<std::string>();

                crew.skillsDef = SkillsDefinition();

                try
                {
                    for (auto stat = child->first_node(); stat; stat = stat->next_sibling())
                    {
                        std::string str = std::string(stat->name());
                        std::string val = std::string(stat->value());

                        if (str == "canFight")
                        {
                            crew.canFight = EventsParser::ParseBoolean(val);
                        }
                        if (str == "canSuffocate")
                        {
                            crew.canSuffocate = EventsParser::ParseBoolean(val);
                        }
                        if (str == "canBurn")
                        {
                            crew.canBurn = EventsParser::ParseBoolean(val);
                        }
                        if (str == "maxHealth")
                        {
                            crew.maxHealth = boost::lexical_cast<int>(val);
                        }
                        if (str == "moveSpeedMultiplier")
                        {
                            crew.moveSpeedMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "repairSpeed")
                        {
                            crew.repairSpeed = boost::lexical_cast<float>(val);
                        }
                        if (str == "damageMultiplier")
                        {
                            crew.damageMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "fireRepairMultiplier")
                        {
                            crew.fireRepairMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "suffocationModifier")
                        {
                            crew.suffocationModifier = boost::lexical_cast<float>(val);
                        }
                        if (str == "providesPower")
                        {
                            crew.providesPower = EventsParser::ParseBoolean(val);
                        }
                        if (str == "isTelepathic")
                        {
                            crew.isTelepathic = EventsParser::ParseBoolean(val);
                        }
                        if (str == "isAnaerobic")
                        {
                            crew.isAnaerobic = EventsParser::ParseBoolean(val);
                        }
                        if (str == "fireDamageMultiplier")
                        {
                            crew.fireDamageMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "canPhaseThroughDoors")
                        {
                            crew.canPhaseThroughDoors = EventsParser::ParseBoolean(val);
                        }
                        if (str == "deathSounds")
                        {
                            for (auto deathSoundNode = stat->first_node(); deathSoundNode; deathSoundNode = deathSoundNode->next_sibling())
                            {
                                if (strcmp(deathSoundNode->name(), "deathSound") == 0)
                                {
                                    //printf("%s\n", deathSoundNode->value());
                                    crew.deathSounds.push_back(std::string(deathSoundNode->value()));
                                }
                            }
                        }
                        if (str == "shootingSounds")
                        {
                            for (auto shootingSoundNode = stat->first_node(); shootingSoundNode; shootingSoundNode = shootingSoundNode->next_sibling())
                            {
                                if (strcmp(shootingSoundNode->name(), "shootingSound") == 0)
                                {
                                    crew.shootingSounds.push_back(std::string(shootingSoundNode->value()));
                                }
                            }
                        }
                        if (str == "oxygenChangeSpeed")
                        {
                            crew.oxygenChangeSpeed = boost::lexical_cast<float>(val);
                        }
                        if (str == "damageTakenMultiplier")
                        {
                            crew.damageTakenMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "passiveHealAmount")
                        {
                            crew.passiveHealAmount = boost::lexical_cast<float>(val);
                        }
                        if (str == "passiveHealDelay")
                        {
                            crew.passiveHealAmount = boost::lexical_cast<int>(val);
                        }
                        if (str == "detectsLifeforms")
                        {
                            crew.detectsLifeforms = EventsParser::ParseBoolean(val);
                        }
                        if (str == "skills")
                        {
                            for (auto skillNode = stat->first_node(); skillNode; skillNode = skillNode->next_sibling())
                            {
                                std::string skillName = std::string(skillNode->name());

                                for (auto i : CREW_SKILLS)
                                {
                                    if (skillName == i)
                                    {
                                        if (skillNode->first_attribute("req"))
                                        {
                                            crew.skillsDef.skills[i].requirement = boost::lexical_cast<int>(skillNode->first_attribute("req")->value());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                catch (boost::bad_lexical_cast const &e)
                {
                    MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR);
                }

                AddCrewDefinition(crew);
            }
        }
    }
    catch (std::exception)
    {
        MessageBoxA(NULL, "Error parsing <crew> in hyperspace.xml", "Error", MB_ICONERROR);
    }

}


CrewMember* CustomCrewManager::CreateCrewMember(CrewBlueprint* bp, int shipId, bool intruder)
{
    std::string race = bp->name;
    if (!IsRace(race))
        return 0;

    Pointf unk = Pointf(0.f, 0.f);
    CrewAnimation* animation = new CrewAnimation(shipId, race, unk, intruder);

    CrewDefinition def = GetDefinition(race);

    CrewMember *crew;
    crew = new CrewMember(*bp, shipId, intruder, animation);

    return crew;
}

bool CustomCrewManager::IsRace(const std::string& race)
{
    return std::count_if(blueprintNames.begin(),
                         blueprintNames.end(),
                         [race](std::pair<std::string, CrewDefinition> i) { return i.first == race; } )
                          > 0;
}









//=================================

HOOK_METHOD_PRIORITY(CrewMember, constructor, -899, (CrewBlueprint& bp, int shipId, bool intruder, CrewAnimation *animation) -> void)
{

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    super(bp, shipId, intruder, animation);

    CrewMember_Extend* ex = CM_EX(this);


    if (custom->IsRace(species))
    {
        ex->passiveHealTimer = new TimerHelper();
        ex->passiveHealTimer->Start(custom->GetDefinition(species).passiveHealDelay);


        auto skillsDef = custom->GetDefinition(species).skillsDef;

        blueprint.skillLevel[0].second = skillsDef.skills["piloting"].requirement * 2;
        blueprint.skillLevel[1].second = skillsDef.skills["engines"].requirement * 2;
        blueprint.skillLevel[2].second = skillsDef.skills["shields"].requirement * 2;
        blueprint.skillLevel[3].second = skillsDef.skills["weapons"].requirement * 2;
        blueprint.skillLevel[4].second = skillsDef.skills["repair"].requirement * 2;
        blueprint.skillLevel[5].second = skillsDef.skills["combat"].requirement * 2;
    }



}

HOOK_METHOD_PRIORITY(CrewMember, UpdateHealth, 2000, () -> void)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (iOnFire && CanBurn())
    {
        float fireMultiplier = 1.f;

        if (custom->IsRace(species))
        {
            fireMultiplier = custom->GetDefinition(species).fireDamageMultiplier;
        }

        DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * -0.133f * fireMultiplier);
    }

    if (bSuffocating && !IsAnaerobic())
    {
        float mod = 1.f;

        ShipObject* shipObject = GetShipObject();

        ShipInfo* shipInfo = G_->GetShipInfo(iShipId);

        if (shipObject->HasAugmentation("O2_MASKS"))
        {
            mod = shipObject->GetAugmentationValue("O2_MASKS");
        }

        float multiplier = GetSuffocationMultiplier();

        DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * -0.4f * mod * multiplier);
    }

    float mod = 1.f;
    if (IsDrone())
    {
        mod = 0.2f;
    }

    DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * fMedbay * mod * 0.4f);
    CrewMember_Extend* ex = CM_EX(this);
    if (ex->isHealing)
    {
        if (custom->IsRace(species))
        {
            DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * custom->GetDefinition(species).passiveHealAmount * 0.4f);
        }
    }
    //super();
}
HOOK_METHOD_PRIORITY(CrewMember, DirectModifyHealth, 1000, (float health) -> void)
{
    super(health);
    if (health < 0.f)
    {
        CrewMember_Extend* ex = CM_EX(this);
        ex->isHealing = false;
        CustomCrewManager *custom = CustomCrewManager::GetInstance();

        if (custom->IsRace(species))
        {
            ex->passiveHealTimer->Start(custom->GetDefinition(species).passiveHealDelay);
        }
    }
}
HOOK_METHOD_PRIORITY(CrewMember, OnLoop, 1000, () -> void)
{
    super();
    CrewMember_Extend* ex = CM_EX(this);

    auto custom = CustomCrewManager::GetInstance();

    if (custom->IsRace(species))
    {
        ex->passiveHealTimer->Update();
        if (ex->passiveHealTimer->Done())
        {
            ex->isHealing = true;
        }
    }
}
HOOK_METHOD_PRIORITY(CrewMember, GetNewGoal, 2000, () -> bool)
{
    auto ex = CM_EX(this);

    if (last_door && !ex->canPhaseThroughDoors)
    {
        last_door->FakeClose();
    }


    last_door = 0;

    bool result = false;


    if (path.distance != -1.f)
    {
        Point pos = Point(std::floor(x), std::floor(y));

        int roomId = iRoomId;
        ShipGraph* shipGraph = ShipGraph::GetShipInfo(currentShipId);
        Point intoRoom = shipGraph->GetIntoRoom(roomId, pos);

        if (path.doors.size() > 0)
        {
            Door* door = path.doors.front();

            Point doorPos = door->GetCenterPoint();
            Point entryWay = door->GetEntryWay(iRoomId);

            Point gridPos = ShipGraph::TranslateToGrid(pos.x, pos.y);
            Point entryPos = ShipGraph::TranslateToGrid(entryWay.x, entryWay.y);

            if (entryPos.x != gridPos.x || entryPos.y != gridPos.y)
            {
                if (intoRoom.x == -1 || intoRoom.y == -1)
                {
                    goal_x = entryWay.x;
                    goal_y = entryWay.y;
                }
                else
                {
                    goal_x = intoRoom.x;
                    goal_y = intoRoom.y;
                }
                return true;
            }

            if (door->IsSealed(iShipId) && (!bMindControlled || door->lockedDown.running) && !ex->canPhaseThroughDoors)
            {
                bool up = false;
                bool right = false;
                bool down = false;
                bool left = false;

                if (blockingDoor)
                {
                    blockingDoor = door;
                    SetCurrentTarget(door, false);
                    return true;
                }
                else
                {
                    if (doorPos.y > entryWay.y)
                    {
                        down = true;
                    }
                    else if (doorPos.y < entryWay.y)
                    {
                        up = true;
                    }

                    if (doorPos.x > entryWay.x)
                    {
                        right = true;
                    }
                    else if (doorPos.x < entryWay.x)
                    {
                        left = true;
                    }

                    int offset = (G_->GetRNG() ? rand() : random32()) % 17;
                    if (door->bVertical)
                    {
                        if (left)
                        {
                            goal_x = doorPos.x + 8;
                        }
                        else
                        {
                            goal_x = doorPos.x - 8;
                        }

                        goal_y = offset + (doorPos.y - 8);

                    }
                    else
                    {
                        if (up)
                        {
                            goal_y = doorPos.y + 8;
                        }
                        else
                        {
                            goal_y = doorPos.y - 8;
                        }

                        goal_x = offset + (doorPos.x - 8);
                    }

                    blockingDoor = door;
                    SetCurrentTarget(door, false);
                    return true;
                }
            }

            if (door->bVertical)
            {
                if (std::abs(pos.y - entryWay.y) > 0.05)
                {
                    goal_x = x;
                    goal_y = entryWay.y;
                    return true;
                }
                if (doorPos.x < entryWay.x)
                {
                    goal_x = entryWay.x - 35;
                    goal_y = y;
                }
                if (doorPos.x > entryWay.x)
                {
                    goal_x = entryWay.x + 34;
                    goal_y = y;
                }
                path.doors.erase(path.doors.begin());
            }
            else
            {
                if (std::abs(pos.x - entryWay.x) > 0.05)
                {
                    goal_y = y;
                    goal_x = entryWay.x;
                    return true;
                }
                if (doorPos.y < entryWay.y)
                {
                    goal_y = entryWay.y - 35;
                    goal_x = x;
                }
                if (doorPos.y > entryWay.y)
                {
                    goal_y = entryWay.y + 34;
                    goal_x = x;
                }



                path.doors.erase(path.doors.begin());

            }

            if (!door->bOpen && !door->forcedOpen.running)
            {
                if (!ex->canPhaseThroughDoors)
                {
                    last_door = door;
                    door->FakeOpen();
                }
                else
                {
                    G_->GetSoundControl()->PlaySoundMix("phase", -1.f, false);
                }
            }


            return true;
        }
        else
        {
            if (intoRoom.x != -1 && intoRoom.y != -1)
            {
                goal_x = intoRoom.x;
                goal_y = intoRoom.y;

                return true;
            }
            else
            {
                Point finish = path.finish;
                goal_x = finish.x;
                goal_y = finish.y;

                if ( std::abs(path.finish.x - x) > 0.5 )
                {
                    return true;
                }

                x = path.finish.x;

                if ( std::abs(path.finish.y - y) > 0.5 )
                {
                    return true;
                }

                y = path.finish.y;

                path.distance = -1.f;
            }
        }
    }

    return result;
}



HOOK_METHOD(CrewMemberFactory, CreateCrewMember, (CrewBlueprint* bp, int shipId, bool intruder) -> CrewMember*)
{
    //printf("%s\n", bp->name.c_str());
    auto custom = CustomCrewManager::GetInstance();
    CrewMember *newCrew = custom->CreateCrewMember(bp, shipId, intruder);

    if (newCrew)
    {
        crewMembers.push_back(newCrew);
        OnLoop();
        return newCrew;
    }

    return super(bp, shipId, intruder);
}

HOOK_STATIC(CrewMemberFactory, IsRace, (std::string& race) -> bool)
{
    auto custom = CustomCrewManager::GetInstance();

    if (custom->IsRace(race))
        return true;
    return super(race);
}

HOOK_METHOD(CrewMember, constructor, (CrewBlueprint& blueprint, int shipId, bool intruder, CrewAnimation *animation) -> void)
{
	super(blueprint, shipId, intruder, animation);

    auto custom = CustomCrewManager::GetInstance();
    if (custom->IsRace(species))
    {
        CrewMember_Extend* ex = CM_EX(this);
        ex->canPhaseThroughDoors = custom->GetDefinition(species).canPhaseThroughDoors;
    }
}

HOOK_STATIC(CrewAnimation, GetDeathSound, (std::string& strRef, CrewAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.deathSounds.size() > 0)
        {
            int rng = 0;
            if (G_->GetRNG())
            {
                rng = rand();
            }
            else
            {
                rng = random32();
            }

            strRef.assign(def.deathSounds[rng % def.deathSounds.size()]);

            return strRef;
        }
    }

    return ret;
}

HOOK_STATIC(CrewAnimation, GetShootingSound, (std::string& strRef, CrewAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.shootingSounds.size() > 0)
        {
            int rng = 0;
            if (G_->GetRNG())
            {
                rng = rand();
            }
            else
            {
                rng = random32();
            }

            strRef.assign(def.shootingSounds[rng % def.shootingSounds.size()]);

            return strRef;
        }
    }

    return ret;
}


HOOK_METHOD(ShipManager, UpdateEnvironment, () -> void)
{
    super();

    if (!G_->GetCApp()->menu.shipBuilder.bOpen && systemKey[2] != -1)
    {
        for (auto const &x: vCrewList)
        {
            auto custom = CustomCrewManager::GetInstance();

            if (custom->IsRace(x->species))
            {
                float oxygenModifier = custom->GetDefinition(x->species).oxygenChangeSpeed;
                if (oxygenModifier != 0.f && !x->bDead)
                {
                    oxygenSystem->ComputeAirLoss(x->iRoomId, -oxygenModifier, true);
                }
            }
        }
    }
}


HOOK_METHOD(CrewMember, ApplyDamage, (float damage) -> bool)
{
    auto custom = CustomCrewManager::GetInstance();

    if (custom->IsRace(species))
    {
        damage *= custom->GetDefinition(species).damageTakenMultiplier;
    }

    return super(damage);
}


static bool crewDetectLifeforms = false;

HOOK_METHOD(ShipManager, OnRender, (bool unk1, bool unk2) -> void)
{
    crewDetectLifeforms = true;
    super(unk1, unk2);
    crewDetectLifeforms = false;
}

HOOK_METHOD(ShipObject, HasEquipment, (const std::string& name) -> int)
{
    if (name == "slug" && crewDetectLifeforms)
    {
        int ret = super(name);

        if (ret == 0)
        {
            auto custom = CustomCrewManager::GetInstance();

            ShipManager *currentShip;
            if (iShipId == 0)
            {
                currentShip = G_->GetWorld()->playerShip->shipManager;
            }
            else
            {
                currentShip = G_->GetWorld()->playerShip->shipManager->current_target;
            }


            for (auto i : currentShip->vCrewList)
            {
                if (custom->IsRace(i->species))
                {
                    if (custom->GetDefinition(i->species).detectsLifeforms)
                    {
                        return 1;
                    }
                }
            }
        }

        return ret;
    }

    return super(name);
}
