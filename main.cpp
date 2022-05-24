#include <iostream>
#include <vector>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "foundation/lua.h"

const std::string lua_script = R"(
    print(clua.engine.version)
)";

int main() {

    Lua::getInstance()
        .push(1)
        .bind("engine.version")
        .pcall(lua_script.c_str(), 0, 0);

    return 0;
}
