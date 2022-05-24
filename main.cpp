#include <iostream>
#include <vector>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "foundation/lua.h"

const std::string global_clua_name = "clua";

namespace utils {
    std::vector<std::string> string_explode(std::string const& source, char const delimiter) {
        std::vector<std::string> parts;
        std::istringstream iss(source);

        for (std::string token; std::getline(iss, token, delimiter); ) {
            parts.push_back(std::move(token));
        }

        return parts;
    }
}
const std::string lua_script = R"(
    print(clua.engine.version)

    local version = clua.engine.getVersion();
    print(version)
)";
void dumpLua(lua_State* lua_state) {
    int top = lua_gettop(lua_state);
    std::cout << "------------------- Starting Lua stack dump -------------------\n";
    if (top == 0) {
        std::cout << "Stack is empty\n";
        return;
    }
    for (int i = 1; i <= top; i++) {
        printf("%d\t%s\t", i, luaL_typename(lua_state,i));
        switch (lua_type(lua_state, i)) {
            case LUA_TNUMBER:
                printf("%g\n",lua_tonumber(lua_state,i));
                break;
            case LUA_TSTRING:
                printf("%s\n",lua_tostring(lua_state,i));
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (lua_toboolean(lua_state, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                printf("%s\n", "nil");
                break;
            default:
                printf("%p\n",lua_topointer(lua_state,i));
                break;
        }
    }
    std::cout << "------------------ Lua dump finished -------------------\n\n\n";
}
void withLua(lua_State* lua, const char* chunk, int nargs = 0, int nresults = 0) {
    (void) luaL_loadstring(lua, chunk);
    lua_insert(lua, -nargs - 1);
    (void)lua_call(lua, nargs, nresults);
}
/**
 * Adds value that's currently on top of Lua stack to Lua Library API
 * @param lua_state
 * @param name
 */
void addToLuaApi(lua_State* lua_state, const char* name) {

    // this is also where the value that will be added to stack lives
    const int stack_offset = lua_gettop(lua_state);
    if (stack_offset == 0) {
        throw std::runtime_error("[addToLuaApi] Cannot add key to Lua API because current stack size is 0. Please push value you wish to be added to Lua stack before calling this method");
    }

    const auto name_exploded = utils::string_explode(name, '.');

    // top-level
    lua_getglobal(lua_state, global_clua_name.c_str());

    // create new global Clua API if it does not exist
    if (lua_isnoneornil(lua_state, stack_offset + 1)) {
        lua_pop(lua_state, 1);

        lua_newtable(lua_state);
        lua_setglobal(lua_state, global_clua_name.c_str());

        lua_getglobal(lua_state, global_clua_name.c_str());
    } else {
        // Otherwise, verify that it is a table
        if (!lua_istable(lua_state, stack_offset + 1)) {
            lua_pop(lua_state, 1);

            std::string error ("Tried to create '");
            error.append(global_clua_name);
            error.append("' but it already exists, and is not a table");

            throw std::runtime_error(error.c_str());
        }
    }

    // add nested subtables
    int local_stack_offset = 0;
    for (auto i = name_exploded.begin(); i != name_exploded.end() - 1; ++i) {
        auto field_name = *i;
        auto field = lua_getfield(lua_state, stack_offset + local_stack_offset + 1, field_name.c_str());

        if (lua_isnoneornil(lua_state, local_stack_offset + stack_offset + 2)) {
            lua_pop(lua_state, 1);

            lua_newtable(lua_state);
            lua_setfield(lua_state, local_stack_offset + stack_offset + 1, field_name.c_str());

            field = lua_getfield(lua_state, stack_offset + local_stack_offset + 1, field_name.c_str());
        } else {
            if (!lua_istable(lua_state, local_stack_offset + stack_offset + 2)) {
                lua_pop(lua_state, 1);

                std::string error ("Tried to create member'");
                error.append(field_name);
                error.append("' but it already exists, and is not a table");

                throw std::runtime_error(error.c_str());
            }
        }
        local_stack_offset++;
    }

    // move value to the top of the stack
    lua_rotate(lua_state, stack_offset, local_stack_offset + 1);

    // set field
    lua_setfield(lua_state, stack_offset + local_stack_offset, name_exploded[name_exploded.size() - 1].c_str());

    // cleanup stack - dont forget to cleanup toplevel global table, hence +1
    lua_pop(lua_state, local_stack_offset + 1);

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

}

int main() {

    /** Load LUA */
    lua_State* lua_state = luaL_newstate();

    lua_gc(lua_state, LUA_GCSTOP, 0);
    luaL_openlibs(lua_state);
    lua_gc(lua_state, LUA_GCRESTART, 0);

    /** Add bindings */
    lua_pushnumber(lua_state, 0.1);
    addToLuaApi(lua_state, "engine.version");

    lua_pushcfunction(lua_state, [](lua_State* lua){
        lua_pushnumber(lua, 0.1);
        return 1;
    });
    addToLuaApi(lua_state, "engine.getVersion");

    /** Run */
    withLua(lua_state, lua_script.c_str(), 0);

    /** Close LUA */
    lua_close(lua_state);

    return 0;
}
