#pragma GCC push_options
#pragma GCC optimize ("O1")
#include "CustomDrones.h"
#include "CustomCrew.h"
#include <algorithm>
#include "PALMemoryProtection.h"

bool CrewDrone::_HS_GetControllable()
{
    bool req = this->iShipId == 0 && !this->IsDead() && !this->bMindControlled && this->Functional();
    if (!req)
    {
        return false;
    }

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::CONTROLLABLE, def, &ret);
        return ret && req;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CONTROLLABLE, def, &ret);
        return ret && req;
    }

    return false;
}

bool CrewDrone::_HS_CanFight()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    if (this->_drone.blueprint->name == "BOARDER_ION") {
        if (custom->IsRace("boarder_ion"))
        {
            auto ex = CM_EX(this);
            auto def = custom->GetDefinition("boarder_ion");
            bool ret = false;
            ex->CalculateStat(CrewStat::CAN_FIGHT, def, &ret);
            return ret;
        }
        return false;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_FIGHT, def, &ret);
        return ret;
    }

    if (this->_drone.blueprint->typeName == "BOARDER")
    {
        return true;
    }
    else if (this->_drone.blueprint->typeName == "BOARDER_ION")
    {
        return false;
    }
    else if (this->_drone.blueprint->typeName == "REPAIR")
    {
        return false;
    }
    else if (this->_drone.blueprint->typeName == "BATTLE")
    {
        return true;
    }
}

bool CrewDrone::_HS_CanRepair()
{
    bool req = this->_drone.powered;
    if (!req)
    {
        return false;
    }

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_REPAIR"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_REPAIR");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_REPAIR, def, &ret);
        return ret && req;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_REPAIR, def, &ret);
        return ret && req;
    }

    if (this->_drone.blueprint->typeName == "BOARDER")
    {
        return false;
    }
    else if (this->_drone.blueprint->typeName == "BOARDER_ION")
    {
        return false;
    }
    else if (this->_drone.blueprint->typeName == "REPAIR")
    {
        return req;
    }
    else if (this->_drone.blueprint->typeName == "BATTLE")
    {
        return false;
    }
}

bool CrewDrone::_HS_CanSabotage()
{
    bool req = this->intruder;
    if (!req)
    {
        return false;
    }
    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_SABOTAGE"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_SABOTAGE");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_SABOTAGE, def, &ret);
        return ret && req;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_SABOTAGE, def, &ret);
        return ret && req;
    }

    if (this->_drone.blueprint->typeName == "BOARDER")
    {
        return true;
    }
    else if (this->_drone.blueprint->typeName == "BOARDER_ION")
    {
        return true;
    }
    else if (this->_drone.blueprint->typeName == "REPAIR")
    {
        return false;
    }
    else if (this->_drone.blueprint->typeName == "BATTLE")
    {
        return false;
    }
}

bool CrewDrone::_HS_CanMan()
{
    bool req = !this->intruder && this->fStunTime == 0.f && this->crewAnim->status != 3;
    if (!req)
    {
        return false;
    }

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_MAN"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_MAN");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_MAN, def, &ret);
        return ret && req;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_MAN, def, &ret);
        return ret && req;
    }

    return false;
}

/*
bool CrewDrone::_HS_CanTeleport()
{
    bool ret = this->CrewMember::CanTeleport(); // vanilla CanTeleport
    if (!ret) return false;

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        ret = false;
        ex->CalculateStat(CrewStat::CAN_TELEPORT, def, &ret);
        return ret;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        ret = false;
        ex->CalculateStat(CrewStat::CAN_TELEPORT, def, &ret);
        return ret;
    }

    return false;
}
*/

bool CrewDrone::_HS_CanSuffocate()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_SUFFOCATE"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_SUFFOCATE");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_SUFFOCATE, def, &ret);
        return ret;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_SUFFOCATE, def, &ret);
        return ret;
    }

    return false;
}

bool CrewDrone::_HS_CanBurn()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_BURN"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_BURN");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_BURN, def, &ret);
        return ret;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::CAN_BURN, def, &ret);
        return ret;
    }

    return false;
}

float CrewDrone::_HS_GetMoveSpeedMultiplier()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto def = custom->GetDefinition("boarder_ion");
        return CM_EX(this)->CalculateStat(CrewStat::MOVE_SPEED_MULTIPLIER, def);
    }
    if (custom->IsRace(this->species))
    {
        auto def = custom->GetDefinition(this->species);
        return CM_EX(this)->CalculateStat(CrewStat::MOVE_SPEED_MULTIPLIER, def);
    }

    return 0.5f;
}

