#ifndef IMGUI_HANDLER_HPP
#define IMGUI_HANDLER_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

class ImGuiHandler
{
private:
    GLFWwindow *m_Window;

public:
    ImGuiHandler(GLFWwindow *window);
    ~ImGuiHandler();

    void Initialize();
    void BeginFrame();
    void EndFrame();
    void Cleanup();

    // Render the shader selector UI
    void RenderShaderSelector(const std::vector<std::string> &shaderNames, int &selectedShader);
    void RenderDebugWindow(float frameTime);
};

#endif // IMGUI_HANDLER_HPP
