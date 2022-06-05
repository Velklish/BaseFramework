#pragma once
#include "Forward.h"

namespace BaseFramework {
	class Window
	{
	public:
		Window(int width, int height, LPCWSTR name, BaseFramework::IGame* instance);
		void InitializeMessageLoop();
		void GetSize(int& width, int& height);
		HWND GetHwnd() { return hwnd; };
	private:
		IGame* instance = nullptr;
		HWND hwnd = nullptr;
		int width;
		int height;
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	};
}

