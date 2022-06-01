#include <iostream>
#include "runtime/game-state-handler.h"
#include "foundation/lua.h"
#include "runtime/game-state-container.h"
#include "utils/log.h"

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
        .bind("state.create")

        .push([](lua_State* lua){
            LOG_INFO("Destroying state\n");
            return 0;
        })
        .bind("state.destroy")

        .push([](lua_State* lua){
            LOG_INFO("Freezing state\n");
            return 0;
        })
        .bind("state.freeze")

        .push([](lua_State* lua){
            LOG_INFO("Unfreezing state\n");
            return 0;
        })
        .bind("state.unfreeze");
}