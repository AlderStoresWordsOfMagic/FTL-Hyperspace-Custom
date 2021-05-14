#include "CustomCrew.h"
#include "CustomOptions.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Resources.h"
#include "freetype.h"
#include "StatBoost.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
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

bool g_advancedCrewTooltips = true;
bool g_showEnemyPowers = true;
bool g_showAllyPowers = false;
int g_advancedCrewTooltipRounding = 2;


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
                crew.repairSounds = std::vector<std::string>();

                crew.repairSounds.push_back("repair");

                crew.skillsDef = SkillsDefinition();
                crew.explosionDef = Damage();

                crew.explosionDef.iDamage = 0;
                crew.explosionDef.iShieldPiercing = 0;
                crew.explosionDef.fireChance = 0;
                crew.explosionDef.breachChance = 0;
                crew.explosionDef.stunChance = 0;
                crew.explosionDef.iIonDamage = 0;
                crew.explosionDef.iSystemDamage = 0;
                crew.explosionDef.iPersDamage = 0;
                crew.explosionDef.bHullBuster = false;
                crew.explosionDef.ownerId = -1;
                crew.explosionDef.selfId = -1;
                crew.explosionDef.bLockdown = false;
                crew.explosionDef.crystalShard = false;
                crew.explosionDef.bFriendlyFire = false;
                crew.explosionDef.iStun = 0;
                crew.explosionShipFriendlyFire = false;

                crew.powerDef.damage = Damage();
                crew.powerDef.damage.iDamage = 0;
                crew.powerDef.damage.iShieldPiercing = 0;
                crew.powerDef.damage.fireChance = 0;
                crew.powerDef.damage.breachChance = 0;
                crew.powerDef.damage.stunChance = 0;
                crew.powerDef.damage.iIonDamage = 0;
                crew.powerDef.damage.iSystemDamage = 0;
                crew.powerDef.damage.iPersDamage = 0;
                crew.powerDef.damage.bHullBuster = false;
                crew.powerDef.damage.ownerId = -1;
                crew.powerDef.damage.selfId = -1;
                crew.powerDef.damage.bLockdown = false;
                crew.powerDef.damage.crystalShard = false;
                crew.powerDef.damage.bFriendlyFire = false;
                crew.powerDef.damage.iStun = 0;
                crew.powerDef.sounds = std::vector<std::string>();
                crew.powerDef.buttonLabel = TextString();
                crew.powerDef.cooldownColor = GL_Color(133.f / 255.f, 231.f / 255.f, 237.f / 255.f, 1.f);
                crew.powerDef.tempPower = TemporaryPowerDefinition();
                crew.powerDef.tempPower.cooldownColor = GL_Color(1.f, 1.f, 1.f, 1.f);
                crew.powerDef.playerReq = ActivatedPowerRequirements();
                crew.powerDef.enemyReq = ActivatedPowerRequirements();

                if (child->first_attribute("drone"))
                {
                    std::string droneType = child->first_attribute("drone")->value();

                    crew.canBurn = false;
                    crew.canRepair = false;
                    crew.canSabotage = false;
                    crew.canSuffocate = false;
                    crew.controllable = false;
                    crew.canFight = false;
                    crew.canMan = false;
                    crew.moveSpeedMultiplier = 0.5f;

                    if (droneType == "BOARDER")
                    {
                        crew.canFight = true;
                        crew.canSabotage = true;
                        crew.damageMultiplier = 1.2f;
                        crew.maxHealth = 150;
                    }
                    if (droneType == "BATTLE")
                    {
                        crew.canFight = true;
                        crew.damageMultiplier = 1.2f;
                        crew.maxHealth = 150;
                    }
                    if (droneType == "BOARDER_ION")
                    {
                        crew.canSabotage = true;
                        crew.maxHealth = 125;
                    }
                    if (droneType == "REPAIR")
                    {
                        crew.canRepair = true;
                        crew.maxHealth = 25;
                        crew.repairSpeed = 2.f;
                    }
                }

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
                        if (str == "controllable")
                        {
                            crew.controllable = EventsParser::ParseBoolean(val);
                        }
                        if (str == "canRepair")
                        {
                            crew.canRepair = EventsParser::ParseBoolean(val);
                        }
                        if (str == "canSabotage")
                        {
                            crew.canSabotage = EventsParser::ParseBoolean(val);
                        }
                        if (str == "canMan")
                        {
                            crew.canMan = EventsParser::ParseBoolean(val);
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
                        if (str == "stunMultiplier")
                        {
                            crew.stunMultiplier = boost::lexical_cast<float>(val);
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
                        if (str == "rangedDamageMultiplier")
                        {
                            crew.rangedDamageMultiplier = boost::lexical_cast<float>(val);
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
                        if (str == "bonusPower")
                        {
                            crew.bonusPower = boost::lexical_cast<int>(val);
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
                        if (str == "healSpeed")
                        {
                            crew.healSpeed = boost::lexical_cast<float>(val);
                        }
                        if (str == "powerDrain")
                        {
                            crew.powerDrain = boost::lexical_cast<int>(val);
                        }
                        if (str == "powerDrainFriendly")
                        {
                            crew.powerDrainFriendly = EventsParser::ParseBoolean(val);
                        }
                        if (str == "deathSounds")
                        {
                            for (auto deathSoundNode = stat->first_node(); deathSoundNode; deathSoundNode = deathSoundNode->next_sibling())
                            {
                                if (strcmp(deathSoundNode->name(), "deathSound") == 0)
                                {
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
                        if (str == "repairSounds")
                        {
                            crew.repairSounds.clear();

                            for (auto repairSoundNode = stat->first_node(); repairSoundNode; repairSoundNode = repairSoundNode->next_sibling())
                            {
                                if (strcmp(repairSoundNode->name(), "repairSound") == 0)
                                {
                                    crew.repairSounds.push_back(std::string(repairSoundNode->value()));
                                }
                            }

                            if (stat->first_attribute("animSoundFrame"))
                            {
                                crew.repairSoundFrame = boost::lexical_cast<int>(stat->first_attribute("animSoundFrame")->value());
                            }
                        }
                        if (str == "passiveStatBoosts")
                        {
                            for (auto statBoostNode = stat->first_node(); statBoostNode; statBoostNode = statBoostNode->next_sibling())
                            {
                                crew.passiveStatBoosts.push_back(ParseStatBoostNode(statBoostNode));
                            }
                        }
                        if (str == "animBase")
                        {
                            crew.animBase = val;
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
                        if (str == "truePassiveHealAmount")
                        {
                            crew.truePassiveHealAmount = boost::lexical_cast<float>(val);
                        }
                        if (str == "healAmount")
                        {
                            crew.healAmount = boost::lexical_cast<float>(val);
                        }
                        if (str == "trueHealAmount")
                        {
                            crew.trueHealAmount = boost::lexical_cast<float>(val);
                        }
                        if (str == "passiveHealDelay")
                        {
                            crew.passiveHealDelay = boost::lexical_cast<int>(val);
                        }
                        if (str == "detectsLifeforms")
                        {
                            crew.detectsLifeforms = EventsParser::ParseBoolean(val);
                        }
                        if (str == "healCrewAmount")
                        {
                            crew.healCrewAmount = boost::lexical_cast<float>(val);
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
                        if (str == "hasCustomDeathAnimation")
                        {
                            crew.hasCustomDeathAnimation = EventsParser::ParseBoolean(val);
                        }
                        if (str == "sabotageSpeedMultiplier")
                        {
                            crew.sabotageSpeedMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "defaultSkillLevel")
                        {
                            crew.defaultSkillLevel = boost::lexical_cast<int>(val);
                        }
                        if (str == "cloneLoseSkills")
                        {
                            crew.cloneLoseSkills = EventsParser::ParseBoolean(val);
                        }
                        if (str == "allDamageTakenMultiplier")
                        {
                            crew.allDamageTakenMultiplier = boost::lexical_cast<float>(val);
                        }
                        if (str == "damageEnemiesAmount")
                        {
                            crew.damageEnemiesAmount = boost::lexical_cast<float>(val);
                        }
                        if (str == "nameRace")
                        {
                            crew.nameRace.push_back(stat->value());
                            if (stat->first_attribute("transformName"))
                            {
                                crew.transformName.push_back(stat->first_attribute("transformName")->value());
                            }
                            if (stat->first_attribute("changeIfSame"))
                            {
                                crew.changeIfSame = EventsParser::ParseBoolean(stat->first_attribute("changeIfSame")->value());
                            }
                        }
                        if (str == "droneAI")
                        {
                            crew.droneAI.hasCustomAI = true;
                            for (auto effectNode = stat->first_node(); effectNode; effectNode = effectNode->next_sibling())
                            {
                                std::string effectName = std::string(effectNode->name());

                                if (effectName == "fightAI")
                                {
                                    crew.droneAI.fightAI = true;
                                }
                                if (effectName == "repairAI")
                                {
                                    crew.droneAI.repairAI = true;
                                }
                                if (effectName == "manAI")
                                {
                                    crew.droneAI.manAI = true;
                                }
                                if (effectName == "batteryAI")
                                {
                                    crew.droneAI.batteryAI = true;
                                }
                                if (effectName == "returnToDroneRoom")
                                {
                                    crew.droneAI.returnToDroneRoom = true;
                                }
                            }
                        }
                        if (str == "droneMoveFromManningSlot")
                        {
                            crew.droneMoveFromManningSlot = EventsParser::ParseBoolean(val);
                        }
                        if (str == "deathEffect")
                        {
                            crew.hasDeathExplosion = true;
                            ParseDeathEffect(stat, &crew.explosionShipFriendlyFire, &crew.explosionDef);
                        }
                        if (str == "powerEffect")
                        {
                            ParseAbilityEffect(stat, &crew.powerDef);
                        }
                    }
                }
                catch (boost::bad_lexical_cast const &e)
                {
                    MessageBoxA(GetDesktopWindow(), e.what(), "Error", MB_ICONERROR | MB_SETFOREGROUND);
                }

                AddCrewDefinition(crew);
            }
        }
    }
    catch (std::exception)
    {
        MessageBoxA(GetDesktopWindow(), "Error parsing <crew> in hyperspace.xml", "Error", MB_ICONERROR | MB_SETFOREGROUND);
    }

}

void CustomCrewManager::ParseDeathEffect(rapidxml::xml_node<char>* stat, bool* friendlyFire, Damage* explosionDef)
{
    Damage def;
    if (explosionDef != nullptr)
    {
        def = *explosionDef;
    }
    for (auto effectNode = stat->first_node(); effectNode; effectNode = effectNode->next_sibling())
    {
        std::string effectName = std::string(effectNode->name());

        if (effectName == "damage")
        {
            def.iDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "fireChance")
        {
            def.fireChance = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "breachChance")
        {
            def.breachChance = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "ion")
        {
            def.iIonDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "sysDamage")
        {
            def.iSystemDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "persDamage")
        {
            def.iPersDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "hullBust")
        {
            def.bHullBuster = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "lockdown")
        {
            def.bLockdown = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "friendlyFire")
        {
            def.bFriendlyFire = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "stun")
        {
            def.iStun = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "shipFriendlyFire")
        {
            *friendlyFire = EventsParser::ParseBoolean(effectNode->value());
        }
    }
    if (!explosionDef)
    {
        explosionDef = new Damage();
    }
    *explosionDef = def;
}

void CustomCrewManager::ParseAbilityEffect(rapidxml::xml_node<char>* stat, ActivatedPowerDefinition* powerDef)
{
    ActivatedPowerDefinition def;
    if (powerDef != nullptr)
    {
        def = *powerDef;
    }
    def.hasSpecialPower = true;
    for (auto effectNode = stat->first_node(); effectNode; effectNode = effectNode->next_sibling())
    {
        std::string effectName = std::string(effectNode->name());

        if (effectName == "powerSounds")
        {
            for (auto soundNode = effectNode->first_node(); soundNode; soundNode = soundNode->next_sibling())
            {
                if (strcmp(soundNode->name(), "powerSound") == 0)
                {
                    def.sounds.push_back(std::string(soundNode->value()));
                }
            }
        }
        if (effectName == "req")
        {
            auto reqDef = ActivatedPowerRequirements();

            for (auto reqNode = effectNode->first_node(); reqNode; reqNode = reqNode->next_sibling())
            {
                std::string req = reqNode->name();
                std::string reqVal = reqNode->value();

                if (req == "enemyShip")
                {
                    reqDef.enemyShip = true;
                }
                if (req == "playerShip")
                {
                    reqDef.playerShip = true;
                }
                if (req == "friendlyInRoom")
                {
                    reqDef.friendlyInRoom = true;
                }
                if (req == "enemyInRoom")
                {
                    reqDef.enemyInRoom = true;
                }
                if (req == "systemInRoom")
                {
                    reqDef.systemInRoom = true;
                    if (reqNode->first_attribute("damaged"))
                    {
                        reqDef.systemDamaged = EventsParser::ParseBoolean(reqNode->first_attribute("damaged")->value());
                    }
                }
                if (req == "hasClonebay")
                {
                    reqDef.hasClonebay = true;
                }
                if (req == "aiDisabled")
                {
                    reqDef.aiDisabled = true;
                }
                if (req == "outOfCombat")
                {
                    reqDef.outOfCombat = true;
                }
                if (req == "inCombat")
                {
                    reqDef.inCombat = true;
                }
                if (req == "minHealth")
                {
                    reqDef.minHealth = boost::lexical_cast<int>(reqVal);
                }
                if (req == "maxHealth")
                {
                    reqDef.maxHealth = boost::lexical_cast<int>(reqVal);
                }
            }

            if (effectNode->first_attribute("type"))
            {
                if (strcmp(effectNode->first_attribute("type")->value(), "player") == 0)
                {
                    def.playerReq = reqDef;
                }
                else if (strcmp(effectNode->first_attribute("type")->value(), "enemy") == 0)
                {
                    def.enemyReq = reqDef;
                }
                else
                {
                    def.enemyReq = reqDef;
                    def.playerReq = reqDef;
                }
            }
            else
            {
                def.enemyReq = reqDef;
                def.playerReq = reqDef;
            }

        }
        if (effectName == "jumpCooldown")
        {
            std::string v = effectNode->value();

            if (v == "full") def.jumpCooldown = ActivatedPowerDefinition::JUMP_COOLDOWN_FULL;
            else if (v == "reset") def.jumpCooldown = ActivatedPowerDefinition::JUMP_COOLDOWN_RESET;
            else if (v == "continue") def.jumpCooldown = ActivatedPowerDefinition::JUMP_COOLDOWN_CONTINUE;
        }
        if (effectName == "damage")
        {
            def.damage.iDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "fireChance")
        {
            def.damage.fireChance = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "breachChance")
        {
            def.damage.breachChance = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "ion")
        {
            def.damage.iIonDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "sysDamage")
        {
            def.damage.iSystemDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "persDamage")
        {
            def.damage.iPersDamage = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "hullBust")
        {
            def.damage.bHullBuster = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "lockdown")
        {
            def.damage.bLockdown = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "friendlyFire")
        {
            def.damage.bFriendlyFire = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "stun")
        {
            def.damage.iStun = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "cooldown")
        {
            def.cooldown = boost::lexical_cast<float>(effectNode->value());
        }
        if (effectName == "shipFriendlyFire")
        {
            def.shipFriendlyFire = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "win")
        {
            def.win = EventsParser::ParseBoolean(effectNode->value());
        }
        if (effectName == "animFrame")
        {
            def.animFrame = boost::lexical_cast<int>(effectNode->value());
        }
        if (effectName == "buttonText")
        {
            if (effectNode->first_attribute("id"))
            {
                def.buttonLabel.data = effectNode->first_attribute("id")->value();
                def.buttonLabel.isLiteral = false;
            }
            else
            {
                def.buttonLabel.data = effectNode->value();
                def.buttonLabel.isLiteral = true;
            }
        }
        if (effectName == "tooltip")
        {
            if (effectNode->first_attribute("id"))
            {
                def.tooltip.data = effectNode->first_attribute("id")->value();
                def.tooltip.isLiteral = false;
            }
            else
            {
                def.tooltip.data = effectNode->value();
                def.tooltip.isLiteral = true;
            }
        }
        if (effectName == "cooldownColor")
        {
            ParseColorNode(def.cooldownColor, effectNode);
        }
        if (effectName == "effectAnim")
        {
            def.effectAnim = effectNode->value();
        }
        if (effectName == "crewHealth")
        {
            def.crewHealth = boost::lexical_cast<float>(effectNode->value());
        }
        if (effectName == "enemyHealth")
        {
            def.enemyHealth = boost::lexical_cast<float>(effectNode->value());
        }
        if (effectName == "selfHealth")
        {
            def.selfHealth = boost::lexical_cast<float>(effectNode->value());
        }
        if (effectName == "activateWhenReady")
        {
            def.activateWhenReady = EventsParser::ParseBoolean(effectNode->value());

            if (effectNode->first_attribute("enemiesOnly"))
            {
                def.activateReadyEnemies = EventsParser::ParseBoolean(effectNode->first_attribute("enemiesOnly")->value());
            }
        }
        if (effectName == "transformRace")
        {
            def.transformRace = effectNode->value();
        }
        if (effectName == "temporaryEffect")
        {
            def.hasTemporaryPower = true;

            for (auto tempEffectNode = effectNode->first_node(); tempEffectNode; tempEffectNode = tempEffectNode->next_sibling())
            {
                std::string tempEffectName = std::string(tempEffectNode->name());

                if (tempEffectName == "duration")
                {
                    def.tempPower.duration = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "cooldownColor")
                {
                    ParseColorNode(def.tempPower.cooldownColor, tempEffectNode);
                }
                if (tempEffectName == "finishSounds")
                {
                    for (auto soundNode = tempEffectNode->first_node(); soundNode; soundNode = soundNode->next_sibling())
                    {
                        if (strcmp(soundNode->name(), "finishSound") == 0)
                        {
                            def.tempPower.sounds.push_back(std::string(soundNode->value()));
                        }
                    }
                }
                if (tempEffectName == "stunMultiplier")
                {
                    def.tempPower.stunMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "moveSpeedMultiplier")
                {
                    def.tempPower.moveSpeedMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "repairSpeed")
                {
                    def.tempPower.repairSpeed = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "damageMultiplier")
                {
                    def.tempPower.damageMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "bonusPower")
                {
                    def.tempPower.bonusPower = boost::lexical_cast<int>(tempEffectNode->value());
                }
                if (tempEffectName == "animSheet")
                {
                    def.tempPower.animSheet = tempEffectNode->value();
                    if (tempEffectNode->first_attribute("baseVisible"))
                    {
                        def.tempPower.baseVisible = EventsParser::ParseBoolean(tempEffectNode->first_attribute("baseVisible")->value());
                    }
                }
                if (tempEffectName == "effectAnim")
                {
                    def.tempPower.effectAnim = tempEffectNode->value();
                }
                if (tempEffectName == "invulnerable")
                {
                    def.tempPower.invulnerable = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "controllable")
                {
                    def.tempPower.controllable = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "canFight")
                {
                    def.tempPower.canFight = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "canRepair")
                {
                    def.tempPower.canRepair = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "canSabotage")
                {
                    def.tempPower.canSabotage = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "canMan")
                {
                    def.tempPower.canMan = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "canSuffocate")
                {
                    def.tempPower.canSuffocate = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "suffocationModifier")
                {
                    def.tempPower.suffocationModifier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "oxygenChangeSpeed")
                {
                    def.tempPower.oxygenChangeSpeed = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "canPhaseThroughDoors")
                {
                    def.tempPower.canPhaseThroughDoors = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "detectsLifeforms")
                {
                    def.tempPower.detectsLifeforms = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "isTelepathic")
                {
                    def.tempPower.isTelepathic = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "fireDamageMultiplier")
                {
                    def.tempPower.fireDamageMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "damageTakenMultiplier")
                {
                    def.tempPower.damageTakenMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "allDamageTakenMultiplier")
                {
                    def.tempPower.allDamageTakenMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "sabotageSpeedMultiplier")
                {
                    def.tempPower.sabotageSpeedMultiplier = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "passiveHealAmount")
                {
                    def.tempPower.passiveHealAmount = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "healAmount")
                {
                    def.tempPower.healAmount = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "truePassiveHealAmount")
                {
                    def.tempPower.truePassiveHealAmount = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "trueHealAmount")
                {
                    def.tempPower.trueHealAmount = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "damageEnemiesAmount")
                {
                    def.tempPower.damageEnemiesAmount = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "animFrame")
                {
                    def.tempPower.animFrame = boost::lexical_cast<int>(tempEffectNode->value());
                }
                if (tempEffectName == "healCrewAmount")
                {
                    def.tempPower.healCrewAmount = boost::lexical_cast<float>(tempEffectNode->value());
                }
                if (tempEffectName == "effectFinishAnim")
                {
                    def.tempPower.effectFinishAnim = tempEffectNode->value();
                }
                if (tempEffectName == "powerDrain")
                {
                    def.tempPower.powerDrain = boost::lexical_cast<int>(tempEffectNode->value());
                }
                if (tempEffectName == "powerDrainFriendly")
                {
                    def.tempPower.powerDrainFriendly = EventsParser::ParseBoolean(tempEffectNode->value());
                }
                if (tempEffectName == "statBoosts")
                {
                    for (auto statBoostNode = tempEffectNode->first_node(); statBoostNode; statBoostNode = statBoostNode->next_sibling())
                    {
                        def.tempPower.statBoosts.push_back(ParseStatBoostNode(statBoostNode));
                    }
                }
            }
        }
    }
    if (!powerDef)
    {
        powerDef = new ActivatedPowerDefinition();
    }
    *powerDef = def;
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
    return blueprintNames.find(race) != blueprintNames.end();
}

static bool loadingGame = false;

HOOK_METHOD(WorldManager, LoadGame, (const std::string file) -> void)
{
    loadingGame = true;
    super(file);
    loadingGame = false;
}

PowerReadyState CrewMember_Extend::PowerReady()
{
    if (loadingGame) return POWER_NOT_READY_COOLDOWN;
    ActivatedPowerRequirements req;

    if (orig->iShipId == 0)
    {
        req = CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef.playerReq;
    }
    else
    {
        req = CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef.enemyReq;
    }

    ShipManager *currentShip = G_->GetShipManager(orig->currentShipId);
    ShipManager *crewShip = G_->GetShipManager(orig->iShipId);

    if (orig->crewAnim->status == 6)
    {
        if (CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef.transformRace != "")
        {
            return POWER_NOT_READY_TELEPORTING;
        }
    }
    if (temporaryPowerActive)
    {
        return POWER_NOT_READY_ACTIVATED;
    }
    if (powerCooldown.first < powerCooldown.second)
    {
        return POWER_NOT_READY_COOLDOWN;
    }
    if (!orig->intruder && req.enemyShip)
    {
        return POWER_NOT_READY_ENEMY_SHIP;
    }
    if (orig->intruder && req.playerShip)
    {
        return POWER_NOT_READY_PLAYER_SHIP;
    }
    if (currentShip && !currentShip->GetSystemInRoom(orig->iRoomId) && req.systemInRoom)
    {
        return POWER_NOT_READY_SYSTEM_IN_ROOM;
    }
    else if (currentShip && req.systemDamaged)
    {
        auto sys = currentShip->GetSystemInRoom(orig->iRoomId);

        if (sys && sys->healthState.first == sys->healthState.second) return POWER_NOT_READY_SYSTEM_DAMAGED;
    }
    if ((req.enemyInRoom || req.friendlyInRoom) && currentShip)
    {
        bool enemyInRoom = false;
        bool friendlyInRoom = false;

        for (auto i : currentShip->vCrewList)
        {
            if (i->iRoomId == orig->iRoomId)
            {
                enemyInRoom = enemyInRoom || (i->iShipId != orig->iShipId);
                friendlyInRoom = friendlyInRoom || (i->iShipId == orig->iShipId && i != orig);
            }
        }

        if (!enemyInRoom && req.enemyInRoom)
        {
            return POWER_NOT_READY_ENEMY_IN_ROOM;
        }
        else if (!friendlyInRoom && req.friendlyInRoom)
        {
            return POWER_NOT_READY_FRIENDLY_IN_ROOM;
        }
    }
    if ((!crewShip || !crewShip->HasSystem(13)) && req.hasClonebay)
    {
        return POWER_NOT_READY_HAS_CLONEBAY;
    }
    if (req.aiDisabled && orig->iShipId == 1)
    {
        return POWER_NOT_READY_AI_DISABLED;
    }
    if (req.outOfCombat && (crewShip && crewShip->current_target && crewShip->current_target->_targetable.hostile))
    {
        return POWER_NOT_READY_OUT_OF_COMBAT;
    }
    if (req.inCombat && (crewShip && (!crewShip->current_target || !crewShip->current_target->_targetable.hostile)))
    {
        return POWER_NOT_READY_IN_COMBAT;
    }
    if (req.minHealth.enabled && orig->health.first < req.minHealth.value)
    {
        return POWER_NOT_READY_MIN_HEALTH;
    }
    if (req.maxHealth.enabled && orig->health.first > req.maxHealth.value)
    {
        return POWER_NOT_READY_MAX_HEALTH;
    }

    return POWER_READY;
}

Damage* CrewMember_Extend::GetPowerDamage()
{
    auto custom = CustomCrewManager::GetInstance();

    Damage* damage = new Damage;


    Damage* customDamage = &(custom->GetDefinition(orig->species).powerDef.damage);

    damage->iDamage = customDamage->iDamage;
    damage->fireChance = customDamage->fireChance;
    damage->breachChance = customDamage->breachChance;
    damage->stunChance = customDamage->stunChance;
    damage->iIonDamage = customDamage->iIonDamage;
    damage->iSystemDamage = customDamage->iSystemDamage;
    damage->iPersDamage = customDamage->iPersDamage;
    damage->bHullBuster = customDamage->bHullBuster;
    damage->ownerId = orig->iShipId;
    damage->selfId = (int)orig;
    damage->bLockdown = false;
    damage->crystalShard = customDamage->crystalShard;
    damage->bFriendlyFire = customDamage->bFriendlyFire;
    damage->iStun = customDamage->iStun;

    return damage;
}

void CrewMember_Extend::ActivateTemporaryPower()
{
    ActivatedPowerDefinition powerDef = CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef;

    auto aex = CMA_EX(orig->crewAnim);

    aex->temporaryPowerActive = true;

    if (hasTemporaryPower)
    {
        temporaryPowerActive = true;
        temporaryPowerDuration.first = powerDef.tempPower.duration;
    }

    if (powerDef.tempPower.canPhaseThroughDoors.enabled)
    {
        canPhaseThroughDoors = powerDef.tempPower.canPhaseThroughDoors.value;
    }
}

void CrewMember_Extend::PreparePower()
{
    ActivatedPowerDefinition powerDef = CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef;

    powerShip = orig->currentShipId;
    powerRoom = orig->iRoomId;

    if (powerDef.animFrame == -1)
    {
        ActivatePower();
    }
    if (powerDef.tempPower.animFrame == -1)
    {
        ActivateTemporaryPower();
    }

    auto aex = CMA_EX(orig->crewAnim);

    if (aex->effectAnim != nullptr)
    {
        aex->effectAnim->SetCurrentFrame(0);
        aex->effectAnim->tracker.SetLoop(false, -1);
        aex->effectAnim->Start(true);
    }

    if (aex->tempEffectAnim != nullptr)
    {
        aex->tempEffectAnim->SetCurrentFrame(0);
        aex->tempEffectAnim->tracker.SetLoop(true, 0);
        aex->tempEffectAnim->Start(true);
    }

    if (powerDef.sounds.size() > 0)
    {
        int rng = random32();

        std::string sound = powerDef.sounds[rng % powerDef.sounds.size()];

        G_->GetSoundControl()->PlaySoundMix(sound, -1.f, false);
    }

    if (aex->effectAnim != nullptr)
    {
        aex->effectPos = Pointf(orig->x - std::ceil((float)aex->effectAnim->info.frameWidth / 2), orig->y - std::ceil((float)aex->effectAnim->info.frameHeight / 2) + orig->PositionShift());
    }
    aex->effectWorldPos = Pointf(orig->x, orig->y);
    aex->powerDone = false;
}

void CrewMember_Extend::ActivatePower()
{
    if (!CustomCrewManager::GetInstance()->IsRace(orig->species))
    {
        return;
    }

    ActivatedPowerDefinition powerDef = CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef;

    ShipManager *ship;

    if (powerShip == 0)
    {
        ship = G_->GetWorld()->playerShip->shipManager;
    }
    else
    {
        ship = G_->GetWorld()->playerShip->enemyShip->shipManager;
    }

    powerCooldown.first = 0;

    powerActivated = true;

    if (powerDef.damage.bLockdown)
    {
        orig->ship->LockdownRoom(orig->iRoomId, Pointf(orig->x, orig->y));
    }

    if (powerDef.win)
    {
        G_->GetSoundControl()->StopPlaylist(100);
        G_->GetSoundControl()->PlaySoundMix("victory", -1.f, false);

        G_->GetScoreKeeper()->SetVictory(true);
        G_->GetCApp()->gui->gameover = true;
        G_->GetCApp()->gui->Victory();
    }

    if (powerDef.crewHealth || powerDef.enemyHealth)
    {
        for (auto i : ship->vCrewList)
        {
            if (i->iRoomId == powerRoom && i != orig)
            {
                if (i->iShipId == orig->iShipId)
                {
                    i->DirectModifyHealth(powerDef.crewHealth);
                }
                else
                {
                    i->DirectModifyHealth(powerDef.enemyHealth);
                }
            }
        }
    }

    if (orig->iRoomId == powerRoom && powerDef.selfHealth)
    {
        orig->DirectModifyHealth(powerDef.selfHealth);
    }

    auto aex = CMA_EX(orig->crewAnim);
    aex->powerDone = true;

    if (!powerDef.transformRace.empty() && orig->crewAnim->status != 3)
    {
        std::string species = powerDef.transformRace;

        auto newBlueprint = G_->GetBlueprints()->GetCrewBlueprint(species);

        orig->blueprint.powers = newBlueprint->powers;
        orig->blueprint.name = newBlueprint->name;
        orig->blueprint.desc = newBlueprint->desc;
        orig->blueprint.type = newBlueprint->type;
        orig->species = species;

        delete newBlueprint;

        auto newCrewAnim = new CrewAnimation(orig->iShipId, orig->species, Pointf(0, 0), orig->iShipId == 1);

        orig->crewAnim->anims = newCrewAnim->anims;
        orig->crewAnim->baseStrip = newCrewAnim->baseStrip;
        orig->crewAnim->colorStrip = newCrewAnim->colorStrip;
        orig->crewAnim->bDrone = newCrewAnim->bDrone;
        orig->crewAnim->bGhost = newCrewAnim->bGhost;
        orig->crewAnim->race = newCrewAnim->race;

        delete newCrewAnim;

        Initialize(orig->blueprint, orig->iShipId, orig->iShipId == 1, orig->crewAnim);

        if (orig->iShipId == 0)
        {
            G_->GetCApp()->gui->crewControl.ClearCrewBoxes();
            G_->GetCApp()->gui->crewControl.UpdateCrewBoxes();
        }
    }
}

void CrewMember_Extend::TemporaryPowerFinished()
{
    if (!CustomCrewManager::GetInstance()->IsRace(orig->species))
    {
        return;
    }

    ActivatedPowerDefinition powerDef = CustomCrewManager::GetInstance()->GetDefinition(orig->species).powerDef;

    temporaryPowerActive = false;

    if (powerDef.tempPower.sounds.size() > 0)
    {
        int rng = random32();

        std::string sound = powerDef.tempPower.sounds[rng % powerDef.tempPower.sounds.size()];

        G_->GetSoundControl()->PlaySoundMix(sound, -1, false);
    }

    auto aex = CMA_EX(orig->crewAnim);

    if (aex->tempEffectAnim != nullptr)
    {
        aex->tempEffectAnim->tracker.Stop(false);
    }

    aex->temporaryPowerActive = false;

    canPhaseThroughDoors = CustomCrewManager::GetInstance()->GetDefinition(orig->species).canPhaseThroughDoors;

    if (aex->effectFinishAnim != nullptr)
    {
        aex->effectFinishAnim->SetCurrentFrame(0);
        aex->effectFinishAnim->tracker.SetLoop(false, -1);
        aex->effectFinishAnim->Start(true);
    }

}

void CrewAnimation_Extend::OnInit(const std::string& name, Pointf position, bool enemy)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();


    if (custom->IsRace(orig->race))
    {
        auto def = custom->GetDefinition(orig->race);

        if (!def.powerDef.effectAnim.empty())
        {
            Animation *anim = new Animation();

            AnimationControl::GetAnimation(*anim, G_->GetAnimationControl(), def.powerDef.effectAnim);

            effectAnim = anim;
        }

        if (!def.powerDef.tempPower.effectFinishAnim.empty())
        {
            Animation *anim = new Animation();

            AnimationControl::GetAnimation(*anim, G_->GetAnimationControl(), def.powerDef.tempPower.effectFinishAnim);

            effectFinishAnim = anim;
        }


        if (!def.powerDef.tempPower.effectAnim.empty())
        {
            Animation *anim = new Animation();

            AnimationControl::GetAnimation(*anim, G_->GetAnimationControl(), def.powerDef.tempPower.effectAnim);

            tempEffectAnim = anim;
        }

        if (!def.powerDef.tempPower.animSheet.empty())
        {
            tempEffectStrip = G_->GetResources()->GetImageId("people/" + def.powerDef.tempPower.animSheet + ".png");
        }
    }
}



//=================================

static bool blockAddSoundQueue = false;

HOOK_METHOD(Animation, AddSoundQueue, (int frame, const std::string& str) -> void)
{
    if (blockAddSoundQueue) return;

    super(frame, str);
}

HOOK_METHOD_PRIORITY(CrewMember, constructor, -899, (CrewBlueprint& bp, int shipId, bool enemy, CrewAnimation *animation) -> void)
{

    super(bp, shipId, enemy, animation);

    CrewMember_Extend* ex = CM_EX(this);
    ex->Initialize(bp, shipId, enemy, animation);
}


void CrewMember_Extend::Initialize(CrewBlueprint& bp, int shipId, bool enemy, CrewAnimation *animation)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (custom->IsRace(orig->species))
    {
        auto def = custom->GetDefinition(orig->species);

        if (def.animBase == "rock")
        {
            std::vector<GL_Color> layerColors = std::vector<GL_Color>();
            std::vector<GL_Texture*> layerStrips = std::vector<GL_Texture*>();

            bool replaceLayers = false;

            if (animation)
            {
                layerColors = animation->layerColors;
                layerStrips = animation->layerStrips;
                replaceLayers = true;
            }

            delete animation;

            blockAddSoundQueue = true;
            orig->crewAnim = new RockAnimation(bp.name, shipId, Pointf(0, 0), enemy);
            blockAddSoundQueue = false;

            if (replaceLayers)
            {
                orig->crewAnim->layerColors = layerColors;
                orig->crewAnim->layerStrips = layerStrips;
            }

            if (def.repairSoundFrame != -1 && def.repairSounds.size() > 0 && !enemy)
            {
                for (auto sound : def.repairSounds)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        orig->crewAnim->anims[i][2].AddSoundQueue(def.repairSoundFrame, sound);
                    }
                }
            }
        }
        if (def.animBase == "mantis")
        {
            std::vector<GL_Color> layerColors = std::vector<GL_Color>();
            std::vector<GL_Texture*> layerStrips = std::vector<GL_Texture*>();

            bool replaceLayers = false;

            if (animation)
            {
                layerColors = animation->layerColors;
                layerStrips = animation->layerStrips;
                replaceLayers = true;
            }

            delete animation;

            orig->crewAnim = new MantisAnimation;
            orig->crewAnim->constructor(shipId, bp.name, Pointf(0, 0), enemy);
            CMA_EX(orig->crewAnim)->isMantisAnimation = true;

            if (replaceLayers)
            {
                orig->crewAnim->layerColors = layerColors;
                orig->crewAnim->layerStrips = layerStrips;
            }
        }
        float passiveHealAmount = CalculateStat(CrewStat::PASSIVE_HEAL_AMOUNT, def);
        float truePassiveHealAmount = CalculateStat(CrewStat::TRUE_PASSIVE_HEAL_AMOUNT, def);
        if (passiveHealAmount != 0.f || truePassiveHealAmount != 0.f)
        {
            float passiveHealDelay = CalculateStat(CrewStat::PASSIVE_HEAL_DELAY, def);
            if (passiveHealDelay > 0)
            {
                passiveHealTimer = new TimerHelper();
                passiveHealTimer->Start(passiveHealDelay);
            }
        }

        powerCooldown.first = 0.f;
        powerCooldown.second = def.powerDef.cooldown;
        temporaryPowerDuration.second = def.powerDef.tempPower.duration;
        temporaryPowerDuration.first = temporaryPowerDuration.second;
        hasSpecialPower = def.powerDef.hasSpecialPower;
        hasTemporaryPower = def.powerDef.hasTemporaryPower;
        canPhaseThroughDoors = def.canPhaseThroughDoors;

        for (auto statBoostDef : def.passiveStatBoosts)
        {
            StatBoost statBoost = StatBoost(statBoostDef);
            statBoost.GiveId();

            statBoost.crewSource = orig;
            statBoost.boostSource = StatBoost::BoostSource::CREW;
            outgoingStatBoosts.push_back(statBoost);
        }
        for (auto statBoostDef : def.powerDef.tempPower.statBoosts)
        {
            StatBoost statBoost = StatBoost(statBoostDef);
            statBoost.GiveId();

            statBoost.crewSource = orig;
            statBoost.boostSource = StatBoost::BoostSource::CREW;
            outgoingAbilityStatBoosts.push_back(statBoost);
        }


        auto skillsDef = def.skillsDef;

        std::string skillOrder[6] = {"piloting", "engines", "shields", "weapons", "repair", "combat"};

        for (int i = 0; i < orig->blueprint.skillLevel.size(); i++)
        {
            orig->blueprint.skillLevel[i].first = ((float)orig->blueprint.skillLevel[i].first / orig->blueprint.skillLevel[i].second) * (skillsDef.skills[skillOrder[i]].requirement * 2);
            orig->blueprint.skillLevel[i].second = skillsDef.skills[skillOrder[i]].requirement * 2;
            int defaultSkillLevel = CalculateStat(CrewStat::DEFAULT_SKILL_LEVEL, def);

            if (orig->blueprint.skillLevel[i].first < (defaultSkillLevel / 2.f) * orig->blueprint.skillLevel[i].second)
            {
                orig->blueprint.skillLevel[i].first = ((defaultSkillLevel / 2.f) * orig->blueprint.skillLevel[i].second);
            }
        }
    }
}

