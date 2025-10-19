#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <array>
#include <algorithm>
#include <cmath>

#include "con_log.hpp"
#include "struct.h"
#include "PtrMemory.h"
#include "config.h"
#include "Dx11Lib/d3d11Hook.h"
#include "Dx11Lib/imgui/imgui_impl_win32.h"
#include "Dx11Lib/imgui/imgui_impl_dx11.h"
#include "Dx11Lib/imgui/implot.h"

void InitImGui();
void DrawCallBack();