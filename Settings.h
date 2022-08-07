#pragma once

#include "stdafx.h"


// Singletone Pattern   : �ϳ��� �ν��Ͻ��� �����ǵ��� �����ϰ� �������� ������ �� �ְ� �ϱ� ���� ����ϴ� ������ ����
class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;   // static Ű���尡 ���� ���� ������ �� �� ���� �ʱ�ȭ�� �����ϰ� ���� �ʱ�ȭ���� ���� �� 0���� �⺻ �ʱ�ȭ �ȴ�.
		return instance;
	}

	auto GetWindowHandle() const -> HWND
	{ 
		return handle; 
	}
	void SetWindowHandle(HWND handle) {
		this->handle = handle;
	}
	auto GetWidth() const -> const float&   // �Լ� �ڿ� const Ű���带 �پ��ָ� ���ο��� ���Ǵ� ��� ������ ���ȭ �ȴ�.
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
	Settings() = default;   // Modern C++�� default Ű���尡 �߰� �Ǿ��µ� {}�� �׳� ���� �ǹ��̴�.
	~Settings() = default;
	
private:
	HWND handle = nullptr;
	float width = 0.0f;
	float height = 0.0f;
};