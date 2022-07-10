#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "utils/log.h"

void Runtime::setBindings() {
    setRuntimeBindings();
    setStateBindings();
}
void Runtime::setRuntimeBindings() {
    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.start");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.update");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.draw");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.destroy");


}

void Runtime::setStateBindings() {
    Lua::push([](lua_State* lua){
        // Create state
        auto pair = Runtime::getInstance().m_gameStateContainer.create();
        auto state = pair.second;

        // Set Lua bindings for the object
        state->setBindings();

        return 1;
    });
    Lua::set_global("clua.state.create");
}

void Runtime::process(APPLICATION_PROCESS state) {
    switch(state) {
        case PROCESS_APPSTART:
            process_appStart();
            break;
        case PROCESS_APPLOADSOURCE:
            process_appLoadSource();
            break;
        case PROCESS_APPRUNSOURCE:
            process_appRunSource();
            break;
        case PROCESS_APPEND:
            process_appEnd();
            break;
        case PROCESS_FRAMESTART:
            process_frameStart();
            break;
        case PROCESS_FRAMEUPDATE:
            process_frameUpdate();
            break;
        case PROCESS_FRAMEDRAW:
            process_frameDraw();
            break;
        case PROCESS_FRAMEEND:
            process_frameEnd();
            break;
    }
}

void Runtime::process_appStart() {

}

void Runtime::process_appLoadSource() {
    if (Lua::load_file("main.lua")) {
        Lua::pcall(0, 0);
    }
}

void Runtime::process_appRunSource() {
    Lua::get_global("clua.start");
    Lua::pcall(0, 0);
}

void Runtime::process_appEnd() {
    Lua::get_global("clua.destroy");
    Lua::pcall(0, 0);
}

void Runtime::process_frameStart() {

}

void Runtime::process_frameUpdate() {
    // Run global Clua update callback
    Lua::get_global("clua.update");
    Lua::push(Window::getInstance().getDeltaTime());
    Lua::pcall(1, 0);

    // Do not update global objects - It is up to developer to update them

    // Run update callback in state - This will also update any objects in the scene
    // Do not update if state (game) is frozen
    if (!m_isPaused) {
        auto active_state = getValidState();
        if (active_state) {
            active_state->process_update();
        } else {
            LOG_INFO("No valid state to update - Skipping");
        }
    }
}

void Runtime::process_frameDraw() {
    // Run global Clua draw callback
    Lua::get_global("clua.draw");
    Lua::pcall(0, 0);

    // Do not update global objects - It is up to developer to draw them

    // Run draw callback in state - This will also draw any objects in the scene
    auto active_state = getValidState();
    if (active_state) {
        active_state->process_draw();
    } else {
        LOG_INFO("No valid state to draw - Skipping");
    }
}

void Runtime::process_frameEnd() {

}

int Runtime::defaultStateId() const {
    return m_defaultStateId;
}

void Runtime::setDefaultStateId(int sceneId) {
    m_defaultStateId = sceneId;
}

int Runtime::activeStateId() const {
    return m_defaultStateId;
}

void Runtime::setActiveStateId(int stateId) {
    if (!m_gameStateContainer.has(stateId)) {
        LOG_ERROR("Cannot set active scene - " << stateId << " is not valid");
    }
    // Run onDeactivate callback on current state
    if (m_activeStateId > -1) {
        auto currentStatePtr = m_gameStateContainer.get(m_activeStateId);
        if (currentStatePtr != nullptr) {
            currentStatePtr->onDeactivated();
        }
    }
    // Set ID
    m_activeStateId = stateId;
    // Get state
    auto newStatePtr = m_gameStateContainer.get(m_activeStateId);
    newStatePtr->onActivated();
}

GameState *Runtime::getValidState() {
    GameState* active_state = nullptr;
    if (m_activeStateId > -1) {
        active_state = m_gameStateContainer.get(m_activeStateId);
    } else {
        if (m_defaultStateId > -1) {
            active_state = m_gameStateContainer.get(m_defaultStateId);
        }
    }
    if (active_state) {
        m_activeStateId = active_state->m_id;
    }
    return active_state;
}
