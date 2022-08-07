#include "stdafx.h"
#include "Window.h"
#include "Execute.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR ipszCmdParam, int nCmdShow)
{
	Window::Create(hInstance, 500, 500);
	Window::Show();

	Settings::Get().SetWindowHandle(Window::global_handle);  // Settings 는 Get() 함수를 통해 객체 인스턴스를 얻어 접근해야 한다.
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));
	
	Execute* execute = new Execute();

	while (Window::Update())
	{
		execute->Update();
		execute->Render();
	}

	SAFE_DELETE(execute);
	Window::Destroy();
	return 0;
}
