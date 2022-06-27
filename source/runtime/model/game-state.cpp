#include "runtime/model/game-state.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"

GameState::GameState(uint8_t id): m_id(id) {
    setBindings();
}

void GameState::setBindings() {
    // Create reference in registry
    Lua::pushtable();
    m_ref = Lua::set_ref();

    // Get back the table
    Lua::get_ref(m_ref);

    // Create callbacks
    Lua::push([](lua_State* lua){
        std::cout << "update state\n";
        return 0;
    });
    Lua::set("update", false);

    Lua::push([](lua_State* lua){
        std::cout << "draw state\n";
        return 0;
    });
    Lua::set("draw", false);

    Lua::pushvoidptr(this);
    Lua::pushcclosure([](lua_State* lua){
        auto ptr = (GameState*) lua_topointer(lua, lua_upvalueindex(1));
        ptr->setAsActive();
        return 0;
    }, 1);
    Lua::set("setAsActive", false);

    Lua::pushvoidptr(this);
    Lua::pushcclosure([](lua_State* lua){
        auto ptr = (GameState*) lua_topointer(lua, lua_upvalueindex(1));
        ptr->setAsDefault();
        return 0;
    }, 1);
    Lua::set("setAsDefault", false);
}

void GameState::process_update() {
    // Get reference
    Lua::get_ref(m_ref);

    // Run state update callback
    Lua::get("update");
    Lua::push(Window::getInstance().getDeltaTime());
    Lua::pcall(1, 0);

    // Run objects update callback

    // Pop reference
    Lua::pop(1);
}

void GameState::process_draw() {
    // Get reference
    Lua::get_ref(m_ref);

    // Run state update callback
    Lua::get("draw");
    Lua::pcall(0, 0);

    // Run objects update callback

    // Pop reference
    Lua::pop(1);
}

void GameState::setAsActive() {
    Runtime::getInstance().setActiveStateId(m_id);
}

void GameState::setAsDefault() {
    Runtime::getInstance().setDefaultStateId(m_id);
}

void GameState::onActivated() {
    std::cout << ''"activated\n";
}

void GameState::onDeactivated() {

}