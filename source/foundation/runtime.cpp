#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"

void Runtime::onBeforeWindowStart() {
    Lua::getInstance()
        .get("start")
        .pcall(0, 0);
}
void Runtime::onWindowUpdate() {
    Lua::getInstance()
        .get("update")
        .push(Window::getInstance().getDeltaTime())
        .pcall(1, 0);
}
void Runtime::setBindings() {
    Lua::getInstance()
        // Default Start and Update functions
        .push([](lua_State* lua){
            return 0;
        })
        .bind("start")
        .push([](lua_State* lua){
            double dt = luaL_checknumber(lua, 1);
            return 0;
        })
        .bind("update");
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
