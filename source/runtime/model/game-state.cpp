#include "runtime/model/game-state.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "utils/log.h"

GameState::GameState(uint8_t id): m_id(id) { }

void GameState::setBindings() {
    // Create reference in registry
    Lua::pushtable();
    m_ref = Lua::set_ref();

    // Get back the table
    Lua::get_ref(m_ref);

    // Create callbacks
    Lua::pushvoidptr(this);
    Lua::pushcclosure([](lua_State* l) {
        auto p = (GameState*) lua_topointer(l, lua_upvalueindex(1));
        p->setAsDefault();
        return 0;
    }, 1);
    Lua::set("setAsDefault", false);

    Lua::pushvoidptr(this);
    Lua::pushcclosure([](lua_State* l) {
        auto p = (GameState*) lua_topointer(l, lua_upvalueindex(1));
        p->setAsActive();
        return 0;
    }, 1);
    Lua::set("setAsActive", false);

    Lua::pushvoidptr(this);
    Lua::pushcclosure([](lua_State* l) {
        auto p = (GameState*) lua_topointer(l, lua_upvalueindex(1));
        return 0;
    }, 1);
    Lua::set("destroy", false);
}

void GameState::process_update() {
    // Get reference
    Lua::get_ref(m_ref);

    // Run state update callback
    Lua::get("update");
    if (!Lua::isnil(-1)) {
        Lua::push(Window::getInstance().getDeltaTime());
        Lua::pcall(1, 0);
    } else {
        Lua::pop(1);
    }


    // Run objects update callback

    // Pop reference
    Lua::pop(1);
}

void GameState::process_draw() {
    // Get reference
    Lua::get_ref(m_ref);

    // Run state update callback
    Lua::get("draw");
    if (!Lua::isnil(-1)) {
        Lua::pcall(0, 0);
    } else {
        Lua::pop(1);
    }

    // Run objects update callback

    // Pop reference
    Lua::pop(1);
}

void GameState::setAsActive() {
    LOG_DIAGNOSTIC("Setting state with ID " << m_id << " as active");
    Runtime::getInstance().setActiveStateId(m_id);
}

void GameState::setAsDefault() {
    LOG_DIAGNOSTIC("Setting state with ID " << m_id << " as default");
    Runtime::getInstance().setDefaultStateId(m_id);
}

void GameState::onActivated() {
    LOG_DIAGNOSTIC("Activated state ID " << m_id);

    // Run Lua callback
    Lua::get_ref(m_ref);

    Lua::get("onActivate");
    if (!Lua::isnil(-1)) {
        Lua::pcall(0, 0);
    } else {
        Lua::pop(1);
    }

    Lua::pop(1);
}

void GameState::onDeactivated() {
    LOG_DIAGNOSTIC("Deactivated state ID " << m_id);

    // Run Lua callback
    Lua::get_ref(m_ref);

    Lua::get("onDeactivate");
    if (!Lua::isnil(-1)) {
        Lua::pcall(0, 0);
    } else {
        Lua::pop(1);
    }

    Lua::pop(1);
}