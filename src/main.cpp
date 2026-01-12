#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <utility>
#include "poll.h"
#include "widget.h"
#include "window.h"
int main(int, char**)
{
    // 1. Setup GLFW
    HttpPoll::init();    
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
    std::string text;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui Linux Example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // 2. Setup Dear ImGui context
    // IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // (void)io;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Window::window win("hello");
    // win.addWidget("win1", std::make_unique<Widget::text<>>("sample"));
    // 3. Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // win.render();

        // ImGui::Begin("Hello, Linux!");
        // ImGui::Text("%s", text.c_str());
        // if(HttpPoll::is_data_available.load(std::memory_order_acquire)){
        //     std::lock_guard<std::mutex> _lock(HttpPoll::data_mtx);
        //     text = std::move(HttpPoll::data.front());
        //     HttpPoll::data.pop();
        //     HttpPoll::is_data_available.store(false);

        // }
        // ImGui::End();

        // Show the built-in demo window (useful for learning)
        // ImGui::ShowDemoWindow();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}