HOOK_METHOD(CrewBlueprint, RandomSkills, (int worldLevel) -> void)
{
    auto custom = CustomCrewManager::GetInstance();
    super(worldLevel);


    if (custom->IsRace(name))
    {
        auto skillsDef = custom->GetDefinition(name).skillsDef;

        std::string skillOrder[6] = {"piloting", "engines", "shields", "weapons", "repair", "combat"};

        for (int i = 0; i < skillLevel.size(); i++)
        {

            skillLevel[i].first = ((float)skillLevel[i].first / skillLevel[i].second) * (skillsDef.skills[skillOrder[i]].requirement * 2);
            skillLevel[i].second = skillsDef.skills[skillOrder[i]].requirement * 2;
        }
    }
}

HOOK_METHOD_PRIORITY(CrewMember, UpdateHealth, 2000, () -> void)
{
    if (G_->GetCApp()->menu.shipBuilder.bOpen) return;

    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto ex = CM_EX(this);

//    using std::chrono::steady_clock;
//    using std::chrono::duration_cast;
//    using std::chrono::duration;
//    using std::chrono::milliseconds;
//    auto t1 = steady_clock::now();

    auto def = custom->GetDefinition(this->species);

//    auto t2 = steady_clock::now();
//    duration<double, std::nano> ms_double = t2 - t1;
//    std::cout << "Definition time: " << ms_double.count();

    if (iOnFire && CanBurn())
    {
        float fireMultiplier = 1.f;

        if (custom->IsRace(species))
        {
            fireMultiplier = ex->CalculateStat(CrewStat::FIRE_DAMAGE_MULTIPLIER, def);
        }

        DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * (iOnFire * -0.133f * fireMultiplier));
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


    if (custom->IsRace(species))
    {
        mod = ex->CalculateStat(CrewStat::HEAL_SPEED_MULTIPLIER, def);
    }

    DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * fMedbay * mod * 0.4f);

    if (custom->IsRace(species))
    {
        float passiveHealAmount = ex->CalculateStat(CrewStat::PASSIVE_HEAL_AMOUNT, def);
        float healAmount = ex->CalculateStat(CrewStat::ACTIVE_HEAL_AMOUNT, def);
        float trueHealAmount = ex->CalculateStat(CrewStat::TRUE_HEAL_AMOUNT, def);
        float truePassiveHealAmount = ex->CalculateStat(CrewStat::TRUE_PASSIVE_HEAL_AMOUNT, def);

        if (healAmount != 0.f && Functional())
        {
            if (healAmount > 0.f && health.first != health.second)
            {
                fMedbay += 0.0000000001;
            }
            DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * healAmount * mod * 0.06245f);
        }
        if (ex->isHealing && passiveHealAmount != 0.f && health.first != health.second && Functional())
        {
            if (passiveHealAmount > 0.f && health.first != health.second)
            {
                fMedbay += 0.0000000001;
            }
            DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * passiveHealAmount * mod * 0.06245f);
        }
        if (trueHealAmount != 0.f && Functional())
        {
            if (trueHealAmount > 0.f && health.first != health.second)
            {
                fMedbay += 0.0000000001;
            }
            DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * trueHealAmount * 0.06245f);
        }
        if (ex->isHealing && truePassiveHealAmount != 0.f && health.first != health.second && Functional())
        {
            if (truePassiveHealAmount > 0.f && health.first != health.second)
            {
                fMedbay += 0.0000000001;
            }
            DirectModifyHealth(G_->GetCFPS()->GetSpeedFactor() * truePassiveHealAmount * 0.06245f);
        }
    }

    //super();
}
HOOK_METHOD_PRIORITY(CrewMember, DirectModifyHealth, 1000, (float healthMod) -> void)
{
    auto custom = CustomCrewManager::GetInstance();
    CrewMember_Extend* ex = CM_EX(this);
    auto def = custom->GetDefinition(this->species);

    if (custom->IsRace(species))
    {
        if (healthMod < 0.f)
        {
            if (ex->temporaryPowerActive && def.powerDef.tempPower.invulnerable)
            {
                return;
            }
            else
            {
                healthMod *= ex->CalculateStat(CrewStat::ALL_DAMAGE_TAKEN_MULTIPLIER, def);
            }
        }
    }

    super(healthMod);
    if (custom->IsRace(species) && healthMod < 0.f && ex->passiveHealTimer)
    {
        ex->isHealing = false;
        ex->passiveHealTimer->Start(ex->CalculateStat(CrewStat::PASSIVE_HEAL_DELAY, def));
    }
}
HOOK_METHOD_PRIORITY(CrewMember, OnLoop, 1000, () -> void)
{
    super();

    auto custom = CustomCrewManager::GetInstance();
    CrewMember_Extend* ex = CM_EX(this);
    if (custom->IsRace(species))
    {
        auto def = custom->GetDefinition(species);
        float delay = ex->CalculateStat(CrewStat::PASSIVE_HEAL_DELAY, def);
        if (!ex->passiveHealTimer && delay != 0)
        {
            ex->passiveHealTimer = new TimerHelper();
            ex->passiveHealTimer->Start(delay);
        }
        else if (ex->passiveHealTimer && delay == 0)
        {
            delete ex->passiveHealTimer;
            ex->isHealing = true;
        }
        else if (ex->passiveHealTimer)
        {
            ex->passiveHealTimer->Update();
            if (ex->passiveHealTimer->Done())
            {
                ex->isHealing = true;
            }
        }
        else
        {
            ex->isHealing = true;
        }

        if (fStunTime != 0.f)
        {
            if (fStunTime < ex->prevStun)
            {
                auto def = custom->GetDefinition(species);
                float stunMultiplier = ex->CalculateStat(CrewStat::STUN_MULTIPLIER, def);

                if (stunMultiplier > 0.f)
                {
                    fStunTime = ex->prevStun - ((ex->prevStun - fStunTime) * (1.f / stunMultiplier));
                    if (fStunTime < 0.f)
                    {
                        fStunTime = 0.f;
                    }
                }
                else
                {
                    fStunTime = 0.f;
                }
            }
        }

        ex->prevStun = fStunTime;

        if (ex->hasSpecialPower && !G_->GetCApp()->menu.shipBuilder.bOpen)
        {
            if (ex->temporaryPowerActive)
            {
                ex->temporaryPowerDuration.first = std::max(0.f, ex->temporaryPowerDuration.first - (float)(G_->GetCFPS()->GetSpeedFactor() * 0.0625));

                if (ex->temporaryPowerDuration.first <= 0.f)
                {
                    ex->TemporaryPowerFinished();
                }
            }
            else
            {
                ex->powerCooldown.first = std::min(ex->powerCooldown.second, (float)(G_->GetCFPS()->GetSpeedFactor() * 0.0625) + ex->powerCooldown.first);
                auto def = custom->GetDefinition(species);
                bool activateWhenReady;
                ex->CalculateStat(CrewStat::ACTIVATE_WHEN_READY, def, &activateWhenReady);
                if (activateWhenReady && ex->PowerReady() == POWER_READY)
                {
                    if (iShipId == 1 || !def.powerDef.activateReadyEnemies)
                    {
                        ex->PreparePower();
                    }
                }
            }
        }

        auto aex = CMA_EX(crewAnim);
        if (aex->effectAnim != nullptr)
        {
            aex->effectAnim->Update();
            auto def = custom->GetDefinition(species);

            // Do it this way since ActivatePower() might delete aex if the crewmember transforms into a different race.
            bool activateTemporaryPower = !aex->temporaryPowerActive && def.powerDef.tempPower.animFrame != -1 && aex->effectAnim->tracker.running && aex->effectAnim->currentFrame == def.powerDef.tempPower.animFrame;

            if (!aex->powerDone && def.powerDef.animFrame != -1 && aex->effectAnim->tracker.running && aex->effectAnim->currentFrame == def.powerDef.animFrame)
            {
                ex->ActivatePower();
                aex = CMA_EX(crewAnim);
            }

            if (activateTemporaryPower)
            {
                ex->ActivateTemporaryPower();
            }
        }
        if (aex->tempEffectAnim != nullptr)
        {
            aex->tempEffectAnim->Update();
        }
        if (aex->effectFinishAnim != nullptr)
        {
            aex->effectFinishAnim->Update();
        }
    }
}

