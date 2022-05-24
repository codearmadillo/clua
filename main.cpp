#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"

const std::string lua_script = R"(
    clua.start = function()
        clua.window.setWidth(1024)
        clua.window.setHeight(768)
        clua.window.setTitle('My new game')
    end
)";

int main() {

    // Set runtime bindings
    Runtime::getInstance().setBindings();

    // Set window bindings
    Window::getInstance().setBindings();

    // Call Lua script with already attached bindings
    Lua::getInstance().pcall(lua_script.c_str(), 0, 0);

    // Run window
    Window::getInstance().start();

    return 0;
}
