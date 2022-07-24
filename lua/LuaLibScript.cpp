#include <sstream>
#include <vector>
#include <map>
#include "LuaLibScript.h"
#include "../Global.h"
#include "swigluarun.h"

static std::vector<LuaFunctionRef> m_on_load_callbacks;
static std::vector<LuaFunctionRef> m_on_init_callbacks;
static std::multimap<std::string, LuaFunctionRef> m_on_game_event_callbacks;
static std::multimap<std::string, LuaFunctionRef> m_on_game_event_loading_callbacks;
static std::multimap<InternalEvents::Identifiers, LuaFunctionRef> m_on_internal_event_callbacks; // TODO: For now we're only going to support `OnTick` and other potential methods with no argument requirements but we need to add support for methods with arguments later and add them to the call (or maybe add the information into the enum so at call time we can pass them?)
static std::multimap<RenderEvents::Identifiers, std::pair<LuaFunctionRef, LuaFunctionRef>> m_on_render_event_callbacks; // Holds before & after function ref in the pair

void LuaLibScript::LoadTypeInfo()
{
    types.pCrewMember = SWIG_TypeQuery(this->m_Lua, "CrewMember *");
}

int LuaLibScript::l_on_load(lua_State* lua)
{
    assert(lua_isfunction(lua, 1));
    LuaFunctionRef callbackReference = luaL_ref(lua, LUA_REGISTRYINDEX);
    m_on_load_callbacks.push_back(callbackReference);
    return 0;
}

void LuaLibScript::call_on_load_callbacks()
{
    lua_State* lua = this->m_Lua;
    // Load the callback by reference number
    printf("Fetching %u on_load callbacks\n", m_on_load_callbacks.size());
    for(auto i = m_on_load_callbacks.cbegin(); i != m_on_load_callbacks.cend(); ++i)
    {
        LuaFunctionRef refL = *i;
        lua_rawgeti(lua, LUA_REGISTRYINDEX, refL);
        if(lua_pcall(lua, 0, 0, 0) != 0) {
            hs_log_file("Failed to call the callback!\n %s\n", lua_tostring(lua, -1));
            lua_pop(lua, 1);
            return;
        }
    }
}

int LuaLibScript::l_on_init(lua_State* lua)
{
    assert(lua_isfunction(lua, 1));
    LuaFunctionRef callbackReference = luaL_ref(lua, LUA_REGISTRYINDEX);
    m_on_init_callbacks.push_back(callbackReference);
    return 0;
}

void LuaLibScript::call_on_init_callbacks()
{
    lua_State* lua = this->m_Lua;
    // Load the callback by reference number
    printf("Fetching %u on_init callbacks\n", m_on_init_callbacks.size());
    for(auto i = m_on_init_callbacks.cbegin(); i != m_on_init_callbacks.cend(); ++i)
    {
        LuaFunctionRef refL = *i;
        lua_rawgeti(lua, LUA_REGISTRYINDEX, refL);
        if(lua_pcall(lua, 0, 0, 0) != 0) {
            hs_log_file("Failed to call the callback!\n %s\n", lua_tostring(lua, -1));
            lua_pop(lua, 1);
            return;
        }
    }
}

int LuaLibScript::l_on_game_event(lua_State* lua)
{
    assert(lua_isstring(lua, 1));
    assert(lua_isboolean(lua, 2));
    assert(lua_isfunction(lua, 3));
    const std::string eventName = std::string(lua_tostring(lua, 1));
    const bool onLoad = lua_toboolean(lua, 2);
    lua_pushvalue(lua, 3);
    LuaFunctionRef callbackReference = luaL_ref(lua, LUA_REGISTRYINDEX); // Not sure if maybe we can move this above the tostring & toboolean? Since it pops off the Lua stack anyways rather than doing the pushvalue above this.
    if(onLoad)
        m_on_game_event_loading_callbacks.emplace(eventName, callbackReference);
    else
        m_on_game_event_callbacks.emplace(eventName, callbackReference);

    return 0;
}

// TODO: Maybe tell the compiler to always inline this
void call_all_functions_from_multimap(lua_State* lua, std::multimap<std::string, LuaFunctionRef> mmap, std::string key)
{
//    if(mmap.count(key) == 0)
//        return; // No registered callbacks

    printf("Fetching %u on_game_event callbacks for %s\n", mmap.count(key), key.c_str()); // TODO: Remove or add to debugging logs?
    for(std::pair<std::multimap<std::string, LuaFunctionRef>::iterator, std::multimap<std::string, LuaFunctionRef>::iterator> range(mmap.equal_range(key)); range.first != range.second; ++range.first)
    {
        LuaFunctionRef refL = range.first->second;
        lua_rawgeti(lua, LUA_REGISTRYINDEX, refL);
        if(lua_pcall(lua, 0, 0, 0) != 0) {
            hs_log_file("Failed to call the callback for %s!\n %s\n", key.c_str(), lua_tostring(lua, -1));
            lua_pop(lua, 1);
            return;
        }
    }
}

