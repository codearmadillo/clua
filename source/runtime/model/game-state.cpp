#include "runtime/model/game-state.h"
#include "foundation/lua.h"

GameState::GameState(uint8_t id): m_id(id) {
    setBindings();
}

void GameState::setBindings() {
    // Create reference in registry
    Lua::pushtable();
    m_ref = Lua::set_ref();

    // Get back the table
    Lua::get_ref(m_ref);
}