#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"

void Runtime::onBeforeWindowStart() {
    Lua::getInstance().pcall("clua.start()", 0, 0);
}
void Runtime::onWindowUpdate() {

}
void Runtime::setBindings() {
    Lua::getInstance()
        // Default Start and Update functions
        .push([](lua_State* lua){
            std::cout << "Starting\n";
            return 0;
        })
        .bind("start")
        .push([](lua_State* lua){
            std::cout << "Updating\n";
            return 0;
        })
        .bind("update");
}
Runtime::Runtime() {
}
Runtime::~Runtime() {
}