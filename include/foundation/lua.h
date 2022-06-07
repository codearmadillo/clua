#pragma once
#include <functional>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

/**
 * Lua Types
 * LUA_TNIL (0)
 * LUA_TNUMBER
 * LUA_TBOOLEAN
 * LUA_TSTRING
 * LUA_TTABLE
 * LUA_TFUNCTION
 * LUA_TUSERDATA
 * LUA_TTHREAD
 * LUA_TLIGHTUSERDATA
 */

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

        /**
         * Dumps current Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void dump_stack(lua_State* luaState = nullptr);

        /**
         * Gets information about provided stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void get_stack(lua_State* luaState = nullptr);

        /**
         * Gets a value from table on top of the stack, and sets it as new top of the stack (-1|+1)
         * @param name Scoped name
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void get(const char* name, lua_State* luaState = nullptr);

        /**
         * Gets a value from global variable, and sets it as new top of the stack (+1)
         * @param name Scoped name
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void get_global(const char* name, lua_State* luaState = nullptr);

        /**
         * Checks if value exists in global scope
         * @param name Scoped name
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static bool exists_global(const char* name, lua_State* luaState = nullptr);

        /**
         * Checks if value exists in table on top of the stack
         * @param name Scoped name
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static bool exists(const char* name, lua_State* luaState = nullptr);

        /**
         * Adds value on top of the stack to table which is under the value on the same stack (-2)
         * @param name Scoped name
         * @param clearTable If true, the table is removed from the stack when finished
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void set(const char* name, bool clearTable = true, lua_State* luaState = nullptr);

        /**
         * Adds value on top of the stack to global variable in Lua state (-1)
         * @param name Scoped name
         * @param clearTable If true, the table is removed from the stack when finished
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void set_global(const char* name, bool clearTable = true, lua_State* luaState = nullptr);

        /**
         * Calls a provided script
         * @param script Script to call
         * @param nargs Number of arguments
         * @param nresults Number of expected returned values
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void call(const char* script, int nargs = 0, int nresults = 0, lua_State* luaState = nullptr);

        /**
         * Calls a provided script in protected mode
         * @param script Script to call
         * @param nargs Number of arguments
         * @param nresults Number of expected returned values
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void pcall(const char* script, int nargs = 0, int nresults = 0, lua_State* luaState = nullptr);

        /**
         * Calls a script on top of the stack in protected mode
         * @param nargs Number of arguments
         * @param nresults Number of expected returned values
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void pcall(int nargs = 0, int nresults = 0, lua_State* luaState = nullptr);

        /**
         * Loads a file from path and pushes the result on top of Lua stack
         * @param path File path
         * @param mode File mode
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static bool load_file(const char* path, const char* mode = "bt", lua_State* luaState = nullptr);

        /**
         * Loads a string on top of the stack
         * @param string Script to load
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void load_string(const char* string, lua_State* luaState = nullptr);


        /**
         * Pushes a nil value on top of Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void pushnil(lua_State* luaState = nullptr);

        /**
         * Pushes a new table on top of Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void pushtable(lua_State* luaState = nullptr);

        /**
         * Pushes a copy of the element at the given index onto the stack.
         * @param index Index of element to copy
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void pushvalue(const int& index, lua_State* luaState = nullptr);

        /**
         * Pushes a double on top of Lua stack
         * @param n Value to push onto Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void push(const double& n, lua_State* luaState = nullptr);

        /**
         * Pushes a float on top of Lua stack
         * @param n Value to push onto Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void push(const float& n, lua_State* luaState = nullptr);

        /**
         * Pushes an integer on top of Lua stack
         * @param n Value to push onto Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void push(const int& n, lua_State* luaState = nullptr);

        /**
         * Pushes a string on top of Lua stack
         * @param n Value to push onto Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void push(const char* n, lua_State* luaState = nullptr);

        /**
         * Pushes a boolean on top of Lua stack
         * @param n Value to push onto Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void push(const bool& n, lua_State* luaState = nullptr);

        /**
         * Pushes a function on top of Lua stack
         * @param n Value to push onto Lua stack
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void push(int (*c)(lua_State*), lua_State* luaState = nullptr);

        /**
         * Assets number of arguments available on Lua stack. This is useful in callbacks from Lua to C++
         * @param arguments Expected number of arguments
         * @param mode Assertion mode (at least/at most/exact)
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void assertArgs(int arguments, LuaAssertArguments mode = LUA_ARGS_EXACT, lua_State* luaState = nullptr);

        static int set_ref(int index = -1, lua_State* luaState = nullptr);
        static void unset_ref(int index, lua_State* luaState = nullptr);
        static void get_ref(int ref, lua_State* luaState = nullptr);
    private:
        Lua();
        ~Lua();
        /**
         * Pushes correct context to top of application Lua stack. If name begins with `clua.`, the global Clua library
         * will be pushed onto application stack. Otherwise, it will verify that a table is on top of application Lua
         * stack
         * @param name Name to explode
         */
        lua_State* getApplicationState();
    private:
        lua_State* m_state;
        const char* m_libName { "clua" };
};