HOOK_METHOD(CrewMember, SaveState, (int file) -> void)
{
    auto ex = CM_EX(this);

    FileHelper::writeFloat(file, ex->powerCooldown.first);
    FileHelper::writeFloat(file, ex->powerCooldown.second);
    FileHelper::writeFloat(file, ex->temporaryPowerDuration.first);
    FileHelper::writeFloat(file, ex->temporaryPowerDuration.second);
    FileHelper::writeInt(file, ex->temporaryPowerActive);

    super(file);

}

HOOK_METHOD(CrewMember, LoadState, (int file) -> void)
{
    auto ex = CM_EX(this);
    auto aex = CMA_EX(crewAnim);

    ex->powerCooldown.first = FileHelper::readFloat(file);
    ex->powerCooldown.second = FileHelper::readFloat(file);
    ex->temporaryPowerDuration.first = FileHelper::readFloat(file);
    ex->temporaryPowerDuration.second = FileHelper::readFloat(file);
    ex->temporaryPowerActive = FileHelper::readInteger(file);
    aex->temporaryPowerActive = ex->temporaryPowerActive;

    if (aex->tempEffectAnim != nullptr && aex->temporaryPowerActive)
    {
        aex->tempEffectAnim->SetCurrentFrame(0);
        aex->tempEffectAnim->tracker.SetLoop(true, 0);
        aex->tempEffectAnim->Start(true);
    }

    super(file);
}

