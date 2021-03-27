#include "CustomCrew.h"


static bool __attribute__((fastcall)) CrewMember_GetControllable(CrewMember *_this)
{
    bool req = _this->iShipId == 0 && !_this->bDead && !_this->bMindControlled;

    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_CONTROLLABLE"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_CONTROLLABLE"));
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.controllable.enabled)
    {
        return def.powerDef.tempPower.controllable.value && req;
    }

    return def.controllable && req;
}

static bool __attribute__((fastcall)) CrewMember_CanSuffocate(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_SUFFOCATE"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_SUFFOCATE"));
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.canSuffocate.enabled)
    {
        return def.powerDef.tempPower.canSuffocate.value;
    }

    return def.canSuffocate;
}

static bool __attribute__((fastcall)) CrewMember_CanFight(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_FIGHT"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_FIGHT"));
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.canFight.enabled)
    {
        return def.powerDef.tempPower.canFight.value;
    }

    return def.canFight;
}

static bool __attribute__((fastcall)) CrewMember_CanRepair(CrewMember *_this)
{
    bool req = !_this->intruder && !_this->bDead && _this->crewAnim->status != 3;

    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_REPAIR"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_REPAIR"));
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.canRepair.enabled)
    {
        return def.powerDef.tempPower.canRepair.value && req;
    }

    return def.canRepair && req;
}

static bool __attribute__((fastcall)) CrewMember_CanSabotage(CrewMember *_this)
{
    bool req = _this->intruder;

    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_SABOTAGE"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_SABOTAGE"));
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.canSabotage.enabled)
    {
        return def.powerDef.tempPower.canSabotage.value && req;
    }

    return def.canSabotage && req;
}

static bool __attribute__((fastcall)) CrewMember_CanMan(CrewMember *_this)
{
    bool req = !_this->intruder && _this->fStunTime == 0.f && _this->crewAnim->status != 3;
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_MAN"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_MAN"));
    }

    if (ex->temporaryPowerActive && def.powerDef.tempPower.canMan.enabled)
    {
        return def.powerDef.tempPower.canMan.value && req;
    }

    return def.canMan && req;
}


static bool __attribute__((fastcall)) CrewMember_CanBurn(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_BURN"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_BURN"));
    }
    return custom->GetDefinition(_this->species).canBurn;
}

static int __attribute__((fastcall)) CrewMember_GetMaxHealth(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto ex = CM_EX(_this);
    auto def = custom->GetDefinition(_this->species);

    float otherCrewStatMultiplier = 1.f;
    ShipManager *ship;
    if (_this->currentShipId == 0)
    {
        ship = G_->GetShipManager(0);
    }
    else
    {
        ship = G_->GetShipManager(1);
    }

    if (ship != nullptr)
    {
        for (auto i: ship->vCrewList)
        {
            if (i->iRoomId == _this->iRoomId && i != _this)
            {
                auto otherCrew = CM_EX(i);
                if (otherCrew->temporaryPowerActive && custom->GetDefinition(i->species).powerDef.tempPower.healthMultiplierAura.enabled)
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).powerDef.tempPower.healthMultiplierAura.value - 1;
                }
                else
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).healthMultiplierAura - 1;
                }
            }
        }
    }

    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("HEALTH_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("HEALTH_BOOST");
    }
    if (_this->GetShipObject()->HasAugmentation("FLAT_HEALTH_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_HEALTH_BOOST");
    }

    int newMaxHealth = (custom->GetDefinition(_this->species).maxHealth * augMultAmount + augAmount) * otherCrewStatMultiplier;
//    _this->health.first *= newMaxHealth / _this->health.second;
    return newMaxHealth;
}

static float __attribute__((fastcall)) CrewMember_GetMoveSpeedMultiplier(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    float otherCrewStatMultiplier = 1.f;
    ShipManager *ship;
    if (_this->currentShipId == 0)
    {
        ship = G_->GetShipManager(0);
    }
    else
    {
        ship = G_->GetShipManager(1);
    }

    if (ship != nullptr)
    {
        for (auto i: ship->vCrewList)
        {
            if (i->iRoomId == _this->iRoomId && i != _this)
            {
                auto otherCrew = CM_EX(i);
                if (otherCrew->temporaryPowerActive && custom->GetDefinition(i->species).powerDef.tempPower.moveSpeedMultiplierAura.enabled)
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).powerDef.tempPower.moveSpeedMultiplierAura.value - 1;
                }
                else
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).healthMultiplierAura - 1;
                }
            }
        }
    }

    auto ex = CM_EX(_this);
    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("SPEED_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("SPEED_BOOST");
    }
    if (_this->GetShipObject()->HasAugmentation("FLAT_SPEED_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_SPEED_BOOST");
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.moveSpeedMultiplier.enabled)
    {
        return (def.powerDef.tempPower.moveSpeedMultiplier.value * augMultAmount + augAmount) * otherCrewStatMultiplier;
    }

    return (def.moveSpeedMultiplier * augMultAmount + augAmount) * otherCrewStatMultiplier;
}

