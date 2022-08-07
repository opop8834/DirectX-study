#pragma once

class Graphics final    // final Ű���� (Modern C++ 11�� �߰�) ���� ��ӽ�ų �� ������ �����ش�.
{
public:
	Graphics();
	~Graphics();

	void Initialize();
	void CreateBackBuffer(const uint& width, const uint& height);

	auto GetDevice() -> ID3D11Device*   // �Լ��� ��ȯ�� Ÿ���� ��Ȯ�� ��츸 ��밡�� 
										// �Լ��� ��ȯ���� �̷��� -> ����Ͽ� auto Ű����� ��ü�� �� �ִ�.
										// ���� -> �Ƚᵵ �Ǳ� �ϴµ� �׳� �������� ->�� �� �ʿ��ߴ�.
	{
		return device;
	}
	auto GetDeviceContext() -> ID3D11DeviceContext*
	{
		return device_context;
	}

	void Begin();
	void End();


private:

	// �����Ͱ� ������ ������������ �׻� �ʱ�ȭ�� ����� �ȴ�. *
	// �̷��� ������Ͽ��� �ʱ�ȭ �ϴ� ���� Modern C++ 11 ���� �����ϴ�
	// = nullptr�� �ʱ�ȭ
	ID3D11Device* device = nullptr;    // I�� �տ� �پ��ִ� �͵��� COM Interface   : Component Object Model  ���� �Ҵ��� ���� ���ϰ� �˾Ƽ� �Ҵ��ϰ� ���� �� �� �̷��� ����� �ʹٰ� �ϸ� �˾Ƽ� ���ο��� ������� �� ���� �����ش� Description�� ������ �־ ������ �ִ�. 
									   // 1. ���� �޸� �Ҵ��� �� ����. 
									   // 2. Release �Լ��� �޸𸮸� �����Ͽ��� �Ѵ�. 
									   // 3. Description�� �ʿ��ϴ�
										// ID3D11Device �� DirectX���� ����� �ڿ��� �����Ѵ�.

	ID3D11DeviceContext* device_context = nullptr;    // Rendering Pipeline  �������� �ϱ� ���� ����Ǿ�� �ϴ� �ܰ�� 
													  // �ܰ�� IA - VS - RS - PS - OM 
													  // ID3D11DeviceContext�� Pipeline�� ������ �Ѵ�.


	IDXGISwapChain* swap_chain = nullptr;          // DXGI : DirectX Graphics Infrastructure  �����縶�� �ٸ� �׷��� ī�带 ���α׷��Ӱ� ���ϰ� ����� �� �ֵ��� �����ִ� ������ ���ش�. �� DXGI���� ����� �ֱ⸸ �ϸ� �˾Ƽ� ���ش�, 
										 //swapChain : �� ���� ���۸� ������ ���� �������Ͽ� ������, ������ ���� �������ش�.  
										 // �� ID3D11SwapChain�� �� ���۸� �����ϴ� DXGI�� �� �����̴�.
										// ��ȭ���� �׸��� �ʿ��� ������ ���� ������ �Ѵ�.
	
	ID3D11RenderTargetView* render_target_view = nullptr;         // �׸��� �׸� ��ȭ���̴�.
	D3D11_VIEWPORT viewport = { 0 };                     // ��ȭ�� �ȿ��� ��� ������ �׸� ������,   I�� ���°� Ư¡.
	
	D3DXCOLOR clear_color = 0xff555566;         // 4byte ũ�⸦ ���� �� 16���� ǥ������ ���� �ľ� ����          
												// ������ ǥ���ϴ� ������  8bit ���� �� 0���� 255���� ������ ǥ��  
												 // float �����̶� ����ȭ ������ �ʿ��ϴ�. �� 0���� 1������ ������ �ٿ��ش�.  
												 // ������ �׳� �ٰ��� �����شٴ� �������� clear color�̴�. ��ȭ���� ���� �Ͼ������ ������ ������� �Ǿ 
												 // 16������ ���� ������ �տ������� �� ���� Alpha, Red, Green, Blue ä���� �ǹ��Ѵ�.
};