HOOK_METHOD_PRIORITY(CrewMember, GetNewGoal, 2000, () -> bool)
{
    auto ex = CM_EX(this);
    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    auto def = CustomCrewManager::GetInstance()->GetDefinition(this->species);
    ex->CalculateStat(CrewStat::CAN_PHASE_THROUGH_DOORS, def, &ex->canPhaseThroughDoors);

    if (!ex->canPhaseThroughDoors) return super();

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

                    int offset = random32() % 17;
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
    auto custom = CustomCrewManager::GetInstance();
    CrewMember *newCrew = custom->CreateCrewMember(bp, shipId, shipId == 1);

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

HOOK_STATIC(CrewAnimation, GetDeathSound, (std::string& strRef, CrewAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.deathSounds.size() > 0)
        {
            int rng = random32();

            strRef.assign(def.deathSounds[rng % def.deathSounds.size()]);

            return strRef;
        }
    }

    return ret;
}

HOOK_STATIC(RockAnimation, GetDeathSound, (std::string& strRef, RockAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.deathSounds.size() > 0)
        {
            int rng = random32();

            strRef.assign(def.deathSounds[rng % def.deathSounds.size()]);

            return strRef;
        }
    }

    return ret;
}

HOOK_STATIC(MantisAnimation, GetDeathSound, (std::string& strRef, MantisAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.deathSounds.size() > 0)
        {
            int rng = random32();
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
            int rng = random32();

            strRef.assign(def.shootingSounds[rng % def.shootingSounds.size()]);

            return strRef;
        }
    }

    return ret;
}

