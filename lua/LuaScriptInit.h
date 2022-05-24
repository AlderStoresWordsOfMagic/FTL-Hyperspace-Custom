#ifndef LUASCRIPTINIT_H
#define LUASCRIPTINIT_H

#include "luaInclude.h"
#include "luaDefines.h"
#include "LuaLibScript.h"

class LuaScriptInit
{
    public:
        /** Default constructor */
        LuaScriptInit();
        /** Default destructor */
        virtual ~LuaScriptInit();

        /** Access m_Lua
         * \return The current value of m_Lua
         */
        lua_State* GetLua() { return m_Lua; }
        /** Set m_Lua
         * \param val New value to set
         */
        void SetLua(lua_State* val) { m_Lua = val; }
        
        LuaLibScript* getLibScript() { return m_libScript; };

    protected:

    private:
        lua_State* m_Lua; //!< Member variable "m_Lua"
        LuaLibScript* m_libScript;
};

#endif // LUASCRIPTINIT_H
