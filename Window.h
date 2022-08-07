#pragma once
#include "stdafx.h"

namespace Window   // namespace로 묶으면 쉽게 전역 함수와 변수를 관리 가능
{
	static HINSTANCE global_instance;
	static HWND global_handle;

	inline LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)       // inline 키워드는 함수를 컴파일 타임에 구체화시킬 수 있다.
	{//stdcall 은 일종의 호출 규약
		// calling Convention은 함수 호출 규칙이다. 메모리를 적재나 처리하는 방식을 정한다.
		// 즉 여기 프로젝트의 기본 Calling Convention은 __cdecl 이다. 그래서 맞춰주기 위해 CALLBACK을 써준 것이다.

		switch (message)
		{
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(handle, message, wParam, lParam);
		}
		return 0;
	}

	inline void Create(HINSTANCE hInstance, const UINT& width, const UINT& height) 
	{
		WNDCLASSEX wnd_class;
		wnd_class.cbClsExtra = 0;
		wnd_class.cbWndExtra = 0;
		wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));   // static_cast는 기본 타입간의 형변환이 가능하다.
		wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR);
		wnd_class.hIconSm = LoadIcon(nullptr, IDI_ERROR);
		wnd_class.hInstance = hInstance;
		wnd_class.lpfnWndProc = WndProc;   // window에 연결을 해줘라
		wnd_class.lpszClassName = L"D2D11Game";   // 와이드 창일때는 앞에 L을 붙인다.
		wnd_class.lpszMenuName = nullptr;
		wnd_class.style = CS_HREDRAW | CS_VREDRAW;  // 수직, 수평, 가시거리 길이
		wnd_class.cbSize = sizeof(WNDCLASSEX);

		auto check = RegisterClassEx(&wnd_class);   // auto 키워드는 Modern C++ 11에서 처음 추가된 기능  컴파일러가 자동으로 알아서 붙임
		assert(check != 0);  // true면 그냥 지나가고 false면 터트림 즉 중단된다.


		//CreateWindowExA  아스키 코드를 쓸거냐 CreateWindowExW 와이드 창을 쓸거냐
		global_handle = CreateWindowExW(
			WS_EX_APPWINDOW, 
			L"D2D11Game", 
			L"D2D11Game", 
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT, 
			static_cast<int>(width),
			static_cast<int>(height), 
			nullptr, 
			nullptr, 
			hInstance,
			nullptr);
		assert(global_handle != nullptr);
	}

	inline void Show()
	{
		SetForegroundWindow(global_handle);
		SetFocus(global_handle);
		ShowCursor(TRUE);
		ShowWindow(global_handle, SW_NORMAL);
		UpdateWindow(global_handle);
	}

	inline const bool Update()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));   // ZeroMemory함수로 더 안전한 초기화를 가능하게
		
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))   // PeekMessage 함수는 비동기 함수, GetMessage 함수는 동기함수 있다.
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message != WM_QUIT;

	}
	inline void Destroy()
	{
		DestroyWindow(global_handle);
		UnregisterClass(L"D2D11Game", global_instance);
	}

	inline const UINT GetWidth()
	{
		RECT rect;
		GetClientRect(global_handle, &rect);
		return static_cast<UINT>(rect.right - rect.left);
	}
	inline const UINT GetHeight()
	{
		RECT rect;
		GetClientRect(global_handle, &rect);
		return static_cast<UINT>(rect.bottom - rect.top);   // bottom에서 top을 빼는 순서
	}
}
