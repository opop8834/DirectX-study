#pragma once
#include "stdafx.h"

namespace Window   // namespace�� ������ ���� ���� �Լ��� ������ ���� ����
{
	static HINSTANCE global_instance;
	static HWND global_handle;

	inline LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)       // inline Ű����� �Լ��� ������ Ÿ�ӿ� ��üȭ��ų �� �ִ�.
	{//stdcall �� ������ ȣ�� �Ծ�
		// calling Convention�� �Լ� ȣ�� ��Ģ�̴�. �޸𸮸� ���糪 ó���ϴ� ����� ���Ѵ�.
		// �� ���� ������Ʈ�� �⺻ Calling Convention�� __cdecl �̴�. �׷��� �����ֱ� ���� CALLBACK�� ���� ���̴�.

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
		wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));   // static_cast�� �⺻ Ÿ�԰��� ����ȯ�� �����ϴ�.
		wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR);
		wnd_class.hIconSm = LoadIcon(nullptr, IDI_ERROR);
		wnd_class.hInstance = hInstance;
		wnd_class.lpfnWndProc = WndProc;   // window�� ������ �����
		wnd_class.lpszClassName = L"D2D11Game";   // ���̵� â�϶��� �տ� L�� ���δ�.
		wnd_class.lpszMenuName = nullptr;
		wnd_class.style = CS_HREDRAW | CS_VREDRAW;  // ����, ����, ���ðŸ� ����
		wnd_class.cbSize = sizeof(WNDCLASSEX);

		auto check = RegisterClassEx(&wnd_class);   // auto Ű����� Modern C++ 11���� ó�� �߰��� ���  �����Ϸ��� �ڵ����� �˾Ƽ� ����
		assert(check != 0);  // true�� �׳� �������� false�� ��Ʈ�� �� �ߴܵȴ�.


		//CreateWindowExA  �ƽ�Ű �ڵ带 ���ų� CreateWindowExW ���̵� â�� ���ų�
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
		ZeroMemory(&msg, sizeof(MSG));   // ZeroMemory�Լ��� �� ������ �ʱ�ȭ�� �����ϰ�
		
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))   // PeekMessage �Լ��� �񵿱� �Լ�, GetMessage �Լ��� �����Լ� �ִ�.
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
		return static_cast<UINT>(rect.bottom - rect.top);   // bottom���� top�� ���� ����
	}
}