HOOK_STATIC(RockAnimation, GetShootingSound, (std::string& strRef, RockAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.shootingSounds.size() > 0)
        {
            int rng = random32();

            strRef.assign(def.shootingSounds[rng % def.shootingSounds.size()]);

            return strRef;
        }
    }

    return ret;
}

HOOK_STATIC(MantisAnimation, GetShootingSound, (std::string& strRef, MantisAnimation *anim) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, anim);

    if (custom->IsRace(anim->race))
    {
        auto def = custom->GetDefinition(anim->race);

        if (def.shootingSounds.size() > 0)
        {
            int rng = random32();

            strRef.assign(def.shootingSounds[rng % def.shootingSounds.size()]);

            return strRef;
        }
    }

    return ret;
}

HOOK_STATIC(CrewMember, GetUniqueRepairing, (std::string& strRef, CrewMember *crew) -> std::string&)
{
    auto custom = CustomCrewManager::GetInstance();

    std::string& ret = super(strRef, crew);

    if (custom->IsRace(crew->species))
    {
        auto def = custom->GetDefinition(crew->species);

        if (def.repairSounds.size() > 0 && def.repairSoundFrame == -1)
        {
            int rng = random32();

            strRef.assign(def.repairSounds[rng % def.repairSounds.size()]);


            return strRef;
        }
        else
        {
            strRef.assign("");
            return strRef;
        }
    }

    return ret;
}

HOOK_METHOD(OxygenSystem, EmptyOxygen, (int roomId) -> void)
{
    if (!G_->GetCApp()->menu.shipBuilder.bOpen)
        return;

    return super(roomId);
}

HOOK_METHOD(ShipManager, UpdateEnvironment, () -> void)
{
    if (!G_->GetCApp()->menu.shipBuilder.bOpen && systemKey[2] != -1)
    {
        for (auto const &x: vCrewList)
        {
            auto custom = CustomCrewManager::GetInstance();

            if (custom->IsRace(x->species))
            {
                auto def = custom->GetDefinition(x->species);
                auto ex = CM_EX(x);
                float oxygenModifier = ex->CalculateStat(CrewStat::OXYGEN_CHANGE_SPEED, def);

                if (oxygenModifier != 0.f && !x->bDead)
                {
                    if (oxygenSystem->oxygenLevels[x->iRoomId] == 0.f)
                    {
                        oxygenSystem->oxygenLevels[x->iRoomId] = 0.0000001f;
                    }

                    oxygenSystem->ComputeAirLoss(x->iRoomId, -oxygenModifier, true);
                }
            }
        }
    }
    super();
}


HOOK_METHOD(CrewMember, ApplyDamage, (float damage) -> bool)
{
    auto custom = CustomCrewManager::GetInstance();

    if (custom->IsRace(species))
    {
        auto def = custom->GetDefinition(species);
        auto ex = CM_EX(this);
        float damageTakenMultiplier = ex->CalculateStat(CrewStat::DAMAGE_TAKEN_MULTIPLIER, def);

        damage = damage * (damageTakenMultiplier);
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

HOOK_METHOD(CombatControl, CanTargetSelf, () -> bool)
{
    crewDetectLifeforms = true;
    bool ret = super();
    crewDetectLifeforms = false;

    return ret;
}

HOOK_METHOD(CombatControl, UpdateTarget, () -> bool)
{
    crewDetectLifeforms = true;
    return super();
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
                    auto def = custom->GetDefinition(i->species);
                    auto ex = CM_EX(i);

                    if (!i->intruder)
                    {
//                        if (HasAugmentation("ALL_CREW_DETECT_LIFEFORMS"))
//                        {
//                            return GetAugmentationValue("ALL_CREW_DETECT_LIFEFORMS");
//                        }
                        bool detectsLifeforms;
                        ex->CalculateStat(CrewStat::DETECTS_LIFEFORMS, def, &detectsLifeforms);
                        if (detectsLifeforms && i->Functional())
                        {
                            return 1;
                        }
                    }
                }
            }
        }

        return ret;
    }

    return super(name);
}

HOOK_METHOD(ShipSystem, SetPowerLoss, (int powerLoss) -> int)
{
    int ret = super(powerLoss);

    SYS_EX(this)->oldPowerLoss = powerLoss;

    return ret;
}

static bool blockClearStatus = false;

HOOK_METHOD(WorldManager, OnLoop, () -> void)
{
    super();


    if (!HostileEnvironment() && ships.size() == 0)
    {
        playerShip->shipManager->ClearStatusSystem(2);
        playerShip->shipManager->ClearStatusSystem(6);
        playerShip->shipManager->ClearStatusSystem(1);
    }
}

HOOK_METHOD(ShipManager, ClearStatusSystem, (int sys) -> void)
{
    if (blockClearStatus) return;

    super(sys);
}

HOOK_METHOD(ShipSystem, ClearStatus, () -> void)
{
    SYS_EX(this)->oldPowerLoss = 0;
    super();

    //iTempPowerLoss = SYS_EX(this)->additionalPowerLoss;
}

HOOK_METHOD(ShipManager, OnLoop, () -> void)
{
    super();

    for (auto i : vSystemList)
    {
        ShipSystem_Extend* sys_ex = SYS_EX(i);

        sys_ex->additionalPowerLoss = 0;
    }

    auto custom = CustomCrewManager::GetInstance();

    for (auto i : vCrewList)
    {
        if (custom->IsRace(i->species))
        {
            auto def = custom->GetDefinition(i->species);
            CustomCrewManager *custom = CustomCrewManager::GetInstance();

            auto ex = CM_EX(i);
            int powerDrain = ex->CalculateStat(CrewStat::POWER_DRAIN, def);

            ShipSystem* sys = GetSystemInRoom(i->iRoomId);

            if (sys && sys->iSystemType != (int)SystemId::PILOT)
            {
                bool powerDrainFriendly = false;
                ex->CalculateStat(CrewStat::POWER_DRAIN_FRIENDLY, def, &powerDrainFriendly);
                if (i->intruder || powerDrainFriendly)
                {
                    ShipSystem_Extend* sys_ex = SYS_EX(sys);

                    sys_ex->additionalPowerLoss += powerDrain;
                }
            }

            if (def.changeIfSame)
            {
                int counter = 0;
                for (auto name : def.transformName)
                {
                    if (i->blueprint.crewNameLong.GetText() == name && i->crewAnim->status != 3) // this is stupid
                    {
                        std::string newSpecies = def.nameRace.at(counter);
                        ++counter;

                        auto newBlueprint = G_->GetBlueprints()->GetCrewBlueprint(newSpecies);

                        i->blueprint.powers = newBlueprint->powers;
                        i->blueprint.name = newBlueprint->name;
                        i->blueprint.desc = newBlueprint->desc;
                        i->blueprint.type = newBlueprint->type;
                        i->species = newSpecies;

                        delete newBlueprint;

                        auto newCrewAnim = new CrewAnimation(i->iShipId, i->species, Pointf(0, 0), i->iShipId == 1);

                        i->crewAnim->anims = newCrewAnim->anims;
                        i->crewAnim->baseStrip = newCrewAnim->baseStrip;
                        i->crewAnim->colorStrip = newCrewAnim->colorStrip;
                        i->crewAnim->bDrone = newCrewAnim->bDrone;
                        i->crewAnim->bGhost = newCrewAnim->bGhost;
                        i->crewAnim->race = newCrewAnim->race;

                        ex->Initialize(i->blueprint, i->iShipId, i->iShipId == 1, i->crewAnim);

                        if (i->iShipId == 0)
                        {
                            G_->GetCApp()->gui->crewControl.ClearCrewBoxes();
                            G_->GetCApp()->gui->crewControl.UpdateCrewBoxes();
                        }
                    }
                }
            }
            else
            {
                bool change = true;
                for (auto check : def.transformName)
                {
                    if (i->blueprint.crewNameLong.GetText() != check)
                    {
                        change = false;
                    }
                }
                if (change)
                {
                    std::string newSpecies = def.nameRace.at(0);

                    auto newBlueprint = G_->GetBlueprints()->GetCrewBlueprint(newSpecies);

                    i->blueprint.powers = newBlueprint->powers;
                    i->blueprint.name = newBlueprint->name;
                    i->blueprint.desc = newBlueprint->desc;
                    i->blueprint.type = newBlueprint->type;
                    i->species = newSpecies;

                    delete newBlueprint;

                    auto newCrewAnim = new CrewAnimation(i->iShipId, i->species, Pointf(0, 0), i->iShipId == 1);

                    i->crewAnim->anims = newCrewAnim->anims;
                    i->crewAnim->baseStrip = newCrewAnim->baseStrip;
                    i->crewAnim->colorStrip = newCrewAnim->colorStrip;
                    i->crewAnim->bDrone = newCrewAnim->bDrone;
                    i->crewAnim->bGhost = newCrewAnim->bGhost;
                    i->crewAnim->race = newCrewAnim->race;

                    ex->Initialize(i->blueprint, i->iShipId, i->iShipId == 1, i->crewAnim);

                    if (i->iShipId == 0)
                    {
                        G_->GetCApp()->gui->crewControl.ClearCrewBoxes();
                        G_->GetCApp()->gui->crewControl.UpdateCrewBoxes();
                    }
                }
            }
        }
    }

    for (auto i : vSystemList)
    {
        ShipSystem_Extend* sys_ex = SYS_EX(i);

        i->iTempPowerLoss = sys_ex->oldPowerLoss + sys_ex->additionalPowerLoss;
        if (i->iTempPowerLoss >= i->powerState.second)
        {
            i->iTempPowerLoss = i->powerState.second;
        }

        i->CheckMaxPower();
        i->CheckForRepower();
    }
}


