#ifndef NDEBUG
    // Production builds should set NDEBUG=1
    #define NDEBUG false
#else
    #define NDEBUG true
#endif
#ifndef DEBUG
    #define DEBUG !NDEBUG
#endif

#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"
#include "foundation/rendering.h"

#include "rendering/element.h"
#include "rendering/vertex.h"
#include "rendering/object-buffer.h"

#include "runtime/game-object-handler.h"
#include "runtime/game-state-handler.h"

int main() {
    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();
    Rendering::Module::getInstance().setBindings();
    GameObjectHandler::getInstance().setBindings();
    GameStateHandler::getInstance().setBindings();

    Runtime::getInstance().loadScripts();

    Window::getInstance().start();

    return 0;
}



