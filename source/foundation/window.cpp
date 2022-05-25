#include <stdexcept>
#include <iostream>
#include "foundation/window.h"
#include "foundation/runtime.h"
#include "foundation/lua.h"
#include "foundation/keyboard.h"


Window::Window(): m_deltaTime(0.0), m_lastFrameTime(0.0) {
    if (!glfwInit())
        throw std::runtime_error("glfw initialization failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}
Window::~Window() {
    terminate();
}
void Window::start() {
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        terminate();
        throw std::runtime_error("glfw window creation failed");
    }

    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        terminate();
        throw std::runtime_error("glew initialization failed");
    }

    // GL pre-configuration
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

    // Store current time
    m_lastFrameTime = glfwGetTime();

    // Call runtime method
    Runtime::getInstance().onBeforeWindowStart();

    // Set callback
    glfwSetKeyCallback(m_window, Keyboard::glfwKeyCallback);

    // Start loop
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update delta time
        double currentFrameTime = glfwGetTime();
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;

        // Application update loop
        Runtime::getInstance().onWindowUpdate();

        glfwSwapBuffers(m_window);
    }
}

Window &Window::setWindowTitle(const char *title) {
    m_title = title;
    if(m_window)
        glfwSetWindowTitle(m_window, m_title.c_str());
    return *this;
}

Window &Window::setWindowSize(const int width, const int height) {
    m_width = width;
    m_height = height;
    onWindowSizeChanged();
    return *this;
}

Window &Window::setWindowWidth(int width) {
    m_width = width;
    onWindowSizeChanged();
    return *this;
}

Window &Window::setWindowHeight(int height) {
    m_height = height;
    onWindowSizeChanged();
    return *this;
}

Window &Window::setWindowFlags() {
    return *this;
}

Window &Window::setWindowClearColor(const glm::vec4 &color) {
    m_clearColor.r = color.r;
    m_clearColor.g = color.g;
    m_clearColor.b = color.b;
    m_clearColor.a = color.a;

    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

    return *this;
}

double Window::getDeltaTime() const {
    return m_deltaTime;
}

void Window::terminate() {
    if (m_window)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}

Window &Window::setBindings() {
    Lua::getInstance()
        // Window Width
        .push([](lua_State* lua){
            Lua::assertArguments(lua, 1);

            auto width = luaL_checknumber(lua, 1);
            Window::getInstance().setWindowWidth(static_cast<int>(width));

            return 0;
        })
        .bind("window.setWidth")
        // Window Height
        .push([](lua_State* lua){
            Lua::assertArguments(lua, 1);

            auto height = luaL_checknumber(lua, 1);
            Window::getInstance().setWindowHeight(static_cast<int>(height));

            return 0;
        })
        .bind("window.setHeight")
        // Window size
        .push([](lua_State* lua){
            Lua::assertArguments(lua, 2);

            auto width = luaL_checknumber(lua, 1);
            auto height = luaL_checknumber(lua, 2);

            Window::getInstance().setWindowSize(width, height);

            return 0;
        })
        .bind("window.setSize")
        // Window title
        .push([](lua_State* lua){
            Lua::assertArguments(lua, 1);

            auto title = luaL_checkstring(lua, 1);
            Window::getInstance().setWindowTitle(title);

            return 0;
        })
        .bind("window.setTitle");

    return *this;
}

void Window::onWindowSizeChanged() {
    if(m_window)
        glfwSetWindowSize(m_window, m_width, m_height);
}