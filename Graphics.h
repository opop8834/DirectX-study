#pragma once

class Graphics final    // final 키위도 (Modern C++ 11에 추가) 더는 상속시킬 수 없도록 막아준다.
{
public:
	Graphics();
	~Graphics();

	void Initialize();
	void CreateBackBuffer(const uint& width, const uint& height);

	auto GetDevice() -> ID3D11Device*   // 함수의 반환값 타입이 명확할 경우만 사용가능 
										// 함수의 반환값을 이렇게 -> 사용하여 auto 키워드로 대체할 수 있다.
										// 물론 -> 안써도 되긴 하는데 그냥 예전에는 ->이 꼭 필요했다.
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

	// 포인터가 나오면 무조건적으로 항상 초기화를 해줘야 된다. *
	// 이렇게 헤더파일에서 초기화 하는 것은 Modern C++ 11 부터 가능하다
	// = nullptr로 초기화
	ID3D11Device* device = nullptr;    // I가 앞에 붙어있는 것들을 COM Interface   : Component Object Model  직접 할당을 하지 못하고 알아서 할당하고 해줌 즉 난 이런걸 만들고 싶다고 하면 알아서 내부에서 만들어줌 즉 만들어서 보내준다 Description을 가지고 있어서 설명서가 있다. 
									   // 1. 직접 메모리 할당할 수 없다. 
									   // 2. Release 함수로 메모리를 해제하여야 한다. 
									   // 3. Description이 필요하다
										// ID3D11Device 는 DirectX에서 사용할 자원을 생성한다.

	ID3D11DeviceContext* device_context = nullptr;    // Rendering Pipeline  랜더링을 하기 위해 수행되어야 하는 단계들 
													  // 단계는 IA - VS - RS - PS - OM 
													  // ID3D11DeviceContext는 Pipeline에 세팅을 한다.


	IDXGISwapChain* swap_chain = nullptr;          // DXGI : DirectX Graphics Infrastructure  제조사마다 다른 그래픽 카드를 프로그래머가 편하게 사용할 수 있도록 도와주는 역할을 해준다. 즉 DXGI에게 명령을 주기만 하면 알아서 해준다, 
										 //swapChain : 두 개의 버퍼를 번갈아 가며 랜더링하여 깜빡임, 찢어짐 현상 방지해준다.  
										 // 즉 ID3D11SwapChain은 백 버퍼를 관리하는 DXGI의 한 종류이다.
										// 도화지를 그릴때 필요한 지지대 같은 역할을 한다.
	
	ID3D11RenderTargetView* render_target_view = nullptr;         // 그림을 그릴 도화지이다.
	D3D11_VIEWPORT viewport = { 0 };                     // 도화지 안에서 어디 영역을 그릴 것인지,   I가 없는게 특징.
	
	D3DXCOLOR clear_color = 0xff555566;         // 4byte 크기를 가진 것 16진수 표현으로 쉽게 파악 가능          
												// 색깔을 표현하는 데이터  8bit 까지 즉 0부터 255까지 색깔을 표현  
												 // float 형식이라서 정규화 과정이 필요하다. 즉 0부터 1까지로 범위를 줄여준다.  
												 // 색깔을 그냥 다같이 입혀준다는 느낌으로 clear color이다. 도화지랑 같은 하얀색으로 입히면 사라지게 되어서 
												 // 16진수로 색상 정보는 앞에서부터 두 개씩 Alpha, Red, Green, Blue 채널을 의미한다.
};
