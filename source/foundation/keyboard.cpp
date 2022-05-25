#include "foundation/keyboard.h"
#include "foundation/lua.h"
#include "foundation/window.h"
#include <iostream>

Keyboard::Keyboard() {

}
Keyboard::~Keyboard() {

}
void Keyboard::setBindings() {
    Lua::getInstance()
        .push([](lua_State* lua){
            return 0;
        })
        .bind("keyboard.keyPressed")
        .push([](lua_State* lua){
            return 0;
        })
        .bind("keyboard.keyReleased")
        .push([](lua_State* lua){
            auto keyName = luaL_checkstring(lua, 1);
            auto keyState = Keyboard::getInstance().getKeyState(keyName);

            lua_pushboolean(lua, keyState == true);

            return 1;
        })
        .bind("keyboard.isKeyDown");
}
void Keyboard::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Keyboard::getInstance().setKeyMap(key, scancode, action);
}
void Keyboard::setKeyMap(int key, int scancode, int action) {
    bool isKeyPressed = action != GLFW_RELEASE;

    if (!m_keys.contains(scancode)) {
        m_keys.insert(std::pair<int, bool>(scancode, isKeyPressed));
        if (isKeyPressed) {
            fireOnKeyDown(key, scancode);
        }
    } else {
        if (m_keys[scancode] == isKeyPressed) {
            return;
        }
        m_keys[scancode] = isKeyPressed;
        if (isKeyPressed) {
            fireOnKeyDown(key, scancode);
        } else {
            fireOnKeyUp(key, scancode);
        }
    }
}
void Keyboard::fireOnKeyDown(int key, int scancode) {
    auto name = getKeyName(key, scancode);
    if (name == nullptr) {
        return;
    }
    Lua::getInstance()
        .get("keyboard.keyPressed")
        .push(name)
        .pcall(1, 0);
}
void Keyboard::fireOnKeyUp(int key, int scancode) {
    auto name = getKeyName(key, scancode);
    if (name == nullptr) {
        return;
    }
    Lua::getInstance()
            .get("keyboard.keyReleased")
            .push(name)
            .pcall(1, 0);
}
bool Keyboard::getKeyState(const char* name) {
    return false;
}
const char* Keyboard::getKeyName(int key, int scancode) {
    const char* name = glfwGetKeyName(key, scancode);
    if (name == nullptr) {
        switch(key) {
            case GLFW_KEY_RIGHT:
                return "arrow_right";
            case GLFW_KEY_LEFT:
                return "arrow_left";
            case GLFW_KEY_UP:
                return "arrow_up";
            case GLFW_KEY_DOWN:
                return "arrow_down";
            default:
                return nullptr;
        }
    }
    return name;
}