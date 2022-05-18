#=================== IMGUI ===================
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

add_library(IMGUI STATIC)

target_sources(
        IMGUI
        PRIVATE ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/imgui.cpp
        PRIVATE ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        PRIVATE ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)
target_include_directories(
        IMGUI
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC ${IMGUI_DIR}/include
        PUBLIC ${IMGUI_DIR}/misc/cpp
)
target_link_libraries(IMGUI glfw libglew_static)