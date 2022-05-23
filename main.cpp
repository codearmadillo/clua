#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "foundation/lua.h"

const std::string lua_static_script = R"(

)";

struct TestObject {
    const char* name;
    float value;
    bool global;
};

class Application {
private:
    static Lua* m_luaModule;
public:
    static void InitializeModules() {
        // Could be smart pointer
        m_luaModule = new Lua();
    }
    static void TerminateModules() {
        delete Application::m_luaModule;
    }
    static Lua* GetLuaModule() {
        return Application::m_luaModule;
    }
};

int main() {

    Application::InitializeModules();

    const auto lua = Application::GetLuaModule();

    std::vector<TestObject> tests {
        { "localTest", 1.0f, false },
        { "test", 2.0f, true },
        { "localTest.nested1", 3.0f, false },
        { "test.nested1", 4.0f, true },
        { "localTest.nested2.nested1", 5.0f, false },
        { "test.nested2.nested1", 6.0f, true },
        { "localTest.nested2.nested2", 7.0f, false },
        { "test.nested2.nested2", 8.0f, true }
    };


    /**
     * Tests:
     * localTest:1
     * clua.test:2
     * localTest.nested1:3
     * clua.test.nested1:4
     * localTest.nested2.nested1:5
     * clua.test.nested2.nested1:6
     * localTest.nested2.nested2:7
     * clua.test.nested2.nested2:8
     */

    for (const auto i : tests) {
        lua->SetFloat(i.name, i.value, i.global);
    }

    lua->Run(lua_static_script.c_str());

    Application::TerminateModules();

    return 0;
}
