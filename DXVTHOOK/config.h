#pragma once
#include "basic.h"
namespace BasicConfig
{
	inline uintptr_t GameBase = (uintptr_t)GetModuleHandle(L"GameAssembly.dll");
	inline std::string WindowName = "心动小镇";
	inline std::string WindowClassName = "UnityWndClass";
	inline auto GameHandle = FindWindowA(WindowClassName.c_str(), WindowName.c_str());
	inline Vector2 Sight = { 0.0f,0.0f };


}