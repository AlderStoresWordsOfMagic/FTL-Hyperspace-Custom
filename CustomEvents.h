#include "Global.h"
#include <algorithm>

struct BeaconType
{
    std::string eventName;
    GL_Color color;
    bool global = false;
    TextString beaconText;
    TextString undiscoveredTooltip;
    TextString unvisitedTooltip;
    TextString visitedTooltip;
};

struct CustomEvent
{
    std::string eventName;
    BeaconType *beacon;
    bool hasCustomBeacon = false;
    bool checkCargo = false;
    bool recursive = true;
    bool preventQuest = false;
    bool noQuestText = false;
    bool removeHazards = false;
    bool removeNebula = false;
    std::string secretSectorWarp = "";

    bool win = false;
    std::string gameOverText = "";

    std::vector<std::string> hiddenAugs = std::vector<std::string>();
};

struct SectorExit
{
    std::string event = "";
    std::string rebelEvent = "";
    std::string nebulaEvent = "";
};

struct SectorFleet
{
    std::string event = "";
    std::string nebulaEvent = "";
};

struct CustomSector
{
    std::string sectorName;
    SectorExit exitBeacons;
    SectorFleet fleetBeacons;
    bool removeFirstBeaconNebula = false;
};

struct BossShipDefinition
{
    std::string shipId;
    int yOffset = 120;
};

class CustomEventsParser
{
public:
    CustomEventsParser()
    {

    }

    void ParseCustomEventNode(rapidxml::xml_node<char> *node);

    static CustomEventsParser *GetInstance()
    {
        return instance;
    }

    BossShipDefinition *GetBossShipDefinition(const std::string& shipId)
    {
        if (bossShipIds.find(shipId) != bossShipIds.end())
        {
            return &bossShipIds[shipId];
        }

        return nullptr;
    }

    CustomEvent *GetCustomEvent(const std::string& event);
    CustomSector *GetCustomSector(const std::string& sectorName);

    static std::string GetBaseEventName(const std::string& event)
    {
        if (event.find(' ') == std::string::npos)
        {
            return event;
        }
        else
        {
            return event.substr(0, event.find(' '));
        }
    }

    std::vector<std::string> eventFiles;


private:
    std::vector<CustomSector*> customSectors;
    std::unordered_map<std::string, CustomEvent*> customEvents;
    std::unordered_map<std::string, BossShipDefinition> bossShipIds;
    static CustomEventsParser *instance;
};
