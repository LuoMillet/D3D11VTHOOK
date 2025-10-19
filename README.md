# D3D11VTHOOK

A D3D11 virtual table hook library for rendering, which can avoid thread detection of hook rendering to a certain extent.

## 项目简介 | Project Introduction

D3D11VTHOOK 是一个基于 DirectX 11 的虚拟表钩子绘制库，通过钩子技术实现了在目标进程中的渲染功能。该库特别设计用于避免传统钩子绘制方法中常见的线程检测问题，提供了一种更隐蔽、更安全的渲染解决方案。

D3D11VTHOOK is a virtual table hook library based on DirectX 11 for rendering purposes. It implements rendering functionality in target processes through hook technology. This library is specifically designed to avoid thread detection issues commonly found in traditional hook rendering methods, providing a more stealthy and secure rendering solution.

![D3D11 Rendering Demo](https://youke1.picui.cn/s1/2025/10/19/68f4496e6b4aa.png)

## 核心功能 | Core Features

- **VTable 钩子技术**：直接钩子 D3D11 设备的虚拟函数表，实现渲染注入
- **线程检测规避**：特殊设计的渲染机制，可在一定程度上避免线程检测
- **完整渲染功能**：支持 2D/3D 图形绘制、文本渲染等基本渲染功能
- **内存读写封装**：提供便捷的内存读写模板，简化外部进程内存操作
- **终端输出**：集成终端输出功能，便于调试和信息展示

- **VTable Hook Technology**: Directly hooks into the virtual function table of D3D11 devices to implement rendering injection
- **Thread Detection Evasion**: Specially designed rendering mechanism that can avoid thread detection to a certain extent
- **Complete Rendering Functions**: Supports 2D/3D graphics drawing, text rendering and other basic rendering functions
- **Memory Read/Write Wrappers**: Provides convenient memory read/write templates to simplify external process memory operations
- **Console Output**: Integrated terminal console output functionality for debugging and information display

## 编译环境 | Compilation Environment

- **编译器**: MSVC 2017 或更高版本 (支持 C++17 标准)
- **Windows SDK**: Windows 10 SDK 或更高版本
- **DirectX SDK**: 包含 DirectX 11 头文件和库
- **编译选项**: 可能需要禁用 C4996 警告 (`/wd4996`)

- **Compiler**: MSVC 2017 or higher (supports C++17 standard)
- **Windows SDK**: Windows 10 SDK or higher
- **DirectX SDK**: Includes DirectX 11 headers and libraries
- **Compile Options**: May need to disable C4996 warning (`/wd4996`)

## 快速开始 | Quick Start

### 1. 编译库 | Compile the Library

```bash
# 克隆项目
git clone https://github.com/LuoMillet/D3D11VTHOOK.git

# 使用 Visual Studio 打开解决方案
# 选择合适的配置 (Debug/Release) 和平台 (x86/x64)
# 编译项目
```

### 2. 集成到目标项目 | Integrate into Target Project

```cpp
#include "D3D11VTHook.h"

// 初始化钩子
D3D11VTHook hook;
if (hook.Initialize())
{
    // 钩子初始化成功
    printf("D3D11 hook initialized successfully!\n");
    
    // 设置自定义渲染回调
    hook.SetRenderCallback([](ID3D11DeviceContext* pContext) {
        // 在这里你的渲染代码
        // Your rendering code here
    });
}
else
{
    printf("Failed to initialize D3D11 hook!\n");
}

// 主循环
while (true)
{
    // 执行钩子逻辑
    hook.Run();
    
    // 其他逻辑
    // Other logic
}

// 卸载钩子
hook.Uninstall();
```

## API 参考 | API Reference

### D3D11VTHook 类 | D3D11VTHook Class

```cpp
// 初始化 D3D11 钩子
bool Initialize();

// 卸载钩子
void Uninstall();

// 设置渲染回调函数
void SetRenderCallback(std::function<void(ID3D11DeviceContext*)> callback);

// 主循环执行
void Run();

// 检查钩子是否激活
bool IsHooked() const;
```

### 内存操作模板 | Memory Operation Templates

```cpp
// 读取内存
template <typename T>
T ReadMemory(HANDLE hProcess, DWORD_PTR address);

// 写入内存
template <typename T>
bool WriteMemory(HANDLE hProcess, DWORD_PTR address, const T& value);

// 查找模块基址
DWORD_PTR FindModuleBaseAddress(DWORD processId, const std::string& moduleName);
```

### 终端输出 | Console Output

```cpp
// 创建控制台
void CreateConsole();

// 控制台输出
void ConsolePrint(const char* format, ...);
```

## 使用示例 | Usage Examples

### 基本渲染示例 | Basic Rendering Example

```cpp
hook.SetRenderCallback([](ID3D11DeviceContext* pContext) {
    // 清除简单的矩形绘制
    DrawRectangle(pContext, 100, 100, 200, 150, Color(255, 0, 0, 255));
    
    // 绘制文本
    DrawText(pContext, "Hello D3D11VTHOOK!", 50, 50, Color(0, 255, 0, 255));
});
```

### 内存读写示例 | Memory Read/Write Example

```cpp
// 查找目标进程
DWORD processId = FindProcessId("target.exe");
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

if (hProcess)
{
    // 查找模块基址
    DWORD_PTR moduleBase = FindModuleBaseAddress(processId, "target.exe");
    
    // 读取内存
    int health = ReadMemory<int>(hProcess, moduleBase + 0x123456);
    ConsolePrint("Current health: %d\n", health);
    
    // 写入内存
    WriteMemory<int>(hProcess, moduleBase + 0x123456, 100);
    ConsolePrint("Health set to 100\n");
    
    CloseHandle(hProcess);
}
```

## 注意事项 | Notes

1. **兼容性**：目前库仅支持 DirectX 11 应用程序，不支持 DirectX 9/10/12 或 OpenGL 应用
2. **管理员权限**：某些目标进程可能需要管理员权限才能成功注入和钩子
3. **反作弊系统**：本库仅用于学习和研究，请勿用于在线游戏或其他受反作弊保护的环境
4. **线程安全**：渲染回调函数将在 DirectX 线程中执行，请注意线程安全

1. **Compatibility**: This library only supports DirectX 11 applications, not DirectX 9/10/12 or OpenGL applications
2. **Admin Rights**: Some target processes may require admin rights for successful injection and hooking
3. **Anti-Cheat Systems**: This library is for learning and research purposes only, do not use in online games or other anti-cheat protected environments
4. **Thread Safety**: The render callback function will execute in the DirectX thread, please attention to thread safety

## 许可证 | License

**本项目仅限用于学习和交流，禁止商业用途途和非法用途。**

**This project is for learning and communication purposes only, and commercial use and illegal use are prohibited.**

## 免责声明 | Disclaimer

使用本项目的风险由用户自行承担。作者不对因使用本项目而产生的任何直接或间接损失负责。使用者应遵守当地法律法规，不得将本项目用于任何非法活动。

Users assume all risks when using this project. The author is not responsible for any direct or indirect damages arising from the use of this project. Users should comply with local laws and regulations and must not use this project for any illegal activities.
