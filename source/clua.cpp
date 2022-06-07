#include "clua.h"

#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/keyboard.h"
#include "foundation/rendering.h"

Clua::Clua() {
    setModuleBindings();
    loadUserScripts();
    startApplicationWindow();
}
Clua::~Clua() {

}
void Clua::setModuleBindings() {
    Runtime::getInstance().setBindings();
    Keyboard::getInstance().setBindings();
    Window::getInstance().setBindings();
    Rendering::Module::getInstance().setBindings();
}

void Clua::loadUserScripts() {
    Runtime::getInstance().loadScripts();
}

void Clua::startApplicationWindow() {
    Window::getInstance().start();
}
