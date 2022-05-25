#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"

#include "utils/file.h"

int main() {
    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();

    Runtime::getInstance().loadScripts();

    Window::getInstance().start();

    return 0;
}