void LuaLibScript::call_on_game_event_callbacks(std::string eventName, bool isLoading)
{
    call_all_functions_from_multimap(this->m_Lua, isLoading ? m_on_game_event_loading_callbacks : m_on_game_event_callbacks, eventName);
}

int LuaLibScript::l_on_render_event(lua_State* lua)
{
    assert(lua_isinteger(lua, 1));
    assert(lua_isfunction(lua, 2));
    assert(lua_isfunction(lua, 3));
    const int callbackHookId = lua_tointeger(lua, 1);
    assert(callbackHookId > RenderEvents::UNKNOWN); // TODO: Print a nice pretty message to the logs maybe if event was not a known value?
    assert(callbackHookId < RenderEvents::UNKNOWN_MAX);
    lua_pushvalue(lua, 2);
    LuaFunctionRef callbackBeforeRef = luaL_ref(lua, LUA_REGISTRYINDEX);
    lua_pushvalue(lua, 3);
    LuaFunctionRef callbackAfterRef = luaL_ref(lua, LUA_REGISTRYINDEX);
    RenderEvents::Identifiers id = static_cast<RenderEvents::Identifiers>(callbackHookId);
    m_on_render_event_callbacks.emplace(id, std::pair<LuaFunctionRef, LuaFunctionRef>(callbackBeforeRef, callbackAfterRef));

    return 0;
}

void LuaLibScript::call_on_render_event_pre_callbacks(RenderEvents::Identifiers id)
{
    assert(id > RenderEvents::UNKNOWN);
    assert(id < RenderEvents::UNKNOWN_MAX);

//    if(m_on_render_event_callbacks.count(id) == 0)
//        return; // No registered callbacks

    for(std::pair<std::multimap<RenderEvents::Identifiers, std::pair<LuaFunctionRef, LuaFunctionRef>>::iterator, std::multimap<RenderEvents::Identifiers, std::pair<LuaFunctionRef, LuaFunctionRef>>::iterator> range(m_on_render_event_callbacks.equal_range(id)); range.first != range.second; ++range.first)
    {
        std::pair<LuaFunctionRef, LuaFunctionRef> beforeAndAfterCallbacksRefL = range.first->second;
        lua_rawgeti(this->m_Lua, LUA_REGISTRYINDEX, beforeAndAfterCallbacksRefL.first);
        if(lua_pcall(this->m_Lua, 0, 0, 0) != 0) {
            hs_log_file("Failed to call the before callback for RenderEvent %u!\n %s\n", id, lua_tostring(this->m_Lua, -1)); // TODO: Maybe map RenderEvents to a readable string also?
            lua_pop(this->m_Lua, 1);
            return;
        }
    }
}
void LuaLibScript::call_on_render_event_post_callbacks(RenderEvents::Identifiers id)
{
    assert(id > RenderEvents::UNKNOWN);
    assert(id < RenderEvents::UNKNOWN_MAX);

//    if(m_on_render_event_callbacks.count(id) == 0)
//        return; // No registered callbacks

    for(std::pair<std::multimap<RenderEvents::Identifiers, std::pair<LuaFunctionRef, LuaFunctionRef>>::iterator, std::multimap<RenderEvents::Identifiers, std::pair<LuaFunctionRef, LuaFunctionRef>>::iterator> range(m_on_render_event_callbacks.equal_range(id)); range.first != range.second; ++range.first)
    {
        std::pair<LuaFunctionRef, LuaFunctionRef> beforeAndAfterCallbacksRefL = range.first->second;
        lua_rawgeti(this->m_Lua, LUA_REGISTRYINDEX, beforeAndAfterCallbacksRefL.second);
        if(lua_pcall(this->m_Lua, 0, 0, 0) != 0) {
            hs_log_file("Failed to call the after callback for RenderEvent %u!\n %s\n", id, lua_tostring(this->m_Lua, -1));
            lua_pop(this->m_Lua, 1);
            return;
        }
    }
}

int LuaLibScript::l_on_internal_event(lua_State* lua)
{
    assert(lua_isinteger(lua, 1));
    assert(lua_isfunction(lua, 2));
    const int callbackHookId = lua_tointeger(lua, 1);
    assert(callbackHookId > InternalEvents::UNKNOWN); // TODO: Print a nice pretty message to the logs maybe if event was not a known value?
    assert(callbackHookId < InternalEvents::UNKNOWN_MAX);
    lua_pushvalue(lua, 2);
    LuaFunctionRef callbackReference = luaL_ref(lua, LUA_REGISTRYINDEX);

    /*
    // TODO: Check that the number of arguments needed matches those for the internal event
    lua_Debug ar;
    lua_rawgeti(lua, LUA_REGISTRYINDEX, callbackReference);
    lua_getinfo(lua, ">u", &ar);
    printf("Registered Lua Function: that accepts '%u' arguments and is variable arguments: %s\n", ar.nparams, ar.isvararg ? "TRUE" : "FALSE");
    */

    InternalEvents::Identifiers id = static_cast<InternalEvents::Identifiers>(callbackHookId);
    m_on_internal_event_callbacks.emplace(id, callbackReference);

    return 0;
}

