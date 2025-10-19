// con_log_inj.hpp  —— 注入专用版
#pragma once
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#endif

namespace detail
{
    // 取纯文件名
    inline const char* filename_only(const char* path)
    {
        const char* s = strrchr(path, '\\');
        if (s) return s + 1;
        s = strrchr(path, '/');
        return s ? s + 1 : path;
    }

    // 真正开启 VT（独立控制台句柄）
    inline void enable_vt()
    {
        HANDLE h = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr, OPEN_EXISTING, 0, nullptr);
        if (h == INVALID_HANDLE_VALUE) return;
        DWORD mode = 0;
        if (GetConsoleMode(h, &mode))
            SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        CloseHandle(h);
    }

    inline void conalloc()
    {
        static bool done = false;
        if (done) return;
#ifdef _WIN32
        if (AllocConsole())
        {
            // 必须重新打开流，否则句柄指向宿主原输出
            FILE* dummy;
            freopen_s(&dummy, "CONOUT$", "w", stdout);
            freopen_s(&dummy, "CONOUT$", "w", stderr);
            freopen_s(&dummy, "CONIN$", "r", stdin);
            // 禁用缓冲，防止输出滞留
            setvbuf(stdout, nullptr, _IONBF, 0);
            setvbuf(stderr, nullptr, _IONBF, 0);
            // 关键：开启 VT
            enable_vt();
        }
#endif
        done = true;
    }

    enum class lvl { err, ok, info };
    inline const char* color(lvl l)
    {
        switch (l)
        {
        case lvl::err:  return "\033[31m";
        case lvl::ok:   return "\033[32m";
        case lvl::info: return "\033[90m";
        default:        return "";
        }
    }

    inline void print(lvl l, const char* file, int line, const char* fmt, ...)
    {
        conalloc();
        const char* tag = l == lvl::err ? "ERROR" :
            l == lvl::ok ? "SUCCESS" : "INFO";
        std::time_t t = std::time(nullptr);
        std::tm tm;
#if defined(_WIN32)
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        char timestr[16];
        std::strftime(timestr, sizeof(timestr), "%H:%M:%S", &tm);

        // 去掉了所有装饰符，只剩 [类型] 文件名:行 时间 内容
        std::printf("%s[%s] %s:%d  %s  ", color(l), tag,
            filename_only(file), line, timestr);

        va_list args;
        va_start(args, fmt);
        std::vprintf(fmt, args);
        va_end(args);

        std::printf("\033[0m\n");
    }
} // namespace detail

#define log_error(...) detail::print(detail::lvl::err,  __FILE__, __LINE__, __VA_ARGS__)
#define log_ok(...)    detail::print(detail::lvl::ok,   __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  detail::print(detail::lvl::info, __FILE__, __LINE__, __VA_ARGS__)