#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "utils/log.h"

void Runtime::onBeforeWindowStart() {
    Lua::get("clua.start");
    Lua::pcall(0, 0);
}
void Runtime::onAfterWindowClose() {
    Lua::get("clua.destroy");
    Lua::pcall(0, 0);
}
void Runtime::onWindowFrame() {
    Lua::get("clua.update");
    Lua::push(Window::getInstance().getDeltaTime());
    Lua::pcall(1, 0);

    Lua::get("clua.draw");
    Lua::pcall(0, 0);
}
void Runtime::setBindings() {

    Lua::push([](lua_State* lua){
        LOG_INFO("Starting application\n");
        return 0;
    });
    Lua::bind("clua.start");

    Lua::push([](lua_State* lua){
        double dt = luaL_checknumber(lua, 1);
        return 0;
    });
    Lua::bind("clua.update");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::bind("clua.draw");

    Lua::push([](lua_State* lua){
        LOG_INFO("Destroying application\n");
        return 0;
    });
    Lua::bind("clua.destroy");
}
Runtime::Runtime() {
}
Runtime::~Runtime() {
}

void Runtime::loadScripts() {
    if (Lua::load_file("main.lua")) {
        Lua::pcall(0, 0);
    }
}
