cmake_minimum_required(VERSION 3.10)
project(MyImGuiApp)

set(CMAKE_CXX_STANDARD 23)

# 1. Find system dependencies (GLFW and OpenGL)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Drogon REQUIRED)
# 2. Define ImGui Sources
# We need the core files AND the specific backends for GLFW and OpenGL3
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
set(HTTP_SOURCES
    src/http/poll.cpp)
# 3. Create the Executable
add_executable(app src/main.cpp ${IMGUI_SOURCES} ${HTTP_SOURCES})

# 4. Include Directories
# Allow includes like <imgui.h> and <backends/imgui_impl_glfw.h>
target_include_directories(app PUBLIC 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
    ${SRC_DIR}/http
    ${SRC_DIR}/widget
    ${SRC_DIR}/window
    ${SRC_DIR}/render
    ${SRC_DIR}/utils
)

# 5. Link Libraries
target_link_libraries(app PRIVATE glfw OpenGL::GL Drogon::Drogon pthread)