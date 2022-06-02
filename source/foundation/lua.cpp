#include "foundation/lua.h"
#include "utils/string.h"

#include <iostream>
#include <vector>

#include "utils/log.h"

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

Lua::Lua(): m_state(luaL_newstate()) {
    lua_gc(m_state, LUA_GCSTOP, 0);
    luaL_openlibs(m_state);
    lua_gc(m_state, LUA_GCRESTART, 0);
}
Lua::~Lua() {
    lua_close(m_state);
}

lua_State* Lua::getApplicationState() {
    return m_state;
}

const char* Lua::getLibraryName() const {
    return m_libName;
}


void Lua::dump(lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
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

void Lua::call(const char* script, int nargs, int nresults, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    (void) luaL_loadstring(lua, script);
    lua_insert(lua, -nargs - 1);
    (void)lua_call(lua, nargs, nresults);
}
void Lua::pcall(const char* script, int nargs, int nresults, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
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
void Lua::pcall(int nargs, int nresults, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    auto top = lua_gettop(lua);
    if (top == 0 || !lua_iscfunction(lua, -1)) {
        return;
    }
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
void Lua::pcall(lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    Lua::pcall(0, 0, lua);
}

bool Lua::load_file(const char* path, const char* mode, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    auto status = luaL_loadfilex(lua, path, mode);
    if (status != LUA_OK) {
        std::cerr << lua_tostring(lua, -1) << "\n";
        lua_pop(lua, 1);
        return false;
    }
    return true;
}
void Lua::load_string(const char* string, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    (void) luaL_loadstring(lua, string);
}

void Lua::pushnil(lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushnil(lua);
}
void Lua::pushtable(lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_newtable(lua);
}
void Lua::pushvalue(const int& index, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushvalue(lua, index);
}
void Lua::push(const double& n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushnumber(lua, n);
}
void Lua::push(const float& n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushnumber(lua, n);
}
void Lua::push(const int& n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushinteger(lua, n);
}
void Lua::push(const char* n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushstring(lua, n);
}
void Lua::push(const bool& n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushboolean(lua, n);
}
void Lua::push(int (*c)(lua_State*), lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushcfunction(lua, c);
}

void Lua::assertArgs(int arguments, LuaAssertArguments mode, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();

    int args = lua_gettop(lua);
    switch(mode) {
        case LUA_ARGS_AT_LEAST:
            if (args < arguments) {
                std::string err("Expected at least" + std::to_string(arguments) + " arguments but received " + std::to_string(args) + " instead\n");
            }
            break;
        case LUA_ARGS_AT_MOST:
            if (args > arguments) {
                std::string err("Expected at most" + std::to_string(arguments) + " arguments but received " + std::to_string(args) + " instead\n");
            }
            break;
        case LUA_ARGS_EXACT:
            if (args != arguments) {
                std::string err("Expected exactly" + std::to_string(arguments) + " arguments but received " + std::to_string(args) + " instead\n");
            }
            break;
    }
}

void Lua::ensureStackContext(const char *name, lua_State* lua) {
    const auto lib_name = Lua::getInstance().getLibraryName();
    const auto name_exploded = utils::string_explode(name, '.');
    const bool push_to_library = std::string(lib_name) == std::string(name_exploded[0]);

    if (push_to_library) {
        lua_getglobal(lua, lib_name);

        if (lua_isnoneornil(lua, -1)) {
            lua_pop(lua, 1);

            lua_newtable(lua);
            lua_setglobal(lua, lib_name);

            lua_getglobal(lua, lib_name);
        } else {
            if (!lua_istable(lua, -1))
                LOG_ERROR("Expected '" << lib_name << "' to be a table (received '" << luaL_typename(lua, -1) << "')");
        }
    } else {
        if (lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
            LOG_ERROR("Failed to ensure correct stack context - the top of the stack needs to be a table");
        }
    }
}


void Lua::get(const char* name, lua_State* luaState) {

    const auto name_exploded = utils::string_explode(name, '.');
    auto lua = luaState ?: Lua::getInstance().getApplicationState();

    if (std::string(name_exploded[0]) == std::string(Lua::getInstance().getLibraryName())) {
        lua_getglobal(lua, Lua::getInstance().getLibraryName());
    }

    const int stack_offset = lua_gettop(lua) - 1;

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
void Lua::bind(const char* name, lua_State* luaState) {

    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    // Subtract one - Do not count base layer
    const auto name_exploded = utils::string_explode(name, '.');
    const auto lib_name = Lua::getInstance().getLibraryName();
    const auto lib_push = std::string(name_exploded[0]) == lib_name;

    // this is also where the value that will be added to stack lives
    const int stack_offset = lua_gettop(lua);
    if (stack_offset == 0) {
        LOG_ERROR("Expected at least 1 values on Lua stack, but received " << stack_offset << " instead");
    }

    // top-level or rotate
    if (std::string(name_exploded[0]) == lib_name) {
        lua_getglobal(lua, lib_name);
        if (lua_isnoneornil(lua, stack_offset + 1)) {
            lua_pop(lua, 1);

            lua_newtable(lua);
            lua_setglobal(lua, lib_name);

            lua_getglobal(lua, lib_name);
        } else {
            // Otherwise, verify that it is a table
            if (!lua_istable(lua, stack_offset + 1)) {
                lua_pop(lua, 1);

                std::string error ("Tried to create '");
                error.append(lib_name);
                error.append("' but it already exists, and is not a table");

                throw std::runtime_error(error.c_str());
            }
        }
    } else {
        lua_rotate(lua, -2, 1);
    }

    /** Bind value */
    Lua::bindInternal(name, lua);

    /** BindInternal doesn't clean top level - pop it */
    if (lib_push) {
        lua_pop(lua, 1);
    }

    std::cout << "binding " << name << ":\n";
    dump();
    std::cout << "\n";
}
void Lua::bindInternal(const char* name, lua_State* lua) {
    // Subtract 1 because of top level
    const int stack_state = lua_gettop(lua);
    const int stack_offset = stack_state - 1;
    const auto name_exploded = utils::string_explode(name, '.');

    /**
     * Stack checks:
     *  - Stack should have at least 2 values
     *  - Top value is table
     *  - Second to top is the actual value (raise warning if nil)
     */
    if (stack_state < 2) {
        LOG_ERROR("Expected at least 2 values on Lua stack, but received " << stack_state << " instead");
    }
    if (!lua_istable(lua, -1)) {
        LOG_ERROR("Expected top stack value to be table, but received '" << luaL_typename(lua, -1) << "' instead")
    }

    /**
     * Bind
     */
    // add nested subtables
    int local_stack_offset = 0;
    for (auto i = name_exploded.begin(); i != name_exploded.end() - 1; ++i) {
        auto field_name = *i;
        auto field = lua_getfield(lua, stack_offset + local_stack_offset + 1, field_name.c_str());

        if (lua_isnoneornil(lua, local_stack_offset + stack_offset + 2)) {
            lua_pop(lua, 1);

            lua_newtable(lua);
            lua_setfield(lua, local_stack_offset + stack_offset + 1, field_name.c_str());

            field = lua_getfield(lua, stack_offset + local_stack_offset + 1, field_name.c_str());
        } else {
            if (!lua_istable(lua, local_stack_offset + stack_offset + 2)) {
                lua_pop(lua, 1);

                std::string error ("Tried to create member'");
                error.append(field_name);
                error.append("' but it already exists, and is not a table");

                throw std::runtime_error(error.c_str());
            }
        }
        local_stack_offset++;
    }

    // move value to the top of the stack
    lua_rotate(lua, stack_offset, local_stack_offset + 1);

    // set field
    lua_setfield(lua, stack_offset + local_stack_offset, name_exploded[name_exploded.size() - 1].c_str());

    // cleanup stack - leave the toplevel
    lua_pop(lua, local_stack_offset);
}