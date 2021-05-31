#include "Global.h"

std::vector<CrewMember*> TeleportCrewShip(ShipManager *ship, int roomId, bool intruders)
{
    std::vector<CrewMember*> leavingCrewList = std::vector<CrewMember*>();

    int teleCounter = 0;

    for (auto i : ship->vCrewList)
    {
        if (roomId != i->iRoomId)
        {
            continue;
        }

        if (i->NeedsIntruderSlot() == intruders && i->CanTeleport())
        {
            i->StartTeleport();
            leavingCrewList.push_back(i);

            teleCounter++;
        }
    }

    return leavingCrewList;
}


HOOK_METHOD(CompleteShip, InitiateTeleport, (int targetRoom, int command) -> void)
{
    if (!arrivingParty.empty() || !leavingParty.empty() || command == 0 ||
        enemyShip == nullptr || enemyShip->shipManager == nullptr || enemyShip->shipManager->bJumping ||
        shipManager == nullptr || shipManager->bJumping)
    {
        return;
    }

    int actualRoom = targetRoom;

    if (targetRoom == -1)
    {
        actualRoom = random32() % ShipGraph::GetShipInfo(enemyShip->shipManager->iShipId)->RoomCount();
    }

    teleTargetRoom = actualRoom;

    int teleSysRoom = shipManager->GetSystemRoom(9); // Change to SYS_TELEPORTER once system stuff is merged

    if (command == 1)
    {
        leavingParty = TeleportCrewShip(shipManager, teleSysRoom, false);
    }
    if (command == 2)
    {
        arrivingParty = TeleportCrewShip(enemyShip->shipManager, teleTargetRoom, true);
    }

    if (!leavingParty.empty() || !arrivingParty.empty())
    {
        shipManager->teleportSystem->InitiateTeleport();
        G_->GetSoundControl()->PlaySoundMix("teleport", -1.f, false);

        if (shipManager->HasEquipment("TELEPORT_HEAL"))
        {
            float value = shipManager->GetAugmentationValue("TELEPORT_HEAL");

            for (auto i : leavingParty)
            {
                i->DirectModifyHealth(value * i->health.second);
            }
            for (auto i : arrivingParty)
            {
                i->DirectModifyHealth(value * i->health.second);
            }
        }
    }
}

/*
HOOK_STATIC(ShipManager, TeleportCrew, (std::vector<CrewMember*>& leavingCrewList, ShipManager *ship, int roomId, bool intruders) -> void)
{
    leavingCrewList = std::vector<CrewMember*>();

    int counter = 0;

    for (auto i : ship->vCrewList)
    {
        if (roomId != i->iRoomId)
        {
            continue;
        }

        if (i->NeedsIntruderSlot() == intruders && i->CanTeleport())
        {
            i->StartTeleport();
            leavingCrewList.push_back(i);

            counter++;

            if (counter >= 4)
            {
                return;
            }
        }
    }
}
*/
