#include <iostream>

#include "foundation/lua.h"
#include "foundation/window.h"

const std::string lua_script = R"(
    print(clua.engine.version)
)";

int main() {

    Window::getInstance()
        .setWindowTitle("My nice window")
        .setWindowClearColor({ 1.0f, 1.0f, 1.0f, 0.0f })
        .start();

    Lua::getInstance()
        .push(1)
        .bind("engine.version")
        .pcall(lua_script.c_str(), 0, 0);

    return 0;
}