HOOK_METHOD(CrewMember, OnLoop, () -> void)
{
    super();

    auto custom = CustomCrewManager::GetInstance();

    if (custom->IsRace(species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);

        if (crewAnim->status == 3)
        {
            if (!ex->exploded && !ex->triggerExplosion)
            {
                ex->triggerExplosion = true;
            }
        }
        else if (!bDead)
        {
            ex->triggerExplosion = false;
            ex->exploded = false;
        }

        /*
        if (!ex->timedStatBoosts.empty())
        {
            int counter = 0;
            for (StatBoost statBoost : ex->timedStatBoosts)
            {
                if (statBoost.timerHelper)
                {
                    statBoost.timerHelper->Update();
                    if (statBoost.timerHelper->Done())
                    {
                        ex->timedStatBoosts.erase(ex->timedStatBoosts.begin() + counter);
                    }
                }
                else
                {
                    if (statBoost.duration != -1)
                    {
                        statBoost.timerHelper = new TimerHelper();
                        statBoost.timerHelper->Start(statBoost.duration);
                    }
                    else
                    {
                        // the "timed" stat boost is actually permanent
                    }
                }
            }
            ++counter;
        }
        */
    }
}

static bool shipFriendlyFire = true;
static bool blockDamageArea = false;

HOOK_STATIC(IonDrone, GetRoomDamage, (Damage *damage, IonDrone* crew) -> Damage*)
{    if (blockDamageArea) return damage;
    return super(damage, crew);
}
HOOK_STATIC(EnergyAlien, GetRoomDamage, (Damage *damage, EnergyAlien* crew) -> Damage*)
{
    if (blockDamageArea) return damage;
    return super(damage, crew);
}

HOOK_STATIC(CrewMember, GetRoomDamage, (Damage *damage, CrewMember *crew) -> Damage*)
{
    Damage *ret = super(damage, crew);

    auto custom = CustomCrewManager::GetInstance();
    if (!blockDamageArea)
    {
        if (custom->IsRace(crew->species))
        {
            auto ex = CM_EX(crew);

            if (ex->triggerExplosion && !ex->exploded)
            {
                ex->exploded = true;

                auto def = custom->GetDefinition(crew->species);
                ex->CalculateStat(CrewStat::DEATH_EFFECT, def);

                if (ex->hasDeathExplosion)
                {
                    Damage *customDamage = &ex->deathEffectChange;

                    damage->iDamage = customDamage->iDamage;
                    damage->fireChance = customDamage->fireChance;
                    damage->breachChance = customDamage->breachChance;
                    damage->stunChance = customDamage->stunChance;
                    damage->iIonDamage = customDamage->iIonDamage;
                    damage->iSystemDamage = customDamage->iSystemDamage;
                    damage->iPersDamage = customDamage->iPersDamage;
                    damage->bHullBuster = customDamage->bHullBuster;
                    damage->ownerId = crew->iShipId;
                    damage->selfId = (int)crew;
                    damage->bLockdown = customDamage->bLockdown;
                    damage->crystalShard = customDamage->crystalShard;
                    damage->bFriendlyFire = customDamage->bFriendlyFire;
                    damage->iStun = customDamage->iStun;
                    shipFriendlyFire = ex->explosionShipFriendlyFire;

                    ret = damage;
                }
            }
        }
        else
        {
            if (crew->species == "energy")
            {
                ret = EnergyAlien::GetRoomDamage(damage, (EnergyAlien*)crew);
            }
        }


        if (CM_EX(crew)->isIonDrone)
        {
            ret = IonDrone::GetRoomDamage(damage, (IonDrone*)crew);
        }
    }

    return ret;
}

HOOK_METHOD(ShipManager, UpdateCrewMembers, () -> void)
{
    blockDamageArea = true;
    super();
    blockDamageArea = false;


    CustomCrewManager *custom = CustomCrewManager::GetInstance();


    for (auto i : vCrewList)
    {
        Damage* dmg = new Damage;

        CrewMember::GetRoomDamage(dmg, i);

        unsigned int bitmask = false << 24 | dmg->bFriendlyFire << 16 | dmg->crystalShard << 8 | dmg->bLockdown;

        DamageArea(Pointf(i->x, i->y), dmg->iDamage, dmg->iShieldPiercing, dmg->fireChance, dmg->breachChance, dmg->stunChance, dmg->iIonDamage, dmg->iSystemDamage, dmg->iPersDamage, dmg->bHullBuster, dmg->ownerId, dmg->selfId, bitmask, dmg->iStun, true);

        delete dmg;

        if (custom->IsRace(i->species))
        {
            auto def = custom->GetDefinition(i->species);

            auto ex = CM_EX(i);
            float damageEnemies = ex->CalculateStat(CrewStat::DAMAGE_ENEMIES_AMOUNT, def) * G_->GetCFPS()->GetSpeedFactor() * 0.06245f;

            if (i->Functional() && damageEnemies != 0.f)
            {
                for (auto crew : vCrewList)
                {
                    if (crew->iRoomId == i->iRoomId && crew->iShipId != i->iShipId)
                    {
                        crew->DirectModifyHealth(-damageEnemies);
                    }
                }
            }
            float healCrewAmount = ex->CalculateStat(CrewStat::HEAL_CREW_AMOUNT, def);

            if (i->Functional() && healCrewAmount != 0.f)
            {
                float healCrew = G_->GetCFPS()->GetSpeedFactor() * healCrewAmount * 0.06245f;

                for (auto crew : vCrewList)
                {
                    if (crew->iRoomId == i->iRoomId && crew->iShipId == i->iShipId && crew != i)
                    {
                        if (healCrew > 0.f && crew->health.first != crew->health.second)
                        {
                            crew->fMedbay += 0.0000000001;
                        }
                        crew->DirectModifyHealth(healCrew);
                    }
                }
            }

            if (ex->powerActivated)
            {
                ShipManager* actualShip = this;
                if (ex->powerShip != iShipId)
                {
                    actualShip = current_target;
                }

                Damage* dmg = ex->GetPowerDamage();
                unsigned int bitmask = false << 24 | dmg->bFriendlyFire << 16 | dmg->crystalShard << 8 | dmg->bLockdown;

                shipFriendlyFire = def.powerDef.shipFriendlyFire;
                actualShip->DamageArea(CMA_EX(i->crewAnim)->effectWorldPos, dmg->iDamage, dmg->iShieldPiercing, dmg->fireChance, dmg->breachChance, dmg->stunChance, dmg->iIonDamage, dmg->iSystemDamage, dmg->iPersDamage, dmg->bHullBuster, dmg->ownerId, dmg->selfId, bitmask, dmg->iStun, true);

                ex->powerActivated = false;
            }
        }
    }
}

HOOK_METHOD(ShipManager, DamageCrew, (CrewMember *crew, int iDamage, int iShieldPiercing, int fireChance, int breachChance, int stunChance, int iIonDamage, int iSystemDamage, int iPersDamage, int bHullBuster, int ownerId, int selfId, int bLockdown, int iStun) -> char)
{
    Damage* dmg = (Damage*)&iDamage;

    if ((CrewMember*)dmg->selfId == crew)
    {
        return false;
    }

    return super(crew, iDamage, iShieldPiercing, fireChance, breachChance, stunChance, iIonDamage, iSystemDamage, iPersDamage, bHullBuster, ownerId, selfId, bLockdown, iStun);
}

HOOK_METHOD(ShipManager, DamageArea, (Pointf location,  int iDamage, int iShieldPiercing, int fireChance, int breachChance, int stunChance, int iIonDamage, int iSystemDamage, int iPersDamage, char bHullBuster, int ownerId, int selfId, int bLockdown, int iStun, bool forceHit) -> bool)
{
    if (blockDamageArea) return false;
    Damage* dmg = (Damage*)&iDamage;

    if (!shipFriendlyFire)
    {
        shipFriendlyFire = true;

        if (ownerId == iShipId)
        {
            int roomId = ship.GetSelectedRoomId(location.x, location.y, true);

            if (roomId == -1)
                return false;

            if (!bJumping)
            {
                for (auto i : vCrewList)
                {
                    if (i->iRoomId == roomId)
                    {

                        DamageCrew(i, iDamage, iShieldPiercing, fireChance, breachChance, stunChance, iIonDamage, iSystemDamage, iPersDamage, bHullBuster, ownerId, selfId, bLockdown, iStun);
                    }
                }
            }

            return true;
        }
    }

    return super(location, iDamage, iShieldPiercing, fireChance, breachChance, stunChance, iIonDamage, iSystemDamage, iPersDamage, bHullBuster, ownerId, selfId, bLockdown, iStun, forceHit);
}

HOOK_METHOD(CrewBox, constructor, (Point pos, CrewMember *crew, int number) -> void)
{
    super(pos, crew, number);

    auto custom = CustomCrewManager::GetInstance();
    if (custom->IsRace(crew->species))
    {
        auto crewDef = custom->GetDefinition(crew->species);

        if (crewDef.powerDef.hasSpecialPower)
        {
            if (!crewDef.powerDef.buttonLabel.data.empty())
            {
                powerButton.label = crewDef.powerDef.buttonLabel;
            }
        }
    }
}