static float __attribute__((fastcall)) CrewMember_GetRepairSpeed(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    float otherCrewStatMultiplier = 1.f;
    ShipManager *ship;
    if (_this->currentShipId == 0)
    {
        ship = G_->GetShipManager(0);
    }
    else
    {
        ship = G_->GetShipManager(1);
    }

    if (ship != nullptr)
    {
        for (auto i: ship->vCrewList)
        {
            if (i->iRoomId == _this->iRoomId && i != _this)
            {
                auto otherCrew = CM_EX(i);
                if (otherCrew->temporaryPowerActive && custom->GetDefinition(i->species).powerDef.tempPower.repairSpeedAura.enabled)
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).powerDef.tempPower.repairSpeedAura.value - 1;
                }
                else
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).repairSpeedAura - 1;
                }
            }
        }
    }

    CrewMember_Extend* ex = CM_EX(_this);
    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("REPAIR_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("REPAIR_BOOST");
    }
    if (_this->GetShipObject()->HasAugmentation("FLAT_REPAIR_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_REPAIR_BOOST");
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.repairSpeed.enabled)
    {
        return ((def.powerDef.tempPower.repairSpeed.value * augMultAmount) + augAmount) * otherCrewStatMultiplier;
    }

    return (def.repairSpeed * augMultAmount + augAmount) * otherCrewStatMultiplier;
}

static float __attribute__((fastcall)) CrewMember_GetDamageMultiplier(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);

    float otherCrewStatMultiplier = 1.f;
    ShipManager *ship;
    if (_this->currentShipId == 0)
    {
        ship = G_->GetShipManager(0);
    }
    else
    {
        ship = G_->GetShipManager(1);
    }

    if (ship != nullptr)
    {
        for (auto i: ship->vCrewList)
        {
            if (i->iRoomId == _this->iRoomId && i != _this)
            {
                auto otherCrew = CM_EX(i);
                if (otherCrew->temporaryPowerActive && custom->GetDefinition(i->species).powerDef.tempPower.damageMultiplierAura.enabled)
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).powerDef.tempPower.damageMultiplierAura.value - 1;
                }
                else
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).damageMultiplierAura - 1;
                }
            }
        }
    }

    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("DAMAGE_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("DAMAGE_BOOST");
    }
    if (_this->GetShipObject()->HasAugmentation("FLAT_DAMAGE_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_DAMAGE_BOOST");
    }
    if (_this->crewAnim->status == 7)
    {
        return (def.rangedDamageMultiplier * augMultAmount * 10 + augAmount) * otherCrewStatMultiplier;
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.damageMultiplier.enabled)
    {
        if (_this->crewAnim->status == 7)
        {
            return (def.powerDef.tempPower.rangedDamageMultiplier.value * augMultAmount * 2 + augAmount) * otherCrewStatMultiplier;
        }
        return ((def.powerDef.tempPower.damageMultiplier.value * augMultAmount) + augAmount) * otherCrewStatMultiplier;
    }
    return (def.damageMultiplier * augMultAmount + augAmount) * otherCrewStatMultiplier;
}

static bool __attribute__((fastcall)) CrewMember_ProvidesPower(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    return custom->GetDefinition(_this->species).providesPower;
}

