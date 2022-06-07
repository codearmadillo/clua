#include "clua.h"

#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"
#include "foundation/rendering.h"

Clua::Clua() {
    bindModules();
    startWindow();
}
Clua::~Clua() {

}
void Clua::bindModules() {
    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();
    Rendering::Module::getInstance().setBindings();
}

void Clua::startWindow() {
    Runtime::getInstance().loadUserSource();
    Window::getInstance().start();
}
