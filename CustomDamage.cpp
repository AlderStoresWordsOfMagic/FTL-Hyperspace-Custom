#include "Global.h"
#include "Projectile_Extend.h"
#include "CustomWeapons.h"

CustomDamage* CustomDamageManager::currentWeaponDmg = nullptr;


HOOK_METHOD(ShipManager, GetDodgeFactor, () -> int)
{
    int ret = super();

    if (CustomDamageManager::currentWeaponDmg != nullptr)
    {
        ret -= CustomDamageManager::currentWeaponDmg->accuracyMod;
    }

    return ret;
}

HOOK_METHOD(ProjectileFactory, Update, () -> void)
{
    CustomDamageManager::currentWeaponDmg = &CustomWeaponManager::instance->GetWeaponDefinition(blueprint->name)->customDamage;

    super();

    CustomDamageManager::currentWeaponDmg = nullptr;
}

HOOK_METHOD(Projectile, Initialize, (WeaponBlueprint& bp) -> void)
{
    super(bp);

    auto customDmg = CustomDamageManager::currentWeaponDmg;

    if (customDmg != nullptr)
    {
        PR_EX(this)->customDamage = *customDmg;
    }
}

HOOK_METHOD(Projectile, CollisionCheck, (Collideable *other) -> void)
{
    CustomDamageManager::currentWeaponDmg = &PR_EX(this)->customDamage;

    super(other);

    CustomDamageManager::currentWeaponDmg = nullptr;
}
