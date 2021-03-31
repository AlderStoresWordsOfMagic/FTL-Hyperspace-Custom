#pragma once
#include "FTLGame.h"
#include <array>

struct CrewDefinition;

enum PowerReadyState
{
    POWER_NOT_READY_COOLDOWN,
    POWER_READY,
    POWER_NOT_READY_ACTIVATED,
    POWER_NOT_READY_ENEMY_SHIP,
    POWER_NOT_READY_PLAYER_SHIP,
    POWER_NOT_READY_ENEMY_IN_ROOM,
    POWER_NOT_READY_FRIENDLY_IN_ROOM,
    POWER_NOT_READY_SYSTEM_IN_ROOM,
    POWER_NOT_READY_HAS_CLONEBAY,
    POWER_NOT_READY_AI_DISABLED,
    POWER_NOT_READY_OUT_OF_COMBAT,
    POWER_NOT_READY_IN_COMBAT,
    POWER_NOT_READY_MIN_HEALTH,
    POWER_NOT_READY_MAX_HEALTH,
    POWER_NOT_READY_SYSTEM_DAMAGED,
    POWER_NOT_READY_TELEPORTING
};

enum class CrewStat
{
    MAX_HEALTH = 0,
    STUN_MULTIPLIER,
    MOVE_SPEED_MULTIPLIER,
    REPAIR_SPEED_MULTIPLIER,
    DAMAGE_MULTIPLIER,
    RANGED_DAMAGE_MULTIPLIER,
    FIRE_REPAIR_MULTIPLIER,
    SUFFOCATION_MODIFIER,
    FIRE_DAMAGE_MULTIPLIER,
    OXYGEN_CHANGE_SPEED,
    DAMAGE_TAKEN_MULTIPLIER,
    PASSIVE_HEAL_AMOUNT,
    PASSIVE_HEAL_DELAY,
    SABOTAGE_SPEED_MULTIPLIER,
    ALL_DAMAGE_TAKEN_MULTIPLIER,
    HEAL_SPEED_MULTIPLIER,
    HEAL_CREW_AMOUNT,
    DAMAGE_ENEMIES_AMOUNT,
    BONUS_POWER,
    POWER_DRAIN,
    CAN_FIGHT,
    CAN_REPAIR,
    CAN_SABOTAGE,
    CAN_MAN,
    CAN_SUFFOCATE,
    CONTROLLABLE,
    CAN_BURN,
    IS_TELEPATHIC,
    IS_ANAEROBIC,
    CAN_PHASE_THROUGH_DOORS,
    DETECTS_LIFEFORMS,
    CLONE_LOSE_SKILLS,
    POWER_DRAIN_FRIENDLY
};

static const std::array<std::string, 33> crewStats =
{
    "maxHealth",
    "stunMultiplier",
    "moveSpeedMultiplier",
    "repairSpeed",
    "damageMultiplier",
    "rangedDamageMultiplier",
    "fireRepairMultiplier",
    "suffocationModifier",
    "fireDamageMultiplier",
    "oxygenChangeSpeed",
    "damageTakenMultiplier",
    "passiveHealAmount",
    "passiveHealDelay",
    "sabotageSpeedMultiplier",
    "allDamageTakenMultiplier",
    "healSpeed",
    "healCrewAmount",
    "damageEnemiesAmount",
    "bonusPower",
    "powerDrain",
    "canFight",
    "canRepair",
    "canSabotage",
    "canMan",
    "canSuffocate",
    "controllable",
    "canBurn",
    "isTelepathic",
    "isAnaerobic",
    "canPhaseThroughDoors",
    "detectsLifeforms",
    "cloneLoseSkills",
    "powerDrainFriendly",
};

struct StatBoost
{
    enum class BoostType
    {
        MULT,
        FLAT,
        SET,
        FLIP
    };

    enum class BoostSource
    {
        CREW,
        AUGMENT
    };

