#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"

#include "utils/file.h"

int main() {

    // Load Lua scripts
    auto scripts = utils::read_file("../main.lua");

    // Set runtime bindings
    Runtime::getInstance().setBindings();

    // Set window bindings
    Window::getInstance().setBindings();

    // Call Lua script with already attached bindings
    Lua::getInstance().pcall(scripts.c_str(), 0, 0);

    // Run window
    Window::getInstance().start();

    return 0;
}
