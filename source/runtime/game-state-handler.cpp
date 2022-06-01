#include <iostream>
#include "runtime/game-state-handler.h"
#include "foundation/lua.h"
#include "runtime/game-state-container.h"

void GameStateHandler::setBindings() {
    Lua::getInstance()
        .push([](lua_State* lua){
            // Create new state
            auto state = GameStateContainer::getInstance().createState();
            // Push table to lua stack
            state->createLuaTableAndPushToState(lua);
            // Return value
            return 1;
        })
        .bind("create.state");
}