    enum class ShipTarget
    {
        PLAYER_SHIP,
        ENEMY_SHIP,
        CURRENT_ALL,
        CURRENT_ROOM,
        OTHER_ALL,
        ALL
    };

    enum class SystemRoomTarget
    {
        ALL,
        NONE
    };

    enum class CrewTarget
    {
        ALLIES,
        ENEMIES,
        SELF,
        ALL
    };

    CrewStat stat;
    CrewMember* crewSource;
    float amount;
    bool value;
    bool isBool = false;
    int priority = -1;
    float duration = -1;
    //TimerHelper* timerHelper;
    bool affectsSelf;
    std::vector<std::string> whiteList = std::vector<std::string>();
    std::vector<std::string> blackList = std::vector<std::string>();
    std::pair<std::vector<int>,std::vector<int>> sourceRoomIds = std::pair<std::vector<int>,std::vector<int>>();
    std::vector<std::string> systemList = std::vector<std::string>();

    SystemRoomTarget systemRoomTarget;
    BoostType boostType;
    BoostSource boostSource;
    ShipTarget shipTarget;
    CrewTarget crewTarget;
    int sourceShipId;

    ~StatBoost()
    {
        //delete timerHelper;
    }
};

struct CrewAnimation_Extend
{
public:
    CrewAnimation *orig;
    Animation* effectAnim = nullptr;
    Animation* tempEffectAnim = nullptr;
    Animation* effectFinishAnim = nullptr;
    GL_Texture* tempEffectStrip = nullptr;

    bool isMantisAnimation = false;
    bool isIonDrone = false;

    Pointf effectPos;
    Pointf effectWorldPos;
    bool powerDone = true;

    bool temporaryPowerActive;
    bool isAbilityDrone = false;

    void OnInit(const std::string& name, Pointf position, bool enemy);

    ~CrewAnimation_Extend();
};

struct CrewMember_Extend
{
public:
    CrewMember *orig;
    bool canPhaseThroughDoors = false;
    bool isHealing = false;
    TimerHelper* passiveHealTimer;
    int lastRoom = -1;
    int lastShipId = -1;
    bool exploded = false;
    bool triggerExplosion = false;

    bool hasSpecialPower = false;
    bool hasTemporaryPower = false;

    std::pair<float, float> powerCooldown = std::pair<float, float>();
    std::pair<float, float> temporaryPowerDuration = std::pair<float, float>();

    int powerRoom;
    int powerShip;

    bool powerActivated = false;
    bool temporaryPowerActive = false;

    void ActivatePower();
    void PreparePower();
    void ActivateTemporaryPower();
    void TemporaryPowerFinished();
    Damage* GetPowerDamage();
    PowerReadyState PowerReady();

    bool isIonDrone = false;
    bool isAbilityDrone = false;

    float prevStun = 0.f; // for use in stun resistance checking

    std::vector<StatBoost> outgoingStatBoosts = std::vector<StatBoost>();
    std::vector<StatBoost> outgoingAbilityStatBoosts = std::vector<StatBoost>();
//    std::vector<StatBoost> outgoingTimedStatBoosts = std::vector<StatBoost>();
    std::vector<StatBoost> outgoingTimedAbilityStatBoosts = std::vector<StatBoost>();
    std::vector<StatBoost> timedStatBoosts = std::vector<StatBoost>();
//    std::vector<StatBoost> personalStatBoosts;

    void Initialize(CrewBlueprint& bp, int shipId, bool enemy, CrewAnimation *animation);


    ~CrewMember_Extend()
    {
        delete passiveHealTimer;
    }

    float CalculateStat(CrewStat stat, const CrewDefinition& def, bool* boolValue=nullptr);
};

CrewMember_Extend* Get_CrewMember_Extend(CrewMember* c);
CrewAnimation_Extend* Get_CrewAnimation_Extend(CrewAnimation *c);

#define CM_EX Get_CrewMember_Extend
#define CMA_EX Get_CrewAnimation_Extend
