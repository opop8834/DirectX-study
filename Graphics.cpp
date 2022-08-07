#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	// 동적 할당된 메모리는 먼저 만든 것을 마지막에 지우는 것이 더 안전하다.
	// 먼저할당된 메모리들이 다른 변수들과 밀접하게 연관되어 있을 가능성이 있어서 dangling pointer 현상이 일어날 수도 있다.
	// dangling pointer : 포인터가 해제된 메모리 영역을 가리키고 있는 경우
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(device);
	SAFE_RELEASE(device_context);
	SAFE_RELEASE(swap_chain);
}

void Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.RefreshRate.Numerator = 60;   // refresh rate 화면 주사율 
												  // Numerator 는 분자
	desc.BufferDesc.RefreshRate.Denominator = 1;   // Denominator 분모
													// 즉 60 Frame : 1초에 60번 화면을 갱신
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 각 채널 R,G,B,A당 8비트씩 할당하겠다.  unsigned NORM 정규화 하겠다.
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;   // 확대 , 축소 될때 효과
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼를 어떻게 사용할건가요?
	desc.BufferCount = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// Aliasing : 픽셀 표현으로 생기는 계단현상
	// Anti Aliasing : 계단 현상을 제거하는 방식
	// SSAA, MSAA 등등이 있다
	// SSAA는 이미지를 엄청 키운 다음에 축소시켜서 계단현상 수정 - 비용이 워낙 강해서 잘 안쓰임
	// MSAA는 특정 부분만 SASS를 적용함 - 비용이 크긴 하지만 SASS보다는 적음


	desc.OutputWindow = Settings::Get().GetWindowHandle();
	desc.Windowed = TRUE;   // 창모드 할것이냐 안할것이냐
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	std::vector<D3D_FEATURE_LEVEL> feature_levels
	{
		D3D_FEATURE_LEVEL_11_1,   // 어떤 버전을 쓸 것인지 이 버전이 없으면 밑으로 내려감
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	auto hr = D3D11CreateDeviceAndSwapChain(  // HRESULT는 함수의 결과가 성공했는지 실패했는지의 정보를 담고있다.
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,         // D3D_DRIVER_TYPE_HARDWARE  : 하드웨어 가속을 사용할 수 있게 해준다.
		nullptr,
		0,
		feature_levels.data(),   // 시작 주소를 반환
		feature_levels.size(),
		D3D11_SDK_VERSION,
		&desc,
		&swap_chain,      // 직접 할당하는 방식이 아니라서 이중 포인터 사용,할당할 주소를 넘기고 걔가 거기 안에 할당해주기 때문에
		&device,
		nullptr,
		&device_context
	);

	assert(SUCCEEDED(hr));

}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{
	auto hr = swap_chain->ResizeBuffers             // ResizeBuffers() 함수 에는 초기 값과 다른 변경할 값만 넣어주면 된다.
	(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0
	);
	assert(SUCCEEDED(hr));

	// ID3D11SwapChain을 통해서 BackBuuffer 정보를 얻어낼 수 있습니다.
	ID3D11Texture2D* back_buffer = nullptr;

	hr = swap_chain->GetBuffer    // IID : Interface Identifier
	(
		0,
		__uuidof(ID3D11Texture2D),    //IID_ID3D11Texture2D,    // UUID,GUID 는 16byte 크기를 가진 데이터다 엄청 크기때문에 중복이 거의 없어서 고유 식별자라 불린다.  
		reinterpret_cast<void**>(&back_buffer)   // void로 형변환 할때
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		back_buffer,
		nullptr,
		&render_target_view
	);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0.0f;  // 왼쪽 상단
	viewport.TopLeftY = 0.0f;
	viewport.Width  = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;   // 기본적으로 두께는 존재

	//// 지역에서 동적 할당되는 메모리는 항상 지역에서 해제해주어야 합니다.
	//if(back_buffer != nullptr)
	//	back_buffer->Release();  // 직접적으로 delete로 지울 수는 없다. 할당을 직접하진 않아서

	SAFE_RELEASE(back_buffer);   // release를 함수로 만들어 주었다.

}

// 일단 도화지 후면에다 그림을 그린다.
void Graphics::Begin()  // 그리기를 시작할 때 호출되기에 버퍼 세팅과 초기화 작업이 들어가야 한다.
{
//	ID3D11RenderTargetView* rtvs[] = { render_target_view };  // 원래는 포인터의 배열을 넣어줘야 된다.
//	device_context->OMSetRenderTargets(1, rtvs);   // OM: Output Merger Stage  
											 // Output Merger Stage에 세팅될 수 있는 RenderTarget의 개수는 총 8장 입니다.
	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);  // 근데 그냥 주소를 넣어줘도 일종의 하나짜리 배열이라서 가능
	device_context->RSSetViewports(1, &viewport);
	device_context->ClearRenderTargetView(render_target_view, clear_color);  // D3DXCOLOR 에는 float 배열 형태로 암시적 캐스팅이 되는 연산자가 내장되어 있어서 형변환 안해줘도 됨
}

// 이제 후면 도화지가 전면 도화지에 나와야 된다.
void Graphics::End()
{
	auto hr = swap_chain->Present(1,0);    // SwapChain의 Present() 후면 버퍼를 전면 버퍼로 출력해줍니다.
	assert(SUCCEEDED(hr));
}



