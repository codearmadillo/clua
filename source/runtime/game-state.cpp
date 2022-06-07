#include "runtime/game-state.h"
#include "foundation/lua.h"
#include <iostream>

#include "utils/log.h"

GameState::GameState(uint8_t id): m_id(id) {
    setBindings();
}
GameState::~GameState() {

}
void GameState::setBindings() {
    Lua::pushtable();

    Lua::push([](lua_State* lua){
        std::cout << "start\n";
        return 0;
    });
    Lua::set("start", false);

    Lua::dump();
}
void GameState::getRef() {
    Lua::get_ref(m_ref);
}
/*
void GameState::createLuaTableAndPushToState(lua_State *lua) {
    https://devdocs.io/lua~5.4/index#luaL_ref

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

    Lua::dump();


}
*/