#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"

#include "utils/file.h"

int main() {

    const char* lua_script = R"(
        --[[
        function clua.start()
            print('start')
        end
        function clua.update(dt)
            print(dt)
        end
        --]]
        function clua.update(dt)
            if (clua.keyboard.isKeyDown('a')) then
                print('a is down')
            else
                print('a is not down')
            end
        end
        function clua.keyboard.keyUp(key)
            print('released ' .. key);
        end
        function clua.keyboard.keyDown(key)
            print('pressed ' .. key);
        end
    )";

    // Bindings
    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();

    // Call Lua script with already attached bindings
    Lua::getInstance().pcall(lua_script, 0, 0);

    // Run window
    Window::getInstance().start();

    return 0;
}
