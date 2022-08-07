#pragma once

#include "stdafx.h"


// Singletone Pattern   : 하나의 인스턴스만 생성되도록 보장하고 전역에서 접근할 수 있게 하기 위해 사용하는 디자인 패턴
class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;   // static 키워드가 붙은 정적 변수는 단 한 번만 초기화를 수행하고 만약 초기화하지 않을 시 0으로 기본 초기화 된다.
		return instance;
	}

	auto GetWindowHandle() const -> HWND
	{ 
		return handle; 
	}
	void SetWindowHandle(HWND handle) {
		this->handle = handle;
	}
	auto GetWidth() const -> const float&   // 함수 뒤에 const 키워드를 붙어주면 내부에서 사용되는 멤버 변수가 상수화 된다.
	{
		return width;
	}
	void SetWidth(const float& width) {
		this-> width = width;
	}
	auto GetHeight() const -> const float&  
	{
		return height;
	}
	void SetHeight(const float& height) {
		this-> height = height;
	}

private:
	Settings() = default;   // Modern C++에 default 키워드가 추가 되었는데 {}랑 그냥 같은 의미이다.
	~Settings() = default;
	
private:
	HWND handle = nullptr;
	float width = 0.0f;
	float height = 0.0f;
};