static float __attribute__((fastcall)) CrewMember_FireRepairMultiplier(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    float otherCrewStatMultiplier = 1.f;
    ShipManager *ship;
    if (_this->currentShipId == 0)
    {
        ship = G_->GetShipManager(0);
    }
    else
    {
        ship = G_->GetShipManager(1);
    }

    if (ship != nullptr)
    {
        for (auto i: ship->vCrewList)
        {
            if (i->iRoomId == _this->iRoomId && i != _this)
            {
                auto otherCrew = CM_EX(i);
                if (otherCrew->temporaryPowerActive && custom->GetDefinition(i->species).powerDef.tempPower.fireRepairMultiplierAura.enabled)
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).powerDef.tempPower.fireRepairMultiplierAura.value - 1;
                }
                else
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).fireRepairMultiplierAura - 1;
                }
            }
        }
    }

    CrewMember_Extend* ex = CM_EX(_this);
    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("FIRE_REPAIR_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("FIRE_REPAIR_BOOST");
    }
    if (_this->GetShipObject()->HasAugmentation("FLAT_FIRE_REPAIR_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_FIRE_REPAIR_BOOST");
    }
    return (custom->GetDefinition(_this->species).fireRepairMultiplier * augMultAmount + augAmount) * otherCrewStatMultiplier;
}

static bool __attribute__((fastcall)) CrewMember_IsTelepathic(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    auto ex = CM_EX(_this);
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_TELEPATHIC"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_TELEPATHIC"));
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.isTelepathic.enabled)
    {
        return def.powerDef.tempPower.isTelepathic.value;
    }

    return def.isTelepathic;
}

static float __attribute__((fastcall)) CrewMember_GetSuffocationModifier(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    float otherCrewStatMultiplier = 1.f;
    ShipManager *ship;
    if (_this->currentShipId == 0)
    {
        ship = G_->GetShipManager(0);
    }
    else
    {
        ship = G_->GetShipManager(1);
    }

    if (ship != nullptr)
    {
        for (auto i: ship->vCrewList)
        {
            if (i->iRoomId == _this->iRoomId && i != _this)
            {
                auto otherCrew = CM_EX(i);
                if (otherCrew->temporaryPowerActive && custom->GetDefinition(i->species).powerDef.tempPower.suffocationModifierAura.enabled)
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).powerDef.tempPower.suffocationModifierAura.value - 1;
                }
                else
                {
                    otherCrewStatMultiplier += custom->GetDefinition(i->species).suffocationModifierAura - 1;
                }
            }
        }
    }

    auto ex = CM_EX(_this);
    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("SUFFOCATION_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("SUFFOCATION_BOOST");
    }
    if (_this->GetShipObject()->HasAugmentation("FLAT_SUFFOCATION_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_SUFFOCATION_BOOST");
    }
    if (ex->temporaryPowerActive && def.powerDef.tempPower.suffocationModifier.enabled)
    {
        return (def.powerDef.tempPower.suffocationModifier.value * augMultAmount + augAmount) * otherCrewStatMultiplier;
    }

    return (custom->GetDefinition(_this->species).suffocationModifier * augMultAmount + augAmount) * otherCrewStatMultiplier;
}

static bool __attribute__((fastcall)) CrewMember_IsAnaerobic(CrewMember *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (_this->GetShipObject()->HasAugmentation("ALL_CREW_ANAEROBIC"))
    {
        return (bool)(_this->GetShipObject()->GetAugmentationValue("ALL_CREW_ANAEROBIC"));
    }
    return custom->GetDefinition(_this->species).isAnaerobic;
}

static bool __attribute__((fastcall)) CrewMember_HasSpecialPower(CrewMember *_this)
{
    auto ex = CM_EX(_this);

    return ex->hasSpecialPower;
}

static std::pair<float, float> __attribute__((fastcall)) CrewMember_GetPowerCooldown(CrewMember *_this)
{
    auto ex = CM_EX(_this);
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto def = custom->GetDefinition(_this->species);

    float augAmount = 0.f;
    float augMultAmount = 1.f;
    if (_this->GetShipObject()->HasAugmentation("COOLDOWN_BOOST"))
    {
        augMultAmount = _this->GetShipObject()->GetAugmentationValue("COOLDOWN_BOOST");
    }

    if (_this->GetShipObject()->HasAugmentation("FLAT_COOLDOWN_BOOST"))
    {
        augAmount = _this->GetShipObject()->GetAugmentationValue("FLAT_COOLDOWN_BOOST");
    }

    ex->powerCooldown.second = ex->powerCooldown.second * augMultAmount + augAmount;
    if (ex->powerCooldown.second < 0.5)
    {
        ex->powerCooldown.second = 0.5;
    }
    if (ex->powerCooldown.first > ex->powerCooldown.second)
    {
        ex->powerCooldown.first = ex->powerCooldown.second;
    }
    return ex->powerCooldown;
}

