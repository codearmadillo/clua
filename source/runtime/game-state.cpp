#include "runtime/game-state.h"
#include "foundation/lua.h"
#include <iostream>

#include "utils/log.h"

GameState::GameState(uint8_t id): m_id(id) {
    LOG_INFO("creating state with id " << std::to_string(m_id));
}
GameState::~GameState() {
    LOG_INFO("deleting state with id " << std::to_string(m_id));
}
void GameState::createLuaTableAndPushToState(lua_State *lua) {
    // Push top-level table
    Lua::pushtable(lua);

    // Bind methods
    Lua::push([](lua_State* l){
        std::cout << "start\n";
        return 0;
    }, lua);
    Lua::bind("start", lua);

    Lua::push([](lua_State* l){
        std::cout << "update\n";
        return 0;
    }, lua);
    Lua::bind("update", lua);

    Lua::push([](lua_State* l){
        std::cout << "draw\n";
        return 0;
    }, lua);
    Lua::bind("draw", lua);

    Lua::push([](lua_State* l){
        std::cout << "destroy\n";
        return 0;
    }, lua);
    Lua::bind("destroy", lua);

    Lua::push([](lua_State* l){
        std::cout << "setAsDefault\n";
        return 0;
    }, lua);
    Lua::bind("setAsDefault", lua);

    Lua::push([](lua_State* l){
        std::cout << "setAsActive\n";
        return 0;
    }, lua);
    Lua::bind("setAsActive", lua);
}