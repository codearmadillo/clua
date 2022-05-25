#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"

#include "utils/file.h"

int main() {

    // Bindings
    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();

    // Call Lua script with already attached bindings
    auto scriptLoaded = Lua::getInstance().load_file("../main.lua");
    if (scriptLoaded) {
        Lua::getInstance().pcall(0, 0);
    }

    // Run window
    Window::getInstance().start();

    return 0;
}
