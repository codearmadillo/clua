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
    Lua::push(1, lua);
}