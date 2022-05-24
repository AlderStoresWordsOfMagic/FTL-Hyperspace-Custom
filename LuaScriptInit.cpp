#include "LuaScriptInit.h"
#include <string>
#include <assert.h>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "Global.h"

/*
    TODO Stuff
    - Have a `global_run` (or `global`) table that contents is saved with the run (functions not allowed to be inserted into the table, data only)
    - Have a `profile` (or `global_profile`) table that contents is saved across runs (functions not allowed, data only)
    - Add serpent library to allow easy printing of Lua tables with `serpent.block()` for debugging for developers of Lua scripts
    - Add a print function to write messages to the in-game screen like a chat/console
    - Add console LUA execution (single strings)
    - Implement table_size() like Factorio does for tables with non-contiguous keys.?
    - Maybe add abilitly to pull localized string?
    - Re-implement require to load only .lua files from the dat, or maybe re-enable lua package (which contains require) and figure out how to restrict it.
    - Re-implement math.random to use FTL's random.
    - Add script table
    - Add script.on_init( (function callback/registration) for functions to run on a new run
    - Add script.on_load( (function callback) for functions to run upon loading the script, this is before the game is loaded or a run is started
    - Add script.on_event(eventEnum, (function callback)) for functions to run upon a defined event (you cannot stop the event but hopefully we can make the code wait for your code to finish and it not being async, maybe we can add an on_async_event in the future)
    - Add defines table
    - Add defines.events table
    - Add defines.events.on_tick and some other events.
    - Maybe add a story system like https://wiki.factorio.com/Tutorial:Scripting#Story_script ?
    
*/

std::string luaTest = "print(\"hello\")\nfunction helloWorld()\n\tprint(\"Hello World\")\nend\nlog(\"LUA MESSAGE\")\n";

void removeDangerousStuff(lua_State* lua)
{
/*
    lua_getglobal(lua, "os");
    lua_pushnil(lua);
    lua_setfield(lua, -2, "execute");
    lua_pushnil(lua);
    lua_setfield(lua, -2, "rename");
    lua_pushnil(lua);
    lua_setfield(lua, -2, "remove");
    lua_pushnil(lua);
    lua_setfield(lua, -2, "exit");
    lua_pop(lua, 1);
*/
    
    
    lua_getglobal(lua, "math");
    lua_pushnil(lua);
    lua_setfield(lua, -2, "randomseed");
    lua_pushnil(lua);
    lua_setfield(lua, -2, "random");
    lua_pop(lua, 1); // remove math table from stack
}
    /* TODO: Replace math.random with call to FTL's random32
    math.random ([m [, n]])
When called without arguments, returns a pseudo-random float with uniform distribution in the range [0,1). When called with two integers m and n, math.random returns a pseudo-random integer with uniform distribution in the range [m, n]. (The value n-m cannot be negative and must fit in a Lua integer.) The call math.random(n) is equivalent to math.random(1,n).

This function is an interface to the underling pseudo-random generator function provided by C.
*/
 
 /** @defgroup luaglobal LuaGlobals
  */
 
 /** General Global Lua Script functions
  * @ingroup luaglobal
  */
 class Script
 {
 }

/** @addtogroup luaglobal
 *
 * @{
 */
 
/** Do logging
 */
auto hs_lua_log2 (lua_State* lua) -> int {}

#if DOXYGEN_ONLY
/** Log a message to the Hyperspace log file & standard out
 * @param msg Message string you would like to log
 */
static void log(char* msg) {}
#endif // DOXYGEN_ONLY

static int printMessage(lua_State* lua)
{
    assert(lua_isstring(lua, 1));
    const char* msg = lua_tostring(lua, 1);
    const char* logMessage = boost::str(boost::format("[Lua]: %s\n") % msg).c_str();
    printf(logMessage);
    hs_log_file(logMessage);
    return 0;
}
/** @} */

/** Initialize Lua Scripting
 */
LuaScriptInit::LuaScriptInit()
{
    printf("Initializing Lua\n");
    lua_State* lua = luaL_newstate(); // Open Lua
    this->SetLua(lua);
    luaL_openlibs(lua); // Load Lua libraries (we restricted what libraries can load in linit.c)
    removeDangerousStuff(lua);
    // TODO: Figure out how to unload and/or replace certain library functions (like math.random!, threads! & other things unsafe for a single-threaded deterministic environment)
    
    lua_pushcfunction(lua, printMessage);
    lua_setglobal(lua, "log");

    printf("Loading Lua string\n");
    int iErr = 0;
    if((iErr = luaL_loadbuffer(lua, luaTest.c_str(), luaTest.size(), "=luaTest")) != 0)
    {
        printf("Lua loadbuffer error\n");
    }
    else
    {    
        // call Lua's Main from the script (note: still need to load script before this)
        if((iErr = lua_pcall(lua, 0, LUA_MULTRET, 0)) == 0)
        {
            lua_getglobal(lua, "helloWorld");
            lua_pcall(lua, 0, 0, 0); // Call function (pop's from stack name + arguments but we say no arguments and no returns here)
        }
        else
        {
            printf("Lua pcall error\n");
            printf(lua_tostring(lua, -1));
            printf("\n");
            lua_pop(lua, 1);
        }
    }
    printf("Lua initialized!\n");
}

LuaScriptInit::~LuaScriptInit()
{
    printf("Lua destruct\n");
    lua_State* lua = this->GetLua();
    lua_close(lua);
    this->SetLua(nullptr);
    printf("Lua destroyed\n");
}

// TODO: Math.random must be reimplemented to be based on FTL's internal random state/seed
// TODO: See other things Factorio decided to disable in Lua (like threads) because they're a problem for determinism
// TODO: Disable os.execute & io.write & other potentially dangerous interactions with the system.

/*
int LuaState::DoString(const std::string &str, bool protect)
{
	g_CurrentLua = this;

	int err = luaL_loadbuffer(this, str.c_str(), str.length(), "=lua_run");
	if(err != 0)
	{
		if(err > 0)
		{
			PrintError(lua_tostring(this, -1));
			lua_pop(this, 1);
		}
		return err;
	}

	if(protect)
	{
		if(err = lua_pcall(this, 0, LUA_MULTRET, 0))
		{
			PrintError(lua_tostring(this, -1));
			lua_pop(this, 1);
			return err;
		}
	}
	else
		lua_call(this, 0, LUA_MULTRET);

	return 0;
}
*/