float CrewDrone::_HS_GetRepairSpeed()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto def = custom->GetDefinition("boarder_ion");
        return CM_EX(this)->CalculateStat(CrewStat::REPAIR_SPEED_MULTIPLIER, def);
    }
    if (custom->IsRace(this->species))
    {
        auto def = custom->GetDefinition(this->species);
        return CM_EX(this)->CalculateStat(CrewStat::REPAIR_SPEED_MULTIPLIER, def);
    }

    return 2.f;
}

int CrewDrone::_HS_GetMaxHealth()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto def = custom->GetDefinition("boarder_ion");
        return CM_EX(this)->CalculateMaxHealth(def);
    }
    if (custom->IsRace(this->species))
    {
        auto def = custom->GetDefinition(this->species);
        return CM_EX(this)->CalculateMaxHealth(def);
    }

    if (this->_drone.blueprint->typeName == "BOARDER")
    {
        return this->_drone.blueprint->name == "BOARDER_ION" ? 125 : 150;
    }
    else if (this->_drone.blueprint->typeName == "BOARDER_ION")
    {
        return 125;
    }
    else if (this->_drone.blueprint->typeName == "REPAIR")
    {
        return 25;
    }
    else if (this->_drone.blueprint->typeName == "BATTLE")
    {
        return 150;
    }
}

float CrewDrone::_HS_GetDamageMultiplier()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto def = custom->GetDefinition("boarder_ion");
        return CM_EX(this)->CalculateStat(CrewStat::DAMAGE_MULTIPLIER, def);
    }
    if (custom->IsRace(this->species))
    {
        auto def = custom->GetDefinition(this->species);
        return CM_EX(this)->CalculateStat(CrewStat::DAMAGE_MULTIPLIER, def);
    }

    if (this->_drone.blueprint->typeName == "BOARDER")
    {
        return 1.2f;
    }
    else if (this->_drone.blueprint->typeName == "BOARDER_ION")
    {
        return 1.f;
    }
    else if (this->_drone.blueprint->typeName == "REPAIR")
    {
        return 1.f;
    }
    else if (this->_drone.blueprint->typeName == "BATTLE")
    {
        return 1.2f;
    }
}

bool CrewDrone::_HS_ProvidesPower()
{
    bool req = this->_drone.powered;
    if (!req)
    {
        return false;
    }

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        return custom->GetDefinition("boarder_ion")->providesPower && req;
    }
    if (custom->IsRace(this->species))
    {
        return custom->GetDefinition(this->species)->providesPower && req;
    }

    return false;
}

float CrewDrone::_HS_GetFireRepairMultiplier()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto def = custom->GetDefinition("boarder_ion");
        return CM_EX(this)->CalculateStat(CrewStat::FIRE_REPAIR_MULTIPLIER, def);
    }
    if (custom->IsRace(this->species))
    {
        auto def = custom->GetDefinition(this->species);
        return CM_EX(this)->CalculateStat(CrewStat::FIRE_REPAIR_MULTIPLIER, def);
    }

    return 1.2f;
}

bool CrewDrone::_HS_IsTelepathic()
{
    bool req = this->_drone.powered;
    if (!req)
    {
        return false;
    }

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_TELEPATHIC"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_TELEPATHIC");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = (ex->temporaryPowerActive && ex->GetPowerDef()->tempPower.isTelepathic.enabled) ? ex->GetPowerDef()->tempPower.isTelepathic.value : def->isTelepathic;
        ex->CalculateStat(CrewStat::IS_TELEPATHIC, def, &ret);
        return ret && req;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = (ex->temporaryPowerActive && ex->GetPowerDef()->tempPower.isTelepathic.enabled) ? ex->GetPowerDef()->tempPower.isTelepathic.value : def->isTelepathic;
        ex->CalculateStat(CrewStat::IS_TELEPATHIC, def, &ret);
        return ret && req;
    }

    return false;
}

float CrewDrone::_HS_GetSuffocationModifier()
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto def = custom->GetDefinition("boarder_ion");
        return CM_EX(this)->CalculateStat(CrewStat::SUFFOCATION_MODIFIER, def);
    }
    if (custom->IsRace(this->species))
    {
        auto def = custom->GetDefinition(this->species);
        return CM_EX(this)->CalculateStat(CrewStat::SUFFOCATION_MODIFIER, def);
    }

    return 0.f;
}

