#include "foundation/lua.h"
#include "utils/string.h"

#include <iostream>
#include <vector>

Lua::Lua(): m_state(luaL_newstate()) {
    lua_gc(m_state, LUA_GCSTOP, 0);
    luaL_openlibs(m_state);
    lua_gc(m_state, LUA_GCRESTART, 0);
}
Lua::~Lua() {
    lua_close(m_state);
}
Lua& Lua::dump() {
    Lua::dump(m_state);
    return *this;
}
void Lua::dump(lua_State *lua) {
    int top = lua_gettop(lua);
    std::cout << "------------------- Starting Lua stack dump -------------------\n";
    if (top == 0) {
        std::cout << "Stack is empty\n";
        return;
    }
    for (int i = 1; i <= top; i++) {
        printf("%d\t%s\t", i, luaL_typename(lua,i));
        switch (lua_type(lua, i)) {
            case LUA_TNUMBER:
                printf("%g\n",lua_tonumber(lua,i));
                break;
            case LUA_TSTRING:
                printf("%s\n",lua_tostring(lua,i));
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (lua_toboolean(lua, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                printf("%s\n", "nil");
                break;
            default:
                printf("%p\n",lua_topointer(lua,i));
                break;
        }
    }
    std::cout << "------------------ Lua dump finished -------------------\n\n\n";
}
Lua& Lua::get(const char* name) {
    lua_getglobal(m_state, m_libName);
    if (lua_isnoneornil(m_state, -1) || !lua_istable(m_state, -1)) {
        lua_pop(m_state, 1);
        throw std::runtime_error("Failed to get global 'clua' API - This happens when the API isn't initialized correctly.");
    }

    Lua::get(name, m_state);

    return *this;
}
void Lua::get(const char* name, lua_State* lua) {
    // Subtract one - Do not count base layer
    const int stack_offset = lua_gettop(lua) - 1;
    const auto name_exploded = utils::string_explode(name, '.');

    // get nested members
    int local_stack_offset = 0;
    for (auto i = name_exploded.begin(); i != name_exploded.end() - 1; i++) {
        auto field_name = *i;
        auto field = lua_getfield(lua, stack_offset + local_stack_offset + 1, field_name.c_str());

        if (lua_isnoneornil(lua, stack_offset + local_stack_offset + 2) || !lua_istable(lua, stack_offset + local_stack_offset + 2)) {
            break;
        }

        local_stack_offset++;
    }

    // get actual field
    auto field = lua_getfield(lua, stack_offset + local_stack_offset + 1, name_exploded[name_exploded.size() - 1].c_str());

    // Rotate value down the stack
    lua_insert(
            lua, lua_gettop(lua) - local_stack_offset - 1
    );

    // And cleanup stack
    lua_pop(lua, local_stack_offset + 1);
}
Lua& Lua::bind(const char* name) {

    // this is also where the value that will be added to stack lives
    const int stack_offset = lua_gettop(m_state);
    if (stack_offset == 0) {
        throw std::runtime_error("[addToLuaApi] Cannot add key to Lua API because current stack size is 0. Please push value you wish to be added to Lua stack before calling this method");
    }

    const auto name_exploded = utils::string_explode(name, '.');

    // top-level
    lua_getglobal(m_state, m_libName);

    // create new global Clua API if it does not exist
    if (lua_isnoneornil(m_state, stack_offset + 1)) {
        lua_pop(m_state, 1);

        lua_newtable(m_state);
        lua_setglobal(m_state, m_libName);

        lua_getglobal(m_state, m_libName);
    } else {
        // Otherwise, verify that it is a table
        if (!lua_istable(m_state, stack_offset + 1)) {
            lua_pop(m_state, 1);

            std::string error ("Tried to create '");
            error.append(m_libName);
            error.append("' but it already exists, and is not a table");

            throw std::runtime_error(error.c_str());
        }
    }

    // add nested subtables
    int local_stack_offset = 0;
    for (auto i = name_exploded.begin(); i != name_exploded.end() - 1; ++i) {
        auto field_name = *i;
        auto field = lua_getfield(m_state, stack_offset + local_stack_offset + 1, field_name.c_str());

        if (lua_isnoneornil(m_state, local_stack_offset + stack_offset + 2)) {
            lua_pop(m_state, 1);

            lua_newtable(m_state);
            lua_setfield(m_state, local_stack_offset + stack_offset + 1, field_name.c_str());

            field = lua_getfield(m_state, stack_offset + local_stack_offset + 1, field_name.c_str());
        } else {
            if (!lua_istable(m_state, local_stack_offset + stack_offset + 2)) {
                lua_pop(m_state, 1);

                std::string error ("Tried to create member'");
                error.append(field_name);
                error.append("' but it already exists, and is not a table");

                throw std::runtime_error(error.c_str());
            }
        }
        local_stack_offset++;
    }

    // move value to the top of the stack
    lua_rotate(m_state, stack_offset, local_stack_offset + 1);

    // set field
    lua_setfield(m_state, stack_offset + local_stack_offset, name_exploded[name_exploded.size() - 1].c_str());

    // cleanup stack - dont forget to cleanup toplevel global table, hence +1
    lua_pop(m_state, local_stack_offset + 1);

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

    return *this;
}
void Lua::bind(const char* name, lua_State* lua) {

}

Lua& Lua::load_string(const char* script) {
    (void) luaL_loadstring(m_state, script);
    lua_pop(m_state, 1);
    return *this;
}
Lua& Lua::call(const char* script, int nargs, int nresults) {
    Lua::call(script, m_state, nargs, nresults);
    return *this;
}
void Lua::call(const char* script, lua_State* lua, int nargs, int nresults) {
    (void) luaL_loadstring(lua, script);
    lua_insert(lua, -nargs - 1);
    (void)lua_call(lua, nargs, nresults);
}

Lua& Lua::pcall(const char *script, int nargs, int nresults) {
    Lua::pcall(script, m_state, nargs, nresults);
    return *this;
}
void Lua::pcall(const char* script, lua_State* lua, int nargs, int nresults) {
    (void) luaL_loadstring(lua, script);
    lua_insert(lua, -nargs - 1);
    const int result = lua_pcall(lua, nargs, nresults, -2);
    switch(result) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            std::cerr << "Calling a script with runtime error produced an error string: " << lua_tostring(lua, -1) << std::endl;
            lua_pop(lua, 1);
            break;
        case LUA_ERRMEM:
            std::cerr << "Memory allocation error occurred" << std::endl;
            lua_pop(lua, 1);
            break;
        case LUA_ERRERR:
            std::cerr << "Error occurred while running custom error delegate" << std::endl;
            lua_pop(lua, 1);
            break;
        default:
            std::cerr << "Undefined error occurred" << std::endl;
            lua_pop(lua, 1);
            break;
    }
}

Lua& Lua::pcall(int nargs, int nresults) {
    Lua::pcall(m_state, nargs, nresults);
    return *this;
}
void Lua::pcall(lua_State* lua, int nargs, int nresults) {
    const int result = lua_pcall(lua, nargs, nresults, -2);
    switch(result) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            std::cerr << "Calling a script with runtime error produced an error string: " << lua_tostring(lua, -1) << std::endl;
            lua_pop(lua, 1);
            break;
        case LUA_ERRMEM:
            std::cerr << "Memory allocation error occurred" << std::endl;
            lua_pop(lua, 1);
            break;
        case LUA_ERRERR:
            std::cerr << "Error occurred while running custom error delegate" << std::endl;
            lua_pop(lua, 1);
            break;
        default:
            std::cerr << "Undefined error occurred" << std::endl;
            lua_pop(lua, 1);
            break;
    }
}

