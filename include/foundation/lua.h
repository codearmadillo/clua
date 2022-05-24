#pragma once

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

class Lua {
    public:
        static Lua& getInstance() {
            static Lua instance;
            return instance;
        }
        void dump();
        void bind(const char* name);
        void call(const char* script, int nargs = 0, int nresults = 0);
        void pcall(const char* script, int nargs = 0, int nresults = 0);
        lua_State* rawState() { return m_state; }
    private:
        Lua();
        ~Lua();
    private:
        lua_State* m_state;
        const char* m_libName { "clua" };
    public:
        Lua(Lua const&) = delete;
        void operator=(Lua const&) = delete;
};