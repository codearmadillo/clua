#pragma once
#include <functional>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

enum LuaAssertArguments {
    LUA_ARGS_EXACT = 0,
    LUA_ARGS_AT_LEAST = 1,
    LUA_ARGS_AT_MOST = 2
};

class Lua {
    public:
        static Lua& getInstance() {
            static Lua instance;
            return instance;
        }
        Lua(Lua const&)         = delete;
        void operator=(Lua const&)  = delete;

        Lua& dump();
        static void dump(lua_State* lua);
        /**
         * Binds nested member to Lua state
         * @param name
         * @return
         */
        Lua& bind(const char* name);
        /**
         * Binds value on top of the stack, to table which sits below the value on top of that stack. The bound value
         * is cleared and target table becomes the top of the stack
         * @param name
         * @param lua
         */
        static void bind(const char* name, lua_State* lua);
        /**
         * Obtains nested member and pushes it on top of stack if it exists, otherwise pushes nil on top of stack
         * @param name
         * @return Status flag indicating whether or not was member pushed
         */
        Lua& get(const char* name);
        /**
         * Obtains nested member from table on top of the stack, and pushes it on top if it exists
         * @param name
         * @param lua
         */
        static void get(const char* name, lua_State* lua);
        Lua& call(const char* script, int nargs = 0, int nresults = 0);
        static void call(const char* script, lua_State* lua, int nargs = 0, int nresults = 0);
        Lua& pcall(const char* script, int nargs = 0, int nresults = 0);
        static void pcall(const char* script, lua_State* lua, int nargs = 0, int nresults = 0);
        /**
         * Calls a method currently sitting on top of stack
         * @param nargs
         * @param nresults
         * @return
         */
        Lua& pcall(int nargs = 0, int nresults = 0);
        static void pcall(lua_State* lua, int nargs = 0, int nresults = 0);

        bool load_file(const char* path, const char* mode = "bt");
        Lua& load_string(const char* script);
        lua_State* rawState() { return m_state; }

        /** Push methods */
        Lua& push();
        Lua& pushtable();
        Lua& pushvalue(const int& index);
        Lua& push(const double& n);
        Lua& push(const float& n);
        Lua& push(const int& n);
        Lua& push(const char* n);
        Lua& push(const bool& n);
        Lua& push(int (*c)(lua_State*));

        /** Static Push methods */
        static void push(lua_State* lua);
        static void pushtable(lua_State* lua);
        static void pushvalue(const int& index, lua_State* lua);
        static void push(const double& n, lua_State* lua);
        static void push(const float& n, lua_State* lua);
        static void push(const int& n, lua_State* lua);
        static void push(const char* n, lua_State* lua);
        static void push(const bool& n, lua_State* lua);
        static void push(int (*c)(lua_State*), lua_State* lua);

        void static assertArguments(lua_State* lua, int n, LuaAssertArguments mode);
    private:
        Lua();
        ~Lua();
    private:
        lua_State* m_state;
        const char* m_libName { "clua" };
};