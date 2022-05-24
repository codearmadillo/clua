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
        Lua(Lua const&)         = delete;
        void operator=(Lua const&)  = delete;

        Lua& dump();
        /**
         * Binds nested member to Lua state
         * @param name
         * @return
         */
        Lua& bind(const char* name);
        /**
         * Obtains nested member and pushes it on top of stack if it exists, otherwise pushes nil on top of stack
         * @param name
         * @return Status flag indicating whether or not was member pushed
         */
        Lua& get(const char* name);
        Lua& call(const char* script, int nargs = 0, int nresults = 0);
        Lua& pcall(const char* script, int nargs = 0, int nresults = 0);
        /**
         * Calls a method currently sitting on top of stack
         * @param nargs
         * @param nresults
         * @return
         */
        Lua& pcall(int nargs = 0, int nresults = 0);
        Lua& load_string(const char* script);
        lua_State* rawState() { return m_state; }

        /** Push methods */
        Lua& push();
        Lua& pushvalue(const int& index);
        Lua& push(const double& n);
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
};