bool CrewDrone::_HS_IsAnaerobic()
{
    bool req = this->_drone.powered;
    if (!req)
    {
        return false;
    }

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

//    if (this->GetShipObject()->HasAugmentation("ALL_DRONE_CREW_ANAEROBIC"))
//    {
//        return (bool)this->GetShipObject()->GetAugmentationValue("ALL_DRONE_CREW_ANAEROBIC");
//    }
    if (this->_drone.blueprint->name == "BOARDER_ION" && custom->IsRace("boarder_ion"))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition("boarder_ion");
        bool ret = false;
        ex->CalculateStat(CrewStat::IS_ANAEROBIC, def, &ret);
        return ret && req;
    }

    if (custom->IsRace(this->species))
    {
        auto ex = CM_EX(this);
        auto def = custom->GetDefinition(this->species);
        bool ret = false;
        ex->CalculateStat(CrewStat::IS_ANAEROBIC, def, &ret);
        return ret && req;
    }

    return false;
}

int CrewDrone::_HS_BlockRoom()
{
    if (CM_EX(this)->isAbilityDrone)
        return ((AbilityDrone*)this)->new_BlockRoom();
    else if (CM_EX(this)->isIonDrone)
        return ((IonDrone*)this)->lastRoom;

    return -1;
}

void SetupVTable(CrewDrone *crew)
{
    void** vtable = *(void***)crew;

    MEMPROT_SAVE_PROT(dwOldProtect);
    MEMPROT_PAGESIZE();
    MEMPROT_UNPROTECT(&vtable[0], sizeof(void*) * 57, dwOldProtect);
    vtable[23] = (void*)&CrewDrone::_HS_GetControllable;
    vtable[25] = (void*)&CrewDrone::_HS_CanFight;
    vtable[26] = (void*)&CrewDrone::_HS_CanRepair;
    vtable[27] = (void*)&CrewDrone::_HS_CanSabotage;
    vtable[28] = (void*)&CrewDrone::_HS_CanMan;
    //if (g_dronesCanTeleport) vtable[29] = (void*)&CrewDrone::_HS_CanTeleport;
    vtable[31] = (void*)&CrewDrone::_HS_CanSuffocate;
    vtable[32] = (void*)&CrewDrone::_HS_CanBurn;
    vtable[33] = (void*)&CrewDrone::_HS_GetMaxHealth;
    vtable[40] = (void*)&CrewDrone::_HS_GetMoveSpeedMultiplier;
    vtable[41] = (void*)&CrewDrone::_HS_GetRepairSpeed;
    vtable[42] = (void*)&CrewDrone::_HS_GetDamageMultiplier;
    vtable[43] = (void*)&CrewDrone::_HS_ProvidesPower;
    vtable[45] = (void*)&CrewDrone::_HS_GetFireRepairMultiplier;
    vtable[46] = (void*)&CrewDrone::_HS_IsTelepathic;
    vtable[52] = (void*)&CrewDrone::_HS_GetSuffocationModifier;
    vtable[53] = (void*)&CrewDrone::_HS_BlockRoom;
    vtable[55] = (void*)&CrewDrone::_HS_IsAnaerobic;
    MEMPROT_REPROTECT(&vtable[0], sizeof(void*) * 57, dwOldProtect);
}

HOOK_METHOD(CrewMemberFactory, CreateRepairDrone, (int shipId, DroneBlueprint* bp) -> RepairDrone*)
{
    LOG_HOOK("HOOK_METHOD -> CrewMemberFactory::CreateRepairDrone -> Begin (DroneVTable.cpp)\n")
    RepairDrone* drone = super(shipId, bp);

    SetupVTable(drone);

    return drone;
}



HOOK_METHOD(CrewMemberFactory, CreateBattleDrone, (int shipId, DroneBlueprint* bp) -> BattleDrone*)
{
    LOG_HOOK("HOOK_METHOD -> CrewMemberFactory::CreateBattleDrone -> Begin (DroneVTable.cpp)\n")
    BattleDrone* drone = super(shipId, bp);

    SetupVTable(drone);

    return drone;
}

HOOK_METHOD_PRIORITY(CrewMemberFactory, CreateBoarderDrone, -900, (int shipId, DroneBlueprint* bp) -> BoarderDrone*)
{
    LOG_HOOK("HOOK_METHOD_PRIORITY -> CrewMemberFactory::CreateBoarderDrone -> Begin (DroneVTable.cpp)\n")
    BoarderDrone* drone = super(shipId, bp);

    SetupVTable(drone);

    return drone;
}

#pragma GCC pop_options
