#pragma once

#if defined(_WIN32) && !defined(_DEBUG)
#define NOMINMAX
#include <Windows.h>
#endif // _WIN32 && !_DEBUG

void MagMain();

#if defined(_WIN32) && !defined(_DEBUG)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
#else
int main()
#endif // _WIN32 && !_DEBUG
{
    MagMain();

    return 0;
}
