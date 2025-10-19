#include "../basic.h"
#include "d3d11Hook.h"
#include "imgui/Font.h"

using namespace ImGui;
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

extern auto ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

inline static bool init;
inline static bool show;
inline static bool tips;

void InitImGui()
{
	dx_hook::HookDx11::Build([&] {
		if (!init) {
			IMGUI_CHECKVERSION();
			// ����ImGui������
			ImGui::CreateContext();
			ImPlot::CreateContext();
			// ����ImGui IO
			auto& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

			io.IniFilename = nullptr;
			io.LogFilename = nullptr;

			// ������������
			io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), 2362740, 15, nullptr, io.Fonts->GetGlyphRangesChineseFull());

			// ��ʼ��ImGui D3D11�豸�ʹ���
			ImGui_ImplWin32_Init(dx_hook::HookDx11::GetHwnd());
			ImGui_ImplDX11_Init(dx_hook::HookDx11::GetDevice(), dx_hook::HookDx11::GetContext());

			ID3D11Texture2D* pBackBuffer{};
			dx_hook::HookDx11::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
			dx_hook::HookDx11::GetDevice()->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
			pBackBuffer->Release();

			io.SetPlatformImeDataFn = nullptr;

			// �ӹܴ�����Ϣ

			dx_hook::HookDx11::SetWndProc([](const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> char {
				// ���������Ϣ
				POINT mPos;
				GetCursorPos(&mPos);
				ScreenToClient(dx_hook::HookDx11::GetHwnd(), &mPos);
				ImGui::GetIO().MousePos.x = static_cast<float>(mPos.x);
				ImGui::GetIO().MousePos.y = static_cast<float>(mPos.y);
				ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

				// �ȼ��ص�

				if (show == true) return false;

				return true;
				});
			init = true;
			show = false;
			tips = true;

		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		// ��ʼ��Ⱦ
		DrawCallBack();
		// ��������Ⱦ
		ImGui::EndFrame();
		ImGui::Render();
		dx_hook::HookDx11::GetContext()->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		});
}