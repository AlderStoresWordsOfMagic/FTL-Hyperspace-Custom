#include "ShipManager_Extend.h"


HOOK_METHOD_PRIORITY(ShipManager, constructor, 900, (int iShipId) -> void)
{
    LOG_HOOK("HOOK_METHOD_PRIORITY -> ShipManager::constructor -> Begin (ShipManager_Extend.cpp)\n")


	super(iShipId);

	auto ex = new ShipManager_Extend();

    uint32_t dEx = (uint32_t)ex;

	this->gap_ex_1[0] = (dEx >> 24) & 0xFF;
	this->gap_ex_1[1] = (dEx >> 16) & 0xFF;
	this->gap_ex_2[0] = (dEx >> 8) & 0xFF;
	this->gap_ex_2[1] = dEx & 0xFF;
	ex->orig = this;
}

HOOK_METHOD(ShipManager, destructor, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> ShipManager::destructor -> Begin (ShipManager_Extend.cpp)\n")
    delete SM_EX(this);

    return super();
}

ShipManager_Extend* Get_ShipManager_Extend(ShipManager* c)
{
    if (!c) return nullptr;

    uint32_t dEx = 0;

    dEx <<= 8;
    dEx |= c->gap_ex_1[0];
    dEx <<= 8;
    dEx |= c->gap_ex_1[1];
    dEx <<= 8;
    dEx |= c->gap_ex_2[0];
    dEx <<= 8;
    dEx |= c->gap_ex_2[1];

    return (ShipManager_Extend*)dEx;
}
