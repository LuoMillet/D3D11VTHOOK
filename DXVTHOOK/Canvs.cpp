#include "basic.h"
#include "./Dx11Lib/d3d11Hook.h"


constexpr size_t FPS_HISTORY_SIZE = 120; // ������� 120 ֡�� FPS ֵ
std::array<float, FPS_HISTORY_SIZE> fpsHistory = {};
size_t fpsHistoryIndex = 0;
void DrawFps()
{
    if (ImGui::Begin("FPS Display", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing))
    {
        // ��ȡ��ǰ FPS
        auto fps = ImGui::GetIO().Framerate;

        // ���� FPS ��ʷ��¼
        fpsHistory[fpsHistoryIndex] = fps;
        fpsHistoryIndex = (fpsHistoryIndex + 1) % FPS_HISTORY_SIZE;

        // ��ʾ��ǰ FPS
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "FPS: %.2f", fps);

        // ���� FPS ����ͼ
        ImGui::PlotLines("##fps", fpsHistory.data(), static_cast<int>(FPS_HISTORY_SIZE), static_cast<int>(fpsHistoryIndex), nullptr, 0.0f, 200.0f, ImVec2(0, 80));
    }
    ImGui::End();
}

void DrawCallBack()
{
	DrawFps();

}