#include "runtime/model/game-state.h"
#include "foundation/lua.h"

GameState::GameState(uint8_t id): m_id(id) {

}

void GameState::setBindings() {
    Lua::pushtable();

    Lua::push([](lua_State* lua){
        std::cout << "Start\n";
        return 0;
    });
    Lua::set("start", false);
}

void GameState::pushToStack() {

}