HOOK_METHOD_PRIORITY(CrewBox, OnRender, 1000, () -> void)
{
    //return super();
    mindControlled.Update();
    stunned.Update();

    auto crew = pCrew;
    auto health = crew->health;
    if (health.first > 25.f || health.first == 0.f || std::floor(health.first) == std::floor(health.second))
    {
        flashHealthTracker.Stop(false);
    }

    if (number < 0)
    {
        return;
    }

    GL_Color boxColor = GL_Color();
    if (!crew->bMindControlled)
    {
        auto selectionState = crew->selectionState;
        if (selectionState == 2)
        {
            boxColor.r = 0.921f;
            boxColor.g = 0.921f;
            boxColor.b = 0.921f;
        }
        else if (selectionState == 1)
        {
            boxColor.r = 0.47f;
            boxColor.g = 1.f;
            boxColor.b = 0.47f;
        }
        else if (flashHealthTracker.running && flashHealthTracker.Progress(-1.f) < 0.5f)
        {
            boxColor.r = 1.f;
            boxColor.g = 0.392f;
            boxColor.b = 0.392f;
        }
        else
        {
            boxColor.r = 0.392f;
            boxColor.g = 0.392f;
            boxColor.b = 0.392f;
        }

        if (crew->fStunTime > 0.f && crew->selectionState != 1)
        {
            boxColor.r = 1.f;
            boxColor.g = 1.f;
            boxColor.b = 0.392f;
        }
    }
    else
    {
        boxColor.r = 1.f;
        boxColor.g = 0.f;
        boxColor.b = 1.f;
    }


    if (!bSelectable)
    {
        boxColor.r = 0.494f;
        boxColor.g = 0.858f;
        boxColor.b = 0.882f;
    }


    boxColor.a = 0.25f;

    if (!mouseHover)
    {
        CSurface::GL_RenderPrimitiveWithColor(boxBackground, boxColor);
        boxColor.a = 1.f;
        CSurface::GL_RenderPrimitiveWithColor(boxOutline, boxColor);
    }
    else
    {
        CSurface::GL_RenderPrimitiveWithColor(skillBoxBackground, boxColor);
        boxColor.a = 1.f;
        CSurface::GL_RenderPrimitiveWithColor(skillBoxOutline, boxColor);
    }

    if (crew->HasSpecialPower())
    {
        auto ex = CM_EX(crew);

        std::pair<float, float> cooldown;

        if (ex->temporaryPowerActive)
        {
            cooldown = ex->temporaryPowerDuration;
        }
        else
        {
            cooldown = crew->GetPowerCooldown();
        }

        int cooldownHeight = std::floor((cooldown.first / cooldown.second) * (box.h - 6));

        GL_Primitive* prim = nullptr;

        if (cooldownHeight == lastCooldownHeight && cooldownBar)
        {
            prim = cooldownBar;
        }
        else
        {
            CSurface::GL_DestroyPrimitive(cooldownBar);
            lastCooldownHeight = cooldownHeight;

            if (cooldownHeight <= 0)
            {
                cooldownBar = nullptr;
            }
            else
            {
                GL_Color barColor = GL_Color(133.f / 255.f, 231.f / 255.f, 237.f / 255.f, 1.f);

                if (CustomCrewManager::GetInstance()->IsRace(crew->species))
                {
                    auto powerDef = CustomCrewManager::GetInstance()->GetDefinition(crew->species).powerDef;

                    if (ex->temporaryPowerActive)
                    {
                        barColor = powerDef.tempPower.cooldownColor;
                    }
                    else
                    {
                        barColor = powerDef.cooldownColor;
                    }
                }

                prim = CSurface::GL_CreateRectPrimitive(box.x - 1, (box.h - cooldownHeight) + box.y - 3, 4, cooldownHeight, barColor);

                cooldownBar = prim;
            }
        }

        if (prim)
        {
            CSurface::GL_RenderPrimitive(prim);
        }
    }

    int healthMaxWidth = box.w - 37;
    int healthWidth = std::floor((health.first / health.second) * healthMaxWidth);

    if (health.first != 0 && healthWidth == 0)
    {
        healthWidth = 1;
    }



    if (healthWidth == lastHealthWidth)
    {
        CSurface::GL_RenderPrimitive(healthBar);
    }
    else
    {
        lastHealthWidth = healthWidth;
        GL_Primitive *prim = nullptr;
        if (health.first <= 0)
        {
            healthBar = nullptr;
        }
        else
        {
            CSurface::GL_DestroyPrimitive(healthBar);

            GL_Color healthColor = GL_Color(0.f, 1.f, 0.f, 1.f);



            if (health.first < health.second)
            {
                if (healthWidth < healthMaxWidth / 2)
                {
                    healthColor.g = ((float)healthWidth) / ((float)healthMaxWidth / 2);
                }
                else
                {
                    healthColor.g = 1.f;
                }

                healthColor.r = 1.f;
            }

            if (healthColor.g >= 1.f)
            {
                healthColor.g = 1.f;
            }




            prim = CSurface::GL_CreateRectPrimitive(box.x + 33, box.y + box.h - 8, healthWidth, 4, healthColor);
            healthBar = prim;
        }

        if (prim)
        {
            CSurface::GL_RenderPrimitive(prim);
        }
    }

    if (mouseHover)
    {
        int skillOffset = 0;

        if (crew->HasSpecialPower())
        {
            powerButton.OnRender();
            skillOffset = 24;
        }

        int skillNumber = 0;

        for (auto i : crew->blueprint.skillLevel)
        {
            crew->blueprint.RenderSkill(box.x + box.w + 7, (box.y + skillOffset) + 24 * skillNumber, 40, 8, skillNumber);

            skillNumber++;
        }

        if (!sTooltip.empty())
        {
            Point tooltipPosition = Point(box.x + box.w + 95, box.y);
            auto mouse = G_->GetMouseControl();


            mouse->SetTooltip(sTooltip);
            mouse->QueueStaticTooltip(tooltipPosition);
            mouse->InstantTooltip();
        }
    }
}

HOOK_METHOD(CrewBox, GetSelected, (int mouseX, int mouseY) -> CrewMember*)
{
    auto ret = super(mouseX, mouseY);

    if (CustomCrewManager::GetInstance()->IsRace(pCrew->species))
    {
        auto def = CustomCrewManager::GetInstance()->GetDefinition(pCrew->species);
        if (pCrew->HasSpecialPower() && mouseX < powerButton.hitbox.x + powerButton.hitbox.w && mouseX > powerButton.hitbox.x && mouseY < powerButton.hitbox.y + powerButton.hitbox.h && mouseY > powerButton.hitbox.y)
        {
            std::string tooltip = "";
            if (pCrew->PowerReady())
            {
                if (!def.powerDef.tooltip.data.empty())
                {
                    tooltip = def.powerDef.tooltip.GetText();

                    if (!tooltip.empty())
                    {
                        std::string replaceWith;
                        Settings::GetHotkeyName(replaceWith, "lockdown");
                        boost::algorithm::replace_all(tooltip, "\\1", replaceWith);
                    }
                }
            }
            else
            {
                auto ex = CM_EX(pCrew);
                auto state = ex->PowerReady();

                std::string tooltipName = "";
                std::string replaceValue = "";


                switch (state)
                {
                case POWER_NOT_READY_ACTIVATED:
                    tooltipName = "power_not_ready_activated";
                    break;
                case POWER_NOT_READY_PLAYER_SHIP:
                    tooltipName = "power_not_ready_player_ship";
                    break;
                case POWER_NOT_READY_ENEMY_SHIP:
                    tooltipName = "power_not_ready_enemy_ship";
                    break;
                case POWER_NOT_READY_ENEMY_IN_ROOM:
                    tooltipName = "power_not_ready_enemy_in_room";
                    break;
                case POWER_NOT_READY_FRIENDLY_IN_ROOM:
                    tooltipName = "power_not_ready_friendly_in_room";
                    break;
                case POWER_NOT_READY_SYSTEM_IN_ROOM:
                    tooltipName = "power_not_ready_system_in_room";
                    break;
                case POWER_NOT_READY_AI_DISABLED:
                    tooltipName = "power_not_ready_ai_disabled";
                    break;
                case POWER_NOT_READY_HAS_CLONEBAY:
                    tooltipName = "power_not_ready_has_clonebay";
                    break;
                case POWER_NOT_READY_OUT_OF_COMBAT:
                    tooltipName = "power_not_ready_out_of_combat";
                    break;
                case POWER_NOT_READY_IN_COMBAT:
                    tooltipName = "power_not_ready_in_combat";
                    break;
                case POWER_NOT_READY_SYSTEM_DAMAGED:
                    tooltipName = "power_not_ready_system_damaged";
                    break;
                case POWER_NOT_READY_MIN_HEALTH:
                    tooltipName = "power_not_ready_min_health";
                    replaceValue = boost::lexical_cast<std::string>(def.powerDef.playerReq.minHealth.value);
                    break;
                case POWER_NOT_READY_MAX_HEALTH:
                    tooltipName = "power_not_ready_max_health";
                    replaceValue = boost::lexical_cast<std::string>(def.powerDef.playerReq.maxHealth.value);
                    break;
                case POWER_NOT_READY_TELEPORTING:
                    tooltipName = "power_not_ready_teleporting";
                    break;
                }

                if (!tooltipName.empty())
                {
                    tooltip = G_->GetTextLibrary()->GetText(tooltipName);
                    if (!replaceValue.empty())
                    {
                        boost::algorithm::replace_all(tooltip, "\\1", replaceValue);
                    }
                }
            }

            if (!tooltip.empty())
            {
                sTooltip = tooltip;
            }
        }
    }

    return ret;
}

HOOK_METHOD(CrewMember, OnRender, (bool outlineOnly) -> void)
{
    super(outlineOnly);

    if (outlineOnly) return;

    auto ex = CMA_EX(crewAnim);

    CSurface::GL_PushMatrix();
    CSurface::GL_Translate(x, y);

    if (ex->tempEffectAnim != nullptr && !ex->tempEffectAnim->tracker.done && ex->tempEffectAnim->tracker.running)
    {
        CSurface::GL_PushMatrix();
        CSurface::GL_Translate(-std::ceil((float)ex->tempEffectAnim->info.frameWidth / 2), -std::ceil((float)ex->tempEffectAnim->info.frameHeight / 2));
        CSurface::GL_Translate(0, PositionShift());
        ex->tempEffectAnim->OnRender(1.f, COLOR_WHITE, false);
        CSurface::GL_PopMatrix();
    }


    for (auto boostAnim : ex->boostAnim)
    {
        if (boostAnim.second != nullptr && !boostAnim.second->tracker.done && boostAnim.second->tracker.running)
        {
            CSurface::GL_PushMatrix();
            CSurface::GL_Translate(-std::ceil((float)boostAnim.second->info.frameWidth / 2), -std::ceil((float)boostAnim.second->info.frameHeight / 2));
            CSurface::GL_Translate(0, PositionShift());
            boostAnim.second->OnRender(1.f, COLOR_WHITE, false);
            CSurface::GL_PopMatrix();
        }
    }


    CSurface::GL_PopMatrix();
}

HOOK_METHOD(CrewMember, OnRenderHealth, () -> void)
{
    auto ex = CMA_EX(crewAnim);

    if (ex->effectAnim != nullptr && !ex->effectAnim->tracker.done)
    {
        CSurface::GL_PushMatrix();
        CSurface::GL_Translate(ex->effectPos.x, ex->effectPos.y);
        ex->effectAnim->OnRender(1.f, COLOR_WHITE, false);
        CSurface::GL_PopMatrix();
    }

    CSurface::GL_PushMatrix();
    CSurface::GL_Translate(x, y);

    if (ex->effectFinishAnim != nullptr && !ex->effectFinishAnim->tracker.done)
    {
        CSurface::GL_PushMatrix();
        CSurface::GL_Translate(-std::ceil((float)ex->effectFinishAnim->info.frameWidth / 2), -std::ceil((float)ex->effectFinishAnim->info.frameHeight / 2));
        ex->effectFinishAnim->OnRender(1.f, COLOR_WHITE, false);
        CSurface::GL_PopMatrix();
    }

    CSurface::GL_PopMatrix();

    super();

}

HOOK_METHOD(CrewAnimation, OnRender, (float scale, int selectedState, bool outlineOnly) -> void)
{
    auto custom = CustomCrewManager::GetInstance();
    auto aex = CMA_EX(this);

    if (outlineOnly || (!aex->temporaryPowerActive || (custom->IsRace(race) && custom->GetDefinition(race).powerDef.tempPower.baseVisible)))
    {
        super(scale, selectedState, outlineOnly);
    }

    if (!outlineOnly && aex->temporaryPowerActive && aex->tempEffectStrip)
    {
        CSurface::GL_PushMatrix();
        CSurface::GL_Translate(lastPosition.x - 17.f, lastPosition.y - 17.f);

        anims[direction][status].SetAnimationId(aex->tempEffectStrip);
        anims[direction][status].OnRender(1.f, COLOR_WHITE, false);

        CSurface::GL_PopMatrix();
    }
}

static bool blockSetBonusPower = false;

HOOK_METHOD(ShipSystem, SetBonusPower, (int amount, int permanentPower) -> void)
{
    if (blockSetBonusPower) return;

    super(amount, permanentPower);
}

