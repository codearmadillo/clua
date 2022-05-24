#######################################################
### Configuration
#######################################################
cmake_minimum_required(VERSION 3.20)
project(game_test)

set(CMAKE_CXX_STANDARD 17)
set(STBI_NO_LINEAR TRUE)
set(CMAKE_CXX_STANDARD 20)

#######################################################
### Add executable files
#######################################################
add_executable(${CMAKE_PROJECT_NAME} main.cpp include/foundation/lua.h source/foundation/lua.cpp include/utils/string.h include/foundation/window.h source/foundation/window.cpp)

#######################################################
### Add subdirectories
#######################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor)

#######################################################
### Include directories
#######################################################
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#######################################################
### Link libraries
#######################################################
target_link_libraries(${CMAKE_PROJECT_NAME} glfw libglew_static IMGUI glm lua_static)