static bool __attribute__((fastcall)) CrewMember_PowerReady(CrewMember *_this)
{
    auto ex = CM_EX(_this);

    auto readyState = ex->PowerReady();

    return readyState == PowerReadyState::POWER_READY;
}

static void __attribute__((fastcall)) CrewMember_ResetPower(CrewMember *_this)
{
    auto ex = CM_EX(_this);

    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    auto jumpCooldown = custom->GetDefinition(_this->species).powerDef.jumpCooldown;

    if (jumpCooldown == ActivatedPowerDefinition::JUMP_COOLDOWN_FULL)
    {
        ex->powerCooldown.first = ex->powerCooldown.second;
    }
    else if (jumpCooldown == ActivatedPowerDefinition::JUMP_COOLDOWN_RESET)
    {
        ex->powerCooldown.first = 0;
    }

}

static void __attribute__((fastcall)) CrewMember_ActivatePower(CrewMember *_this)
{
    auto ex = CM_EX(_this);

    ex->PreparePower();
}

void SetupVTable(CrewMember *crew)
{
    void** vtable = *(void***)crew;

    DWORD dwOldProtect, dwBkup;
    VirtualProtect(&vtable[0], sizeof(void*) * 57, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    vtable[23] = (void*)&CrewMember_GetControllable;
    vtable[25] = (void*)&CrewMember_CanFight;
    vtable[26] = (void*)&CrewMember_CanRepair;
    vtable[27] = (void*)&CrewMember_CanSabotage;
    vtable[28] = (void*)&CrewMember_CanMan;
    vtable[31] = (void*)&CrewMember_CanSuffocate;
    vtable[32] = (void*)&CrewMember_CanBurn;
    vtable[33] = (void*)&CrewMember_GetMaxHealth;
    vtable[40] = (void*)&CrewMember_GetMoveSpeedMultiplier;
    vtable[41] = (void*)&CrewMember_GetRepairSpeed;
    vtable[42] = (void*)&CrewMember_GetDamageMultiplier;
    vtable[43] = (void*)&CrewMember_ProvidesPower;
    vtable[45] = (void*)&CrewMember_FireRepairMultiplier;
    vtable[46] = (void*)&CrewMember_IsTelepathic;
    vtable[47] = (void*)&CrewMember_GetPowerCooldown;
    vtable[48] = (void*)&CrewMember_PowerReady;
    vtable[49] = (void*)&CrewMember_ActivatePower;
    vtable[50] = (void*)&CrewMember_HasSpecialPower;
    vtable[51] = (void*)&CrewMember_ResetPower;
    vtable[52] = (void*)&CrewMember_GetSuffocationModifier;
    vtable[55] = (void*)&CrewMember_IsAnaerobic;

    VirtualProtect(&vtable[0], sizeof(void*) * 57, dwOldProtect, &dwBkup);
}



HOOK_METHOD_PRIORITY(CrewMember, constructor, 500, (CrewBlueprint& bp, int shipId, bool intruder, CrewAnimation* animation) -> void)
{
    super(bp, shipId, intruder, animation);

    CustomCrewManager *custom = CustomCrewManager::GetInstance();

    if (custom->IsRace(species) && !IsDrone())
    {
        SetupVTable(this);
        health.first = GetMaxHealth();
    }
}



static bool __attribute__((fastcall)) CrewAnimation_CustomDeath(CrewAnimation *_this)
{
    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (!custom->IsRace(_this->race)) return false;
    return custom->GetDefinition(_this->race).hasCustomDeathAnimation;
}

void SetupVTable(CrewAnimation *anim)
{
    void** vtable = *(void***)anim;

    DWORD dwOldProtect, dwBkup;
    VirtualProtect(&vtable[0], sizeof(void*) * 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    vtable[12] = (void*)&CrewAnimation_CustomDeath;

    VirtualProtect(&vtable[0], sizeof(void*) * 12, dwOldProtect, &dwBkup);
}



HOOK_METHOD_PRIORITY(CrewAnimation, constructor, 500, (int shipId, const std::string& race, Pointf unk, bool hostile) -> void)
{
    super(shipId, race, unk, hostile);

    CustomCrewManager *custom = CustomCrewManager::GetInstance();
    if (custom->IsRace(race) && !bDrone)
    {
        SetupVTable(this);
    }
}




