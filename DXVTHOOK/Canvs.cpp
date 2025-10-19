#include "basic.h"
#include "./Dx11Lib/d3d11Hook.h"


constexpr size_t FPS_HISTORY_SIZE = 120; // 保存最近 120 帧的 FPS 值
std::array<float, FPS_HISTORY_SIZE> fpsHistory = {};
size_t fpsHistoryIndex = 0;
void DrawFps()
{
    if (ImGui::Begin("FPS Display", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing))
    {
        // 获取当前 FPS
        auto fps = ImGui::GetIO().Framerate;

        // 更新 FPS 历史记录
        fpsHistory[fpsHistoryIndex] = fps;
        fpsHistoryIndex = (fpsHistoryIndex + 1) % FPS_HISTORY_SIZE;

        // 显示当前 FPS
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "FPS: %.2f", fps);

        // 绘制 FPS 波形图
        ImGui::PlotLines("##fps", fpsHistory.data(), static_cast<int>(FPS_HISTORY_SIZE), static_cast<int>(fpsHistoryIndex), nullptr, 0.0f, 200.0f, ImVec2(0, 80));
    }
    ImGui::End();
}

void DrawCallBack()
{
	DrawFps();

}