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
        static void dump(lua_State* luaState = nullptr);


        static void bindInternal(const char* name, lua_State* lua = nullptr);

        /**
         * Binds a value on top of the stack, to the table that sits at below the top stack value. This value needs
         * to be a table. The bound value is cleared, and the target becomes the top of the stack
         * @param name Name to bind
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void bind(const char* name, lua_State* luaState = nullptr);

        /**
         * Retrieves a member from table on top of the Lua stack, and pushes it on top of the stack if it exists.
         * If it does not exist, pushes nil on top of the stack instead
         * @param name Name to retrieve
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void get(const char* name, lua_State* luaState = nullptr);

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
         * Calls a script on top of the stack in protected mode with 0 expected arguments and results
         * @param luaState Lua state to use. Defaults to global application Lua state
         */
        static void pcall(lua_State* luaState = nullptr);

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
    private:
        Lua();
        ~Lua();
        /**
         * Pushes correct context to top of application Lua stack. If name begins with `clua.`, the global Clua library
         * will be pushed onto application stack. Otherwise, it will verify that a table is on top of application Lua
         * stack
         * @param name Name to explode
         */
        static void ensureStackContext(const char* name, lua_State* lua = nullptr);
        lua_State* getApplicationState();
        const char* getLibraryName() const;
    private:
        lua_State* m_state;
        const char* m_libName { "clua" };
};