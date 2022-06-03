#include <iostream>
#include "runtime/game-state-handler.h"
#include "foundation/lua.h"
#include "runtime/game-state-container.h"
#include "utils/log.h"

void GameStateHandler::setBindings() {
    Lua::push([](lua_State* lua){
        std::cout << "creating state";
        // Create new state
        auto state = GameStateContainer::getInstance().createState();
        // Push table to lua stack
        state->createLuaTableAndPushToState(lua);
        // Return value
        return 1;
    });
    Lua::set_global("clua.state.create");

    Lua::push([](lua_State* lua){
        LOG_INFO("Destroying state\n");
        return 0;
    });
    Lua::set_global("clua.state.destroy");

    Lua::push([](lua_State* lua){
        LOG_INFO("Freezing state\n");
        return 0;
    });
    Lua::set_global("clua.state.freeze");

    Lua::push([](lua_State* lua){
        LOG_INFO("Unfreezing state\n");
        return 0;
    });
    Lua::set_global("clua.state.unfreeze");
}