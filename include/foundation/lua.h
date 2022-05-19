#pragma once

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

class Lua {
    public:
        Lua();
        ~Lua();
        [[nodiscard]] lua_State* GetState() const;
        void Run(const char* source);
        void SetFloat(const char* name, float value, bool globalApi = true);
    private:
        void SetGlobals();
        void LoadLibraries();
    private:
        lua_State* m_state;
        const char* m_globalTableName { "clua" };
};