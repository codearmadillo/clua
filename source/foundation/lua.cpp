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
    int top = lua_gettop(m_state);
    std::cout << "------------------- Starting Lua stack dump -------------------\n";
    if (top == 0) {
        std::cout << "Stack is empty\n";
        return *this;
    }
    for (int i = 1; i <= top; i++) {
        printf("%d\t%s\t", i, luaL_typename(m_state,i));
        switch (lua_type(m_state, i)) {
            case LUA_TNUMBER:
                printf("%g\n",lua_tonumber(m_state,i));
                break;
            case LUA_TSTRING:
                printf("%s\n",lua_tostring(m_state,i));
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (lua_toboolean(m_state, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                printf("%s\n", "nil");
                break;
            default:
                printf("%p\n",lua_topointer(m_state,i));
                break;
        }
    }
    std::cout << "------------------ Lua dump finished -------------------\n\n\n";
    return *this;
}
Lua& Lua::get(const char* name) {

    const int stack_offset = lua_gettop(m_state);
    const auto name_exploded = utils::string_explode(name, '.');

    lua_getglobal(m_state, m_libName);

    if (lua_isnoneornil(m_state, -1) || !lua_istable(m_state, -1)) {
        lua_pop(m_state, 1);
        throw std::runtime_error("Failed to get global 'clua' API - This happens when the API isn't initialized correctly.");
    }

    // get nested members
    bool scope_found = true;
    int local_stack_offset = 0;
    for (auto i = name_exploded.begin(); i != name_exploded.end() - 1; i++) {
        auto field_name = *i;
        auto field = lua_getfield(m_state, stack_offset + local_stack_offset + 1, field_name.c_str());

        if (lua_isnoneornil(m_state, stack_offset + local_stack_offset + 2) || !lua_istable(m_state, stack_offset + local_stack_offset + 2)) {
            scope_found = false;
            break;
        }

        local_stack_offset++;
    }

    // get actual field
    auto field = lua_getfield(m_state, stack_offset + local_stack_offset + 1, name_exploded[name_exploded.size() - 1].c_str());

    // Rotate value down the stack
    lua_insert(
        m_state, lua_gettop(m_state) - local_stack_offset - 1
    );

    // And cleanup stack
    lua_pop(m_state, local_stack_offset + 1);

    return *this;
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
Lua& Lua::call(const char* script, int nargs, int nresults) {
    (void) luaL_loadstring(m_state, script);
    lua_insert(m_state, -nargs - 1);
    (void)lua_call(m_state, nargs, nresults);
    return *this;
}
Lua& Lua::load_string(const char* script) {
    (void) luaL_loadstring(m_state, script);
    lua_pop(m_state, 1);
    return *this;
}
Lua& Lua::pcall(const char *script, int nargs, int nresults) {
    (void) luaL_loadstring(m_state, script);
    lua_insert(m_state, -nargs - 1);
    const int result = lua_pcall(m_state, nargs, nresults, -2);
    switch(result) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            std::cerr << "Calling a script with runtime error produced an error string: " << lua_tostring(m_state, -1) << std::endl;
            lua_pop(m_state, 1);
            break;
        case LUA_ERRMEM:
            std::cerr << "Memory allocation error occurred" << std::endl;
            lua_pop(m_state, 1);
            break;
        case LUA_ERRERR:
            std::cerr << "Error occurred while running custom error delegate" << std::endl;
            lua_pop(m_state, 1);
            break;
        default:
            std::cerr << "Undefined error occurred" << std::endl;
            lua_pop(m_state, 1);
            break;
    }
    return *this;
}
Lua& Lua::pcall(int nargs, int nresults) {
    const int result = lua_pcall(m_state, nargs, nresults, -2);
    switch(result) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            std::cerr << "Calling a script with runtime error produced an error string: " << lua_tostring(m_state, -1) << std::endl;
            lua_pop(m_state, 1);
            break;
        case LUA_ERRMEM:
            std::cerr << "Memory allocation error occurred" << std::endl;
            lua_pop(m_state, 1);
            break;
        case LUA_ERRERR:
            std::cerr << "Error occurred while running custom error delegate" << std::endl;
            lua_pop(m_state, 1);
            break;
        default:
            std::cerr << "Undefined error occurred" << std::endl;
            lua_pop(m_state, 1);
            break;
    }
    return *this;
}

Lua& Lua::push() {
    lua_pushnil(m_state);
    return *this;
}

Lua& Lua::pushvalue(const int &index) {
    lua_pushvalue(m_state, index);
    return *this;
}

Lua& Lua::push(const float &n) {
    lua_pushnumber(m_state, n);
    return *this;
}

Lua& Lua::push(const int &n) {
    lua_pushinteger(m_state, n);
    return *this;
}

Lua& Lua::push(const char *n) {
    lua_pushstring(m_state, n);
    return *this;
}

Lua& Lua::push(const bool &n) {
    lua_pushboolean(m_state, n);
    return *this;
}

Lua& Lua::push(int (*c)(lua_State *)) {
    lua_pushcfunction(m_state, c);
    return *this;
}

Lua &Lua::push(const double &n) {
    lua_pushnumber(m_state, n);
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
