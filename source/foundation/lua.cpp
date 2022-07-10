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

void Lua::dump_stack(lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    int top = lua_gettop(lua);
    std::cout << "------------------- Starting Lua stack dump -------------------\n";
    if (top == 0) {
        std::cout << "Stack is empty\n";
    } else {
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
    };
    std::cout << "------------------ Lua dump finished -------------------\n\n\n";
}

void Lua::get_stack(lua_State *luaState) {

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
    Lua::pcall(nargs, nresults, lua);
}
void Lua::pcall(int nargs, int nresults, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
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
void Lua::pushcclosure(int (*c)(lua_State *), int arguments, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushcclosure(lua, c, arguments);
}
void Lua::pushvoidptr(void* p, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pushlightuserdata(lua, p);
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

void Lua::get(const char* name, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    const auto name_exploded = utils::string_explode(name, '.');

    if(lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
        LOG_ERROR("Tried to get '" << name << "' but '" << name_exploded[0] << "' was '" << luaL_typename(lua, -1) << "'")
    }

    for (int i = 0; i < name_exploded.size(); i++) {
        lua_getfield(lua, -1, name_exploded[i].c_str());

        if (i == name_exploded.size() - 1) {
            lua_insert(lua, name_exploded.size() * -1);
            lua_pop(lua, name_exploded.size() - 1);
        } else {
            if (lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
                LOG_ERROR("Expected table at '" << name << "' ('" << name_exploded[i] << "')" << "' but it was '" << luaL_typename(lua, -1) << "'")
            }
        }
    }
}

void Lua::get_global(const char *name, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    const auto name_exploded = utils::string_explode(name, '.');

    lua_getglobal(lua, name_exploded[0].c_str());

    if(lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
        LOG_ERROR("Tried to get '" << name << "' but '" << name_exploded[0] << "' was '" << luaL_typename(lua, -1) << "'")
    }

    for (int i = 1; i < name_exploded.size(); i++) {
        lua_getfield(lua, -1, name_exploded[i].c_str());

        if(i == name_exploded.size() - 1) {
            // Last value is now on top of the stack
            lua_insert(lua, name_exploded.size() * -1);
            lua_pop(lua, name_exploded.size() - 1);
        } else {
            if(lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
                LOG_ERROR("Expected table at '" << name << "' ('" << name_exploded[i] << "')" << "' but it was '" << luaL_typename(lua, -1) << "'")
            }
        }
    }
}

bool Lua::exists(const char* name, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    const auto name_exploded = utils::string_explode(name, '.');

    throw std::runtime_error("Not implemented");
}

bool Lua::exists_global(const char* name, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    const auto name_exploded = utils::string_explode(name, '.');

    lua_getglobal(lua, name_exploded[0].c_str());

    if(lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
        lua_pop(lua, 1);
        return false;
    }

    int iterations = 1;
    bool found;

    for (int i = 1; i < name_exploded.size(); i++) {
        iterations++;

        lua_getfield(lua, -1, name_exploded[i].c_str());

        if(i == name_exploded.size() - 1) {
            if (lua_isnone(lua, -1)) {
                found = false;
            } else {
                found = true;
            }
        } else {
            if(lua_isnoneornil(lua, -1) || !lua_istable(lua, -1)) {
                found = false;
                break;
            }
        }
    }

    lua_pop(lua, iterations);

    return found;
}

void Lua::set(const char *name, bool clearTable, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    const auto name_exploded = utils::string_explode(name, '.');
    const auto scoped = name_exploded.size() > 1;

    // Ensure a table sits on top of the stack
    if (lua_gettop(lua) < 2) {
        LOG_ERROR("Tried to set '" << name << "' but the stack is missing table or value");
    }
    if (lua_isnoneornil(lua, -2) || !lua_istable(lua, -2)) {
        LOG_ERROR("Tried to set '" << name << "' but the top of the stack is not a table");
    }

    lua_rotate(lua, -2, 1);

    int intermediates = 0;

    for (int i = 0; i < name_exploded.size(); i++) {
        if (i == name_exploded.size() - 1) {
            /**
             * Reorganize stack as follows:
             * - Anything othet values
             * - Original table
             * - Destination table
             * - Destination value
             */
            // Move destination table under value
            lua_insert(lua, -name_exploded.size() - 1);

            if (intermediates - 1 > 0) {
                lua_pop(lua, intermediates - 1);
            }

            // If scoped structure, move base table under destination table
            if (scoped) {
                lua_insert(lua, -3);
            }

            // Set field
            lua_setfield(lua, -2, name_exploded[i].c_str());

            // Remove destination from stack if name is scoped (otherwise destination is base)
            if (scoped) {
                lua_pop(lua, 1);
            }
        } else {
            lua_getfield(lua, -1, name_exploded[i].c_str());
            if (lua_isnoneornil(lua, -1)) {
                lua_pop(lua, 1);

                lua_newtable(lua);
                lua_setfield(lua, -2, name_exploded[i].c_str());

                lua_getfield(lua, -1, name_exploded[i].c_str());

                intermediates++;
            } else if (!lua_istable(lua, -1)) {
                LOG_ERROR("Tried to index '" << name_exploded[i] << "' in '" << name << "' but value at the given index was a '" << luaL_typename(lua, -1) << "'");
            }
        }
    }

    if (clearTable) {
        lua_pop(lua, 1);
    }


}

void Lua::set_global(const char *name, bool clearTable, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    const auto name_exploded = utils::string_explode(name, '.');
    const auto nested = name_exploded.size() > 1;

    // If value is not nested, just set the value right away
    if (!nested) {
        lua_setglobal(lua, name_exploded[0].c_str());

        if(!clearTable) {
            lua_getglobal(lua, name_exploded[0].c_str());
        }

        return;
    }

    // Ensure global exists and it is a table
    lua_getglobal(lua, name_exploded[0].c_str());

    // If value is not nested, just set the value right away
    if (lua_isnoneornil(lua, -1)) {
        lua_pop(lua, 1);

        lua_newtable(lua);
        lua_setglobal(lua, name_exploded[0].c_str());

        lua_getglobal(lua, name_exploded[0].c_str());
    } else if (!lua_istable(lua, -1)) {
        LOG_ERROR("Tried to set '" << name << "' but '" << name_exploded[0] << "' is not a table");
    }

    int intermediates = 0;

    // Iterate through remaining fields and build structure
    for (int i = 1; i < name_exploded.size() - 1; i++) {
        lua_getfield(lua, -1, name_exploded[i].c_str());

        // Create field if it does not exist
        if(lua_isnoneornil(lua, -1)) {
            lua_pop(lua, 1);

            lua_newtable(lua);
            lua_setfield(lua, -2, name_exploded[i].c_str());

            lua_getfield(lua, -1, name_exploded[i].c_str());
        } else if (!lua_istable(lua, -1)) {
            LOG_ERROR("Tried to set '" << name << "' but '" << name_exploded[i] << "' is not a table");
        }

        intermediates++;
    }

    // Get stack offset - How many steps do we need to rotate the value
    int rotateBy = intermediates + 1;

    // Move value to top of the stack
    lua_rotate(lua, -rotateBy - 1, rotateBy);

    // Set value
    lua_setfield(lua, -2, name_exploded[name_exploded.size() - 1].c_str());

    // Remove intermediates. If required, cleanup the table
    lua_pop(lua, intermediates);

    // Cleanup underlying table
    if (clearTable) {
        lua_pop(lua, 1);
    }
}

int Lua::set_ref(int index, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    if (lua_gettop(lua) == 0) {
        LOG_ERROR("Cannot set a value into registry - stack is empty");
    }
    return luaL_ref(lua, LUA_REGISTRYINDEX);
}

void Lua::unset_ref(int index, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    luaL_unref(lua, LUA_REGISTRYINDEX, index);
}

void Lua::get_ref(int ref, lua_State *luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_rawgeti(lua, LUA_REGISTRYINDEX, ref);
}

void Lua::pop(int n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    lua_pop(lua, n);
}

bool Lua::isnil(int n, lua_State* luaState) {
    auto lua = luaState ?: Lua::getInstance().getApplicationState();
    return lua_isnil(lua, n);
}