// TODO: This might need to be a varargs in the future to allow calling with the arguments from the hook & also passing that infromation via the enum so we can check at registration time above if the function has the correct number of arguments and if the correct number were passed here.
int LuaLibScript::call_on_internal_event_callbacks(InternalEvents::Identifiers id, int nArg, int nRet)
{
    assert(id > InternalEvents::UNKNOWN);
    assert(id < InternalEvents::UNKNOWN_MAX);

//    if(m_on_internal_event_callbacks.count(id) == 0)
//        return; // No registered callbacks

    for(std::pair<std::multimap<InternalEvents::Identifiers, LuaFunctionRef>::iterator, std::multimap<InternalEvents::Identifiers, LuaFunctionRef>::iterator> range(m_on_internal_event_callbacks.equal_range(id)); range.first != range.second; ++range.first)
    {
        LuaFunctionRef refL = range.first->second;
        lua_rawgeti(this->m_Lua, LUA_REGISTRYINDEX, refL);
        for (int i=0; i<nArg; ++i)
        {
            lua_pushvalue(this->m_Lua, -1-nArg);
        }
        if(lua_pcall(this->m_Lua, nArg, 0, 0) != 0) {
            hs_log_file("Failed to call the callback for InternalEvent %u!\n %s\n", id, lua_tostring(this->m_Lua, -1)); // Also TODO: Maybe map RenderEvents to a readable string also?
            lua_pop(this->m_Lua, 1);
            continue;
        }
        if (nRet > 0)
        {
            if (!lua_isnil(this->m_Lua, -nRet))
            {
                return nRet;
            }
            // If the return value is nil then we pop them before trying the next callback function
            lua_pop(this->m_Lua, nRet);
        }
    }
    return 0;
}


// TODO: This hook to kick it off, could potentially move if needed? Or maybe we should of done a singleton pattern initialized by LuaScriptInit instead of passing the object back to LuaScriptInit and getting it from the global context there?
HOOK_METHOD(AchievementTracker, LoadAchievementDescriptions, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> AchievementTracker::LoadAchievementDescriptions -> Begin (LuaLibScript.cpp)\n")
    super();
    Global::GetInstance()->getLuaContext()->getLibScript()->call_on_load_callbacks();
}

HOOK_METHOD(ScoreKeeper, LoadGame, (int fh) -> void)
{
    LOG_HOOK("HOOK_METHOD -> ScoreKeeper::LoadGame -> Begin (LuaLibScript.cpp)\n")
    super(fh);
    // TODO: Probably need LoadGame, Opening the ship editor & game restart hooks to all call this
    // TODO: Or maybe we can use StarMap::NewGame ?
    // TODO: Or maybe WorldManager::StartGame or WorldManager::CreateNewGame? (Note, WOrldManager::CreateNewGame might be ideal)
    Global::GetInstance()->getLuaContext()->getLibScript()->call_on_init_callbacks();
}

HOOK_METHOD(WorldManager, UpdateLocation, (LocationEvent* locationEvent) -> void)
{
    LOG_HOOK("HOOK_METHOD -> WorldManager::UpdateLocation -> Begin (LuaLibScript.cpp)\n")
    printf("UpdateLocation - LocationEvent Seen, name: %s\n", locationEvent->eventName.c_str());
    Global::GetInstance()->getLuaContext()->getLibScript()->call_on_game_event_callbacks(locationEvent->eventName.c_str(), false);
    super(locationEvent);
}

HOOK_METHOD(WorldManager, CreateLocation, (Location* location) -> void)
{
    LOG_HOOK("HOOK_METHOD -> WorldManager::CreateLocation -> Begin (LuaLibScript.cpp)\n")

    std::string eventName;

    if (location->boss)
    {
        std::stringstream s;
        s << "BOSS_TEXT_" << bossShip->nextStage;

        eventName = s.str();
    }
    else
    {
        eventName = location->event->eventName;
    }

    printf("CreateLocation - LocationEvent Seen, name: %s\n", eventName.c_str());
    Global::GetInstance()->getLuaContext()->getLibScript()->call_on_game_event_callbacks(eventName.c_str(), bLoadingGame);
    super(location);
   // TODO: Also call lua scripting for entering a location? (In addition to when the event is loaded) Maybe this is like Events.OnLocation and the other is the special LocationEvent that takes a name to hook to.
}

static const struct luaL_Reg slib_funcs[] = {
   { "on_load", LuaLibScript::l_on_load },
   { "on_init", LuaLibScript::l_on_init },
   { "on_game_event", LuaLibScript::l_on_game_event },
   { "on_render_event", LuaLibScript::l_on_render_event },
   { "on_internal_event", LuaLibScript::l_on_internal_event },
   { NULL, NULL }
};

static int luaopen_scriptlib(lua_State* lua)
{
   luaL_newlib(lua, slib_funcs);
   return 1;
}

LuaLibScript::LuaLibScript(lua_State* lua)
{
   this->m_Lua = lua;
   luaL_requiref(lua, "script", luaopen_scriptlib, 1);
}
