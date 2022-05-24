#pragma once
#include <functional>

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
        Lua& dump();
        Lua& bind(const char* name);
        Lua& call(const char* script, int nargs = 0, int nresults = 0);
        Lua& pcall(const char* script, int nargs = 0, int nresults = 0);
        lua_State* rawState() { return m_state; }

        /** Push methods */
        Lua& push();
        Lua& pushvalue(const int& index);
        Lua& push(const float& n);
        Lua& push(const int& n);
        Lua& push(const char* n);
        Lua& push(const bool& n);
        Lua& push(int (*c)(lua_State*));
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