HOOK_METHOD(WeaponSystem, SetBonusPower, (int amount, int permanentPower) -> void)
{
    if (blockSetBonusPower) return;

    super(amount, permanentPower);
}

HOOK_METHOD(DroneSystem, SetBonusPower, (int amount, int permanentPower) -> void)
{
    if (blockSetBonusPower) return;

    super(amount, permanentPower);
}



HOOK_METHOD(ShipManager, OnLoop, () -> void)
{
    auto custom = CustomCrewManager::GetInstance();

    for (auto i : vSystemList)
    {
        int bonusPowerCounter = 0;
        int permanentPowerCounter = 0;

        for (auto j : vCrewList)
        {
            if (i->roomId == j->iRoomId && j->iShipId == iShipId && j->Functional())
            {
                if (custom->IsRace(j->species))
                {
                    auto def = custom->GetDefinition(j->species);

                    auto ex = CM_EX(j);

                    int bonusPower = ex->CalculateStat(CrewStat::BONUS_POWER, def);

                    bonusPowerCounter += bonusPower;

                    if (j->AtFinalGoal() && !j->IsDrone())
                    {
                        permanentPowerCounter += bonusPower;
                    }
                }

                if (j->ProvidesPower())
                {
                    bonusPowerCounter++;
                    if (j->AtFinalGoal() && !j->IsDrone())
                    {
                        permanentPowerCounter++;
                    }
                }
            }
        }

        i->SetBonusPower(bonusPowerCounter, permanentPowerCounter);
    }

    blockSetBonusPower = true;
    super();
    blockSetBonusPower = false;
}

static float sabotageMultiplier = 1.f;



HOOK_METHOD(ShipSystem, PartialDamage, (float amount) -> bool)
{
    return super(amount * sabotageMultiplier);
}


HOOK_METHOD(CrewMember, UpdateRepair, () -> void)
{
    auto custom = CustomCrewManager::GetInstance();
    auto ex = CM_EX(this);
    auto def = custom->GetDefinition(this->species);
    if (custom->IsRace(species))
    {
        sabotageMultiplier = ex->CalculateStat(CrewStat::SABOTAGE_SPEED_MULTIPLIER, def);
    }

    super();

    sabotageMultiplier = 1.f;
}

static bool windowFrameCheck = false;
static int windowFrameHeight = 0;

HOOK_METHOD(WindowFrame, constructor, (int x, int y, int w, int h) -> void)
{
    if (windowFrameCheck)
    {
        h = windowFrameHeight;
        windowFrameCheck = false;
    }

    super(x, y, w, h);
}

bool blockAnimationUpdate = false;

HOOK_METHOD(AnimationTracker, Update, () -> void)
{
    if (blockAnimationUpdate) return;

    super();
}

HOOK_METHOD(CrewMember, OnRenderHealth, () -> void)
{
    bool fullHealth = std::floor(health.first) == std::floor(health.second);

    if (fullHealth)
    {
        flashHealthTracker.SetProgress(0.5f);
        blockAnimationUpdate = true;
    }

    super();

    blockAnimationUpdate = false;

    if (fullHealth)
    {
        flashHealthTracker.Start(0.5f);
    }
}

HOOK_METHOD(CrewAI, PrioritizeIntruderRoom, (CrewMember *crew, int roomId, int target) -> int)
{
    if (!crew->CanSabotage() && !CM_EX(crew)->isIonDrone)
    {
        if (ship->CountCrewShipId(roomId, !crew->iShipId) == 0)
        {
            return 1000;
        }
    }

    return super(crew, roomId, target);
}

HOOK_METHOD(CrewMember, Clone, () -> void)
{
    bool cloneLoseSkills = false;
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(this->species);

    auto ex = CM_EX(this);
    ex->CalculateStat(CrewStat::CLONE_LOSE_SKILLS, def, &cloneLoseSkills);
    if (!CustomCrewManager::GetInstance()->IsRace(species) || cloneLoseSkills) return super();

    bOutOfGame = false;
    bDead = false;
    fStunTime = 0.f;
    Restart();

    crewAnim->status = 6;
    crewAnim->direction = 0;
    crewAnim->anims[0][6].tracker.SetLoop(false, 0.f);
    crewAnim->anims[0][6].Start(true);
    crewAnim->anims[0][6].StartReverse(true);
    G_->GetSoundControl()->PlaySoundMix("cloneArrive", -1.f, false);
}

static bool needsIntruderControllable = false;

HOOK_METHOD(CrewMember, NeedsIntruderSlot, () -> bool)
{
    if (needsIntruderControllable)
    {
        return super() || GetControllable();
    }

    return super();
}


HOOK_METHOD(CrewAI, OnLoop, () -> void)
{
    super();
    if (ship->iShipId == 0)
    {
        bool wasCalm = bCalmShip;

        bCalmShip = false;
        needsIntruderControllable = true;
        CheckForHealing();
        needsIntruderControllable = false;
        bCalmShip = wasCalm;
    }
}

HOOK_METHOD(CrewAI, UpdateCrewMember, (int crewId) -> void)
{
    auto crewMember = crewList[crewId];

    if (!crewMember->GetControllable() && !crewMember->bMindControlled && !crewMember->IsDead() && crewMember->iShipId == 0 && !crewMember->IsDrone())
    {
        super(crewId);
    }
    else
    {
        return super(crewId);
    }
}

HOOK_METHOD(CrewAI, PrioritizeTask, (CrewTask task, int crewId) -> int)
{
    if (task.taskId == 0 && !crewList[crewId]->CanMan())
    {
        return 1001;
    }

    return super(task, crewId);
}

HOOK_STATIC(CrewMember, GetSavedPosition, (Slot *ret, CrewMember *crew) -> void)
{
    if (!crew->GetControllable())
    {
        ret->roomId = crew->finalGoal.roomId;
        ret->slotId = crew->finalGoal.slotId;
        ret->worldLocation = crew->finalGoal.worldLocation;

        return;
    }

    return super(ret, crew);
}

HOOK_METHOD(ShipManager, GetSelectedCrewPoint, (int mX, int mY, bool intruder) -> CrewMember*)
{
    if (vCrewList.empty())
    {
        return nullptr;
    }

    for (auto i : vCrewList)
    {
        if (i->ContainsPoint(mX, mY))
        {
            if (i->IsDead())
            {
                continue;
            }
            if (intruder != i->NeedsIntruderSlot())
            {
                continue;
            }
            if (!i->GetControllable() && !intruder)
            {
                continue;
            }

            if (Point(i->x, i->y).Distance(Point(mX, mY)) <= 14)
            {
                return i;
            }
        }
    }

    return nullptr;
}

HOOK_METHOD(CrewControl, MouseMove, (int mX, int mY, int wX, int wY) -> void)
{
    super(mX, mY, wX, wY);

    if (selectedDoor) return;

    for (auto i : shipManager->vCrewList)
    {
        if (i->Functional() && i->selectionState != 1 && std::find(potentialSelectedCrew.begin(), potentialSelectedCrew.end(), i) == potentialSelectedCrew.end())
        {
            Pointf pos = Pointf(i->x, i->y) - Pointf(17.f, 17.f);

            if (wX - pos.x < 35.f && wX - pos.x > 0.f && wY - pos.y < 35.f && wY - pos.y > 0.f)
            {
                potentialSelectedCrew.push_back(i);
                i->selectionState = 2;
            }
        }
    }

    selectedCrew.erase(std::remove_if(selectedCrew.begin(), selectedCrew.end(), [](CrewMember* crew) { return !crew->GetControllable(); }), selectedCrew.end());
}

HOOK_STATIC(CrewMember, GetTooltip, (std::string& strRef, CrewMember* crew) -> std::string&)
{
    super(strRef, crew);
    auto custom = CustomOptionsManager::GetInstance();
    if (custom->advancedCrewTooltips.currentValue)
    {
        std::string tooltip = "";
        tooltip += "-" + crew->blueprint.crewNameLong.GetText() + " (" + crew->blueprint.desc.title.GetText() + "):" + '\n';
        int maxHealth = (int)crew->health.second;
        if (crew->health.first == 0)
        {
            tooltip += G_->GetTextLibrary()->GetText("advanced_health_tooltip") + ": " + G_->GetTextLibrary()->GetText("death_tooltip");
        }
        else if (crew->health.first == crew->health.second)
        {
            std::stringstream stream;
            tooltip += G_->GetTextLibrary()->GetText("advanced_health_tooltip") + ": " + std::to_string(maxHealth) + "/" + std::to_string(maxHealth) + " (100%)";
        }
        else if (custom->advancedCrewTooltipRounding.currentAmount == 0)
        {
            tooltip += G_->GetTextLibrary()->GetText("advanced_health_tooltip") + ": " + std::to_string((int)crew->health.first) + "/" + std::to_string(maxHealth) + " (" + std::to_string((int)(crew->health.first / maxHealth * 100)) + "%)";
        }
        else
        {
            std::stringstream stream;
            stream << std::fixed <<std::setprecision(custom->advancedCrewTooltipRounding.currentAmount) << crew->health.first;
            tooltip += G_->GetTextLibrary()->GetText("advanced_health_tooltip") + ": " + stream.str() + "/" + std::to_string(maxHealth) + " (" + std::to_string((int)(crew->health.first / maxHealth * 100)) + "%)";
        }

        if (crew->fStunTime != 0)
        {
            tooltip += '\n';
            std::stringstream stream;
            stream << std::fixed <<std::setprecision(1) << crew->fStunTime * CustomCrewManager::GetInstance()->GetDefinition(crew->species).stunMultiplier;
            std::string currentText = G_->GetTextLibrary()->GetText("crew_stun_time");
            currentText = boost::algorithm::replace_all_copy(currentText, "\\1", stream.str());
            tooltip += currentText;
        }

        if (custom->showEnemyPowers.currentValue && crew->iShipId == 1)
        {
            tooltip += '\n';
            for (auto j : crew->blueprint.powers)
            {
                tooltip += "*" + j.GetText() + '\n';
            }
            boost::trim_right(tooltip);
        }
        else if (custom->showAllyPowers.currentValue && crew->iShipId == 0)
        {
            tooltip += '\n';
            for (auto j : crew->blueprint.powers)
            {
                tooltip += "*" + j.GetText() + '\n';
            }
            boost::trim_right(tooltip);
        }
        tooltip += '\n';

//        if (crew->bMindControlled)
//        {
//            tooltip += G_->GetTextLibrary()->GetText("mind_controlled_tooltip");
//        }
        boost::trim_right(tooltip);
        strRef.assign(tooltip);
        return strRef;
    }
    else
    {
        return super(strRef, crew);
    }
}

HOOK_METHOD(CrewAnimation, FireShot, () -> bool)
{
    if (CMA_EX(this)->isMantisAnimation)
    {
        anims[0][7].SetProgress(anims[direction][status].tracker.Progress(-1.f));
        anims[1][7].SetProgress(anims[direction][status].tracker.Progress(-1.f));
        anims[2][7].SetProgress(anims[direction][status].tracker.Progress(-1.f));
        anims[3][7].SetProgress(anims[direction][status].tracker.Progress(-1.f));

        auto frame = anims[direction][status].currentFrame;
        bool ret = false;

        if (frame == 3)
        {
            ret = false;
            if (!uniqueBool1)
            {
                uniqueBool1 = true;
                ret = true;
            }
        }
        else if (frame > 2)
        {
            ret = false;
        }
        else
        {
            uniqueBool1 = false;
            ret = false;
        }

        return ret;
    }

    return super();
}

HOOK_METHOD(CrewAnimation, GetFiringFrame, () -> int)
{
    if (CMA_EX(this)->isMantisAnimation)
    {
        return -1;
    }

    return super();
}

CrewAnimation_Extend::~CrewAnimation_Extend()
{
    if (effectAnim) effectAnim->destructor();
    if (tempEffectAnim) tempEffectAnim->destructor();
}
