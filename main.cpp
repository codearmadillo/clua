#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"
#include "foundation/rendering.h"

#include "rendering/element.h"
#include "rendering/vertex.h"
#include "rendering/object-buffer.h"

class ObjectBuffer {

};

int main() {

    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();
    Rendering::Module::getInstance().setBindings();

    Runtime::getInstance().loadScripts();

    Window::getInstance().start();

    return 0;
}



