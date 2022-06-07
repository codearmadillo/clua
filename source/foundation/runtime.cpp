#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "utils/log.h"

void Runtime::onBeforeWindowStart() {
    Lua::get_global("clua.start");
    Lua::pcall(0, 0);
}
void Runtime::onAfterWindowClose() {
    Lua::get_global("clua.destroy");
    Lua::pcall(0, 0);
}
void Runtime::onWindowFrame() {
    // Update state
    Lua::get_global("clua.update");
    Lua::push(Window::getInstance().getDeltaTime());
    Lua::pcall(1, 0);

    // Update current state

    Lua::get_global("clua.draw");
    Lua::pcall(0, 0);
}
void Runtime::setBindings() {
    setRuntimeBindings();
    setStateBindings();
}
Runtime::Runtime() {
}
Runtime::~Runtime() {
}

void Runtime::loadUserSource() {
    if (Lua::load_file("main.lua")) {
        Lua::pcall(0, 0);
    }
}

void Runtime::setRuntimeBindings() {
    Lua::push([](lua_State* lua){
        LOG_INFO("Starting application\n");
        return 0;
    });
    Lua::set_global("clua.start");

    Lua::push([](lua_State* lua){
        double dt = luaL_checknumber(lua, 1);
        return 0;
    });
    Lua::set_global("clua.update");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.draw");

    Lua::push([](lua_State* lua){
        LOG_INFO("Destroying application\n");
        return 0;
    });
    Lua::set_global("clua.destroy");
}

void Runtime::setStateBindings() {
    Lua::push([](lua_State* lua){
        // Create state
        auto pair = Runtime::getInstance().m_gameStateContainer.create();
        auto state = pair.second;

        return 1;
    });
    Lua::set_global("clua.create.state");
}
