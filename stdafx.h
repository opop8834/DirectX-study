#pragma once

#define NOMINMAX   // <Windows.h>�� include �ϱ� ���� �����ؾ� �Ѵ�.

// _DEBUG �޼ҵ� : Debug ��� ������ ���ǵ�
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")  // �׽�Ʈ�� �ܼ� â ��������

#endif // DEBUG


//window
#include <Windows.h>
#include <assert.h>  // ����� �Ҷ� ����ȴ�.

//STL
#include <iostream> // �����
#include <vector>  // Vector�� C++���� �����ϴ� STL�μ� �迭�� ����� ������ �����̳� �Դϴ�.

//DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>


// #pragma commnet ���̺귯�� ������ ��������ִ� ��ó���� ����  ����Ŭ���� ����.
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")

typedef unsigned int uint;         // typedef�� �ڷ����� �� �ٸ� ������ �ٿ��ִ� Ű�����


// Macro Function   ���� �Ҵ�� �޸𸮸� ����� �Լ�,  ��ũ�� �Լ��� �߰�ȣ�� ����Ͽ� �����ִ� ���� ����.
// �� �����ٷ� �ۼ��ϸ� ������ ���� ���ٷ� �ۼ��ؾ� ������ �ȳ���?
#define SAFE_DELETE(p) {if (p){delete(p);(p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if (p){delete[](p);(p) = nullptr;}}
#define SAFE_RELEASE(p) {if (p){(p)->Release();(p) = nullptr;}}

// Framework
#include "Settings.h"