void Lua::push(lua_State* lua) {
    lua_pushnil(lua);
}
Lua& Lua::push() {
    Lua::push(m_state);
    return *this;
}

void Lua::pushvalue(const int &index, lua_State* lua) {
    lua_pushvalue(lua, index);
}
Lua& Lua::pushvalue(const int &index) {
    Lua::pushvalue(index, m_state);
    return *this;
}

void Lua::pushtable(lua_State* lua) {
    lua_newtable(lua);
}
Lua& Lua::pushtable() {
    Lua::pushtable(m_state);
    return *this;
}

void Lua::push(const float &n, lua_State* lua) {
    lua_pushnumber(lua, n);
}
Lua& Lua::push(const float &n) {
    Lua::push(n, m_state);
    return *this;
}

void Lua::push(const int &n, lua_State* lua) {
    lua_pushinteger(lua, n);
}
Lua& Lua::push(const int &n) {
    Lua::push(n, m_state);
    return *this;
}

void Lua::push(const char *n, lua_State* lua) {
    lua_pushstring(lua, n);
}
Lua& Lua::push(const char *n) {
    Lua::push(n, m_state);
    return *this;
}

void Lua::push(const bool &n, lua_State* lua) {
    lua_pushboolean(lua, n);
}
Lua& Lua::push(const bool &n) {
    Lua::push(n, m_state);
    return *this;
}

void Lua::push(int (*c)(lua_State *), lua_State* lua) {
    lua_pushcfunction(lua, c);
}
Lua& Lua::push(int (*c)(lua_State *)) {
    Lua::push(c, m_state);
    return *this;
}

void Lua::push(const double &n, lua_State* lua) {
    lua_pushnumber(lua, n);
}
Lua &Lua::push(const double &n) {
    Lua::push(n, m_state);
    return *this;
}

bool Lua::load_file(const char *path, const char* mode) {
    auto status = luaL_loadfilex(m_state, path, mode);
    if (status != LUA_OK) {
        std::cerr << lua_tostring(m_state, -1) << "\n";
        lua_pop(m_state, 1);
        return false;
    }
    return true;
}

void Lua::assertArguments(lua_State* lua, int n, LuaAssertArguments mode)
{
    int args = lua_gettop(lua);
    switch(mode) {
        case LUA_ARGS_AT_LEAST:
            if (args < n) {
                std::string err("Expected at least" + std::to_string(n) + " arguments but received " + std::to_string(args) + " instead\n");
            }
            break;
        case LUA_ARGS_AT_MOST:
            if (args > n) {
                std::string err("Expected at most" + std::to_string(n) + " arguments but received " + std::to_string(args) + " instead\n");
            }
            break;
        case LUA_ARGS_EXACT:
            if (args != n) {
                std::string err("Expected exactly" + std::to_string(n) + " arguments but received " + std::to_string(args) + " instead\n");
            }
            break;
    }
}