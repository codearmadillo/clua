#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "utils/log.h"

void Runtime::onBeforeWindowStart() {
    Lua::getInstance()
        .get("start")
        .pcall(0, 0);
}
void Runtime::onAfterWindowClose() {
    Lua::getInstance()
            .get("destroy")
            .pcall(0, 0);
}
void Runtime::onWindowFrame() {
    Lua::getInstance()
        .get("update")
        .push(Window::getInstance().getDeltaTime())
        .pcall(1, 0);
    Lua::getInstance()
        .get("draw")
        .pcall(0, 0);
}
void Runtime::setBindings() {
    Lua::getInstance()
        // Default Start and Update functions
        .push([](lua_State* lua){
            LOG_INFO("Starting application\n");
            return 0;
        })
        .bind("start")

        .push([](lua_State* lua){
            double dt = luaL_checknumber(lua, 1);
            return 0;
        })
        .bind("update")

        .push([](lua_State* lua){
            return 0;
        })
        .bind("draw")

        .push([](lua_State* lua){
            LOG_INFO("Destroying application\n");
            return 0;
        })
        .bind("destroy");
}
Runtime::Runtime() {
}
Runtime::~Runtime() {
}

void Runtime::loadScripts() {
    auto loaded = Lua::getInstance().load_file("main.lua");
    if(loaded) {
        Lua::getInstance().pcall(0, 0);
    }
}
