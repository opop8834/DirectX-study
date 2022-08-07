#pragma once

#define NOMINMAX   // <Windows.h>를 include 하기 전에 정의해야 한다.

// _DEBUG 메소드 : Debug 모드 에서만 정의됨
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")  // 테스트용 콘솔 창 생성가능

#endif // DEBUG


//window
#include <Windows.h>
#include <assert.h>  // 디버깅 할때 실행된다.

//STL
#include <iostream> // 입출력
#include <vector>  // Vector는 C++에서 제공하는 STL로서 배열과 비슷한 시퀀스 컨테이너 입니다.

//DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>


// #pragma commnet 라이브러리 파일을 연결시켜주는 전처리기 문법  세미클론이 없다.
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")

typedef unsigned int uint;         // typedef는 자료형에 또 다른 별명을 붙여주는 키워드다


// Macro Function   동적 할당된 메모리를 지우는 함수,  매크로 함수는 중괄호를 사용하여 묶어주는 것이 좋다.
// 왜 여러줄로 작성하면 오류가 나고 한줄로 작성해야 오류가 안날까?
#define SAFE_DELETE(p) {if (p){delete(p);(p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if (p){delete[](p);(p) = nullptr;}}
#define SAFE_RELEASE(p) {if (p){(p)->Release();(p) = nullptr;}}

// Framework
#include "Settings.h"