#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"

#include "utils/file.h"

int main() {

    const char* lua_script = R"(
        function clua.start()
            print('start')
        end
        function clua.update(dt)
            print(dt)
        end
    )";

    // Set runtime bindings
    Runtime::getInstance().setBindings();

    // Set window bindings
    Window::getInstance().setBindings();

    // Call Lua script with already attached bindings
    Lua::getInstance().pcall(lua_script, 0, 0);

    // Run window
    Window::getInstance().start();

    return 0;
}
