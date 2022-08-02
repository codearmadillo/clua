#include <iostream>
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include "utils/log.h"

void Runtime::setBindings() {
    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.start");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.update");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.draw");

    Lua::push([](lua_State* lua){
        return 0;
    });
    Lua::set_global("clua.destroy");
}

void Runtime::process(APPLICATION_PROCESS state) {
    switch(state) {
        case PROCESS_APPSTART:
            process_appStart();
            break;
        case PROCESS_APPLOADSOURCE:
            process_appLoadSource();
            break;
        case PROCESS_APPRUNSOURCE:
            process_appRunSource();
            break;
        case PROCESS_APPEND:
            process_appEnd();
            break;
        case PROCESS_FRAMESTART:
            process_frameStart();
            break;
        case PROCESS_FRAMEUPDATE:
            process_frameUpdate();
            break;
        case PROCESS_FRAMEDRAW:
            process_frameDraw();
            break;
        case PROCESS_FRAMEEND:
            process_frameEnd();
            break;
    }
}

void Runtime::process_appStart() {

}

void Runtime::process_appLoadSource() {
    if (Lua::load_file("main.lua")) {
        Lua::pcall(0, 0);
    }
}

void Runtime::process_appRunSource() {
    Lua::get_global("clua.start");
    Lua::pcall(0, 0);
}

void Runtime::process_appEnd() {
    Lua::get_global("clua.destroy");
    Lua::pcall(0, 0);
}

void Runtime::process_frameStart() {

}

void Runtime::process_frameUpdate() {
    // Run global Clua update callback
    Lua::get_global("clua.update");
    Lua::push(Window::getInstance().getDeltaTime());
    Lua::pcall(1, 0);

    // Do not update global objects - It is up to developer to update them

    // Run update callback in state - This will also update any objects in the scene
    // Do not update if state (game) is frozen
}

void Runtime::process_frameDraw() {
    // Run global Clua draw callback
    Lua::get_global("clua.draw");
    Lua::pcall(0, 0);

    // Do not update global objects - It is up to developer to draw them

    // Run draw callback in state - This will also draw any objects in the scene
}

void Runtime::process_frameEnd() {

}