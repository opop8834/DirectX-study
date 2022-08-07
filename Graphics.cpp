#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	// ���� �Ҵ�� �޸𸮴� ���� ���� ���� �������� ����� ���� �� �����ϴ�.
	// �����Ҵ�� �޸𸮵��� �ٸ� ������� �����ϰ� �����Ǿ� ���� ���ɼ��� �־ dangling pointer ������ �Ͼ ���� �ִ�.
	// dangling pointer : �����Ͱ� ������ �޸� ������ ����Ű�� �ִ� ���
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
	desc.BufferDesc.RefreshRate.Numerator = 60;   // refresh rate ȭ�� �ֻ��� 
												  // Numerator �� ����
	desc.BufferDesc.RefreshRate.Denominator = 1;   // Denominator �и�
													// �� 60 Frame : 1�ʿ� 60�� ȭ���� ����
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �� ä�� R,G,B,A�� 8��Ʈ�� �Ҵ��ϰڴ�.  unsigned NORM ����ȭ �ϰڴ�.
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;   // Ȯ�� , ��� �ɶ� ȿ��
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���۸� ��� ����Ұǰ���?
	desc.BufferCount = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// Aliasing : �ȼ� ǥ������ ����� �������
	// Anti Aliasing : ��� ������ �����ϴ� ���
	// SSAA, MSAA ����� �ִ�
	// SSAA�� �̹����� ��û Ű�� ������ ��ҽ��Ѽ� ������� ���� - ����� ���� ���ؼ� �� �Ⱦ���
	// MSAA�� Ư�� �κи� SASS�� ������ - ����� ũ�� ������ SASS���ٴ� ����


	desc.OutputWindow = Settings::Get().GetWindowHandle();
	desc.Windowed = TRUE;   // â��� �Ұ��̳� ���Ұ��̳�
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	std::vector<D3D_FEATURE_LEVEL> feature_levels
	{
		D3D_FEATURE_LEVEL_11_1,   // � ������ �� ������ �� ������ ������ ������ ������
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	auto hr = D3D11CreateDeviceAndSwapChain(  // HRESULT�� �Լ��� ����� �����ߴ��� �����ߴ����� ������ ����ִ�.
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,         // D3D_DRIVER_TYPE_HARDWARE  : �ϵ���� ������ ����� �� �ְ� ���ش�.
		nullptr,
		0,
		feature_levels.data(),   // ���� �ּҸ� ��ȯ
		feature_levels.size(),
		D3D11_SDK_VERSION,
		&desc,
		&swap_chain,      // ���� �Ҵ��ϴ� ����� �ƴ϶� ���� ������ ���,�Ҵ��� �ּҸ� �ѱ�� �°� �ű� �ȿ� �Ҵ����ֱ� ������
		&device,
		nullptr,
		&device_context
	);

	assert(SUCCEEDED(hr));

}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{
	auto hr = swap_chain->ResizeBuffers             // ResizeBuffers() �Լ� ���� �ʱ� ���� �ٸ� ������ ���� �־��ָ� �ȴ�.
	(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0
	);
	assert(SUCCEEDED(hr));

	// ID3D11SwapChain�� ���ؼ� BackBuuffer ������ �� �� �ֽ��ϴ�.
	ID3D11Texture2D* back_buffer = nullptr;

	hr = swap_chain->GetBuffer    // IID : Interface Identifier
	(
		0,
		__uuidof(ID3D11Texture2D),    //IID_ID3D11Texture2D,    // UUID,GUID �� 16byte ũ�⸦ ���� �����ʹ� ��û ũ�⶧���� �ߺ��� ���� ��� ���� �ĺ��ڶ� �Ҹ���.  
		reinterpret_cast<void**>(&back_buffer)   // void�� ����ȯ �Ҷ�
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		back_buffer,
		nullptr,
		&render_target_view
	);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0.0f;  // ���� ���
	viewport.TopLeftY = 0.0f;
	viewport.Width  = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;   // �⺻������ �β��� ����

	//// �������� ���� �Ҵ�Ǵ� �޸𸮴� �׻� �������� �������־�� �մϴ�.
	//if(back_buffer != nullptr)
	//	back_buffer->Release();  // ���������� delete�� ���� ���� ����. �Ҵ��� �������� �ʾƼ�

	SAFE_RELEASE(back_buffer);   // release�� �Լ��� ����� �־���.

}

// �ϴ� ��ȭ�� �ĸ鿡�� �׸��� �׸���.
void Graphics::Begin()  // �׸��⸦ ������ �� ȣ��Ǳ⿡ ���� ���ð� �ʱ�ȭ �۾��� ���� �Ѵ�.
{
//	ID3D11RenderTargetView* rtvs[] = { render_target_view };  // ������ �������� �迭�� �־���� �ȴ�.
//	device_context->OMSetRenderTargets(1, rtvs);   // OM: Output Merger Stage  
											 // Output Merger Stage�� ���õ� �� �ִ� RenderTarget�� ������ �� 8�� �Դϴ�.
	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);  // �ٵ� �׳� �ּҸ� �־��൵ ������ �ϳ�¥�� �迭�̶� ����
	device_context->RSSetViewports(1, &viewport);
	device_context->ClearRenderTargetView(render_target_view, clear_color);  // D3DXCOLOR ���� float �迭 ���·� �Ͻ��� ĳ������ �Ǵ� �����ڰ� ����Ǿ� �־ ����ȯ �����൵ ��
}

// ���� �ĸ� ��ȭ���� ���� ��ȭ���� ���;� �ȴ�.
void Graphics::End()
{
	auto hr = swap_chain->Present(1,0);    // SwapChain�� Present() �ĸ� ���۸� ���� ���۷� ������ݴϴ�.
	assert(SUCCEEDED(hr));
}



