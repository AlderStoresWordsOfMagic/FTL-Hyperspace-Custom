#pragma once
#include "FTLGame.h"

struct ErosionEffect
{
    float systemDamageSpeed = 0.8f;
    float systemRepairMultiplier = 0.75f;
    int erosionTime = 10;
    std::string animation = "room_erosion";
};

struct CustomDamage
{
    int accuracyMod = 0;

    int erosionChance = 10;
    ErosionEffect erosionEffect;
};




class CustomDamageManager
{
public:
    static CustomDamage* currentWeaponDmg;
};
