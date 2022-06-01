#include <iostream>
#include "runtime/game-object-handler.h"
#include "foundation/lua.h"

void GameObjectHandler::setBindings() {
    Lua::getInstance()
            .push([](lua_State* lua) {
                Lua::assertArguments(lua, 1, LUA_ARGS_AT_MOST);
                auto args = lua_gettop(lua);
                // Check if object belongs to a state
                if (args == 1) {
                    std::cout << "create object in state\n";
                } else {
                    std::cout << "create global object\n";
                }
                return 0;
            })
            .bind("create.object");
}