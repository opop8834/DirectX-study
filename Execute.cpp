#include "stdafx.h"
#include "Execute.h"
#include "Graphics.h"  // 헤더 파일 추가

Execute::Execute()
{
	// 불완전한 형식이라는 것은 컴파일러가 해당 자료형을 모르기 때문에 해당 자료형의 헤더 파일을 포함해준다.
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));



//			|
//	1		|		3
//			|
//----------|------------   //시계 방향으로 회전하는 직각 삼각형을 만들 것이다.
//			|				시계 방향으로 감기는 사각형
//	0		|		2       시계 방향이면 기본적으로 앞면 이라는 것을 의미한다. 즉 뒷면은 반시계
//			|

	// Vertex Data
	//{
	//	vertices = new VertexColor[4];   // 메모리를 할당 했으면 무조건 지우자
	//	vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);  //0
	//	vertices[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//	vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);  //1
	//	vertices[1].color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	//	vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);  //2
	//	vertices[2].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//	vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);  //3
	//	vertices[3].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//	// 6개의 정점에서 중복되는 걸 뺀 4개의 정점을 이용해보자
	//}
	{
		vertices = new VertexTexture[4];   // 메모리를 할당 했으면 무조건 지우자
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);  //0
		vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);         // uv좌표는 이미지 파일이 붙을 자료이다. 
														  // 그리고 텍스쳐 좌표계는 좌측 상단이 시작점이고 0~1 범위로 정규화되어 있다.

		vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);  //1
		vertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);   // 1번 위치가 0,0

		vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);  //2
		vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);

		vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);  //3
		vertices[3].uv = D3DXVECTOR2(1.0f, 0.0f);
	}
	
	// Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;   //  Usage : GPU 와 CPU중 누가 데이터를 읽거나 쓸 것인지 결정하는 플래그 
											  //  D3D11_USAGE_DEFAULT : GPU만 데이터를 읽고 쓸 수 있다.  CPU는 X 
											  //  D3D11_USAGE_IMMUTABLE : GPU만 데이터를 읽을 수 있다. (쓰기는 불가능)
		
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // Buffer형 데이터를 어느 스테이지에서 사용할 건지 지정해주는 플래그다.
		
	    //desc.ByteWidth = sizeof(VertexColor) * 4;
		desc.ByteWidth = sizeof(VertexTexture) * 4;

		D3D11_SUBRESOURCE_DATA sub_data;   // CPU 데이터를 GPU에 연결 시킬 수 있다.
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
		sub_data.pSysMem = vertices;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &sub_data, &vertex_buffer);
		assert(SUCCEEDED(hr));
	}

	//Index Data
	{
		// 중괄호를 통하여 바로 할당이 가능하다.
		indices = new uint[6]
		{
			0, 1, 2, 2, 1, 3
		};
	}

	//Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(uint) * 6;

		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
		sub_data.pSysMem = indices;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &sub_data, &index_buffer);
		assert(SUCCEEDED(hr));
	}


	//Vertex Shader
	{
		auto hr = D3DX11CompileFromFileA // D3DX11로 시작하는 함수들은 최신 DirectX SDK에는 지원하지 않는다.
		(
			//"Color.hlsl",
			"Texture.hlsl",
			nullptr,
			nullptr,
			"VS",
			"vs_5_0",
			0,
			0,
			nullptr,
			&vs_blob,
			nullptr,
			nullptr
		);
		assert(SUCCEEDED(hr));

		// 꼭 기억하기 DirexX의 자원을 만들때는 ID3D11Device를 사용한다.
		hr = graphics->GetDevice()->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader);
		assert(SUCCEEDED(hr));
	}

	// Vertex Buffer와 Vertex Shader의 입력 데이터는 항상 일치하여야 한다.
	// 즉 VS를 위에서 만들고 아래에 이 값들을 inputlayout에서 비교하여 값이 다르면 펑 터트린다.

	// Input Layout
	// 정점의 정보가 바뀌면 Input Layout도 바뀌어야 한다.
	{
		//D3D11_INPUT_ELEMENT_DESC layout_desc[]

		//	// Instancing : 하나의 오브젝트에 대한 여러 개의 인스턴스를 한 번의 그리기 호출로 렌더링 하는 방식
		//{                          // 4BYTE의 RGB형식을 가진 FLOAT
		//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},   // vector3 형식으로 넘겨도 자동으로 뒤에 하나가 추가되서 float4형식이 된다.
		//	{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//};
		D3D11_INPUT_ELEMENT_DESC layout_desc[]	
		{ 
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},  
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},  // Vector2 형식이어서 RG만 값을 준다.
		};

		auto hr = graphics->GetDevice()->CreateInputLayout(layout_desc, 2, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout);
		assert(SUCCEEDED(hr));
	}

	// Pixel Shader
	{
		auto hr = D3DX11CompileFromFileA
		(
			//"Color.hlsl",
			"Texture.hlsl",
			nullptr,
			nullptr,
			"PS",
			"ps_5_0",
			0,
			0,
			nullptr,
			&ps_blob,
			nullptr,
			nullptr
		);
		assert(SUCCEEDED(hr));

		// bytecode 나오면 blob을 써주기
		hr = graphics->GetDevice()->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader);
		assert(SUCCEEDED(hr));
	}

	// Create World View Projection
	{
		// 함수를 활용해서 모든 행렬을 단위 행렬로 만들어 줍시다.
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);

		D3DXVECTOR3 position(0, 0, 0);
		D3DXVECTOR3 target(0, 0, 1);
		D3DXVECTOR3 up(0, 1, 0);

		D3DXMatrixLookAtLH(&view, &position, &target, &up);
		D3DXMatrixOrthoLH(&projection, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 1);   // 가운데 점을 중심으로
		//D3DXMatrixOrthoOffCenterLH(&view, 0, Settings::Get().GetWidth(), 0, Settings::Get().GetHeight(), 0, 1);  // 중점을 좌측 상단으로 지정했다 뒷면(Window 좌표계는 좌측 상단이 0,0이고 y축이 뒤집어져 있다.
		//D3DXMatrixOrthoOffCenterLH(&view, 0, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 0, 1);   // 좌측 하단 앞면 
																									             // 이렇게 어떤 좌표계를 사용할지는 개발자의 선택이다.

		// Perspective : 사람이 보는 시선, 절두체, 원근법 적용
		// Orthographic : 직육면체로 표현, 원근법 없는 것
		// 투영 방법에는 원근 투영과 직교 투영 두 가지가 있다.


		std::cout << "View Matrix" << std::endl;
		std::cout << view._11 << " " << view._12 << " " << view._13 << " " << view._14 << std::endl;
		std::cout << view._21 << " " << view._22 << " " << view._23 << " " << view._24 << std::endl;
		std::cout << view._31 << " " << view._32 << " " << view._33 << " " << view._34 << std::endl;
		std::cout << view._41 << " " << view._42 << " " << view._43 << " " << view._44 << std::endl;

		std::cout << std::endl;

		std::cout << "Projection Matrix" << std::endl;
		std::cout << projection._11 << " " << projection._12 << " " << projection._13 << " " << projection._14 << std::endl;
		std::cout << projection._21 << " " << projection._22 << " " << projection._23 << " " << projection._24 << std::endl;
		std::cout << projection._31 << " " << projection._32 << " " << projection._33 << " " << projection._34 << std::endl;
		std::cout << projection._41 << " " << projection._42 << " " << projection._43 << " " << projection._44 << std::endl;
	
		std::cout << std::endl;

		// World
		D3DXMATRIX S;
		D3DXMatrixScaling(&S, 500, 500, 1);   // 스케일 세팅시 1이상의 값을 넣어줘야 된다. 0을 넣으면 안됨
												  // 이렇게 간단하게 공간의 길이를 바꿀 수 있다.

		std::cout << "Scale Matrix" << std::endl;
		std::cout << S._11 << " " << S._12 << " " << S._13 << " " << S._14 << std::endl;
		std::cout << S._21 << " " << S._22 << " " << S._23 << " " << S._24 << std::endl;
		std::cout << S._31 << " " << S._32 << " " << S._33 << " " << S._34 << std::endl;
		std::cout << S._41 << " " << S._42 << " " << S._43 << " " << S._44 << std::endl;

		std::cout << std::endl;

		D3DXMATRIX T;
		D3DXMatrixTranslation(&T, 0, 0, 0);   // 공간을 이동시켜주는 함수 근데 위에서 지정한 크기가 아니라 1로 세팅되어 있다. 
													  // 즉 공간 스케일과 위치를 조정하는 함수는 이전 데이터를 유지하지 않는다.

		std::cout << "Translation Matrix" << std::endl;
		std::cout << T._11 << " " << T._12 << " " << T._13 << " " << T._14 << std::endl;
		std::cout << T._21 << " " << T._22 << " " << T._23 << " " << T._24 << std::endl;
		std::cout << T._31 << " " << T._32 << " " << T._33 << " " << T._34 << std::endl;
		std::cout << T._41 << " " << T._42 << " " << T._43 << " " << T._44 << std::endl;

		std::cout << std::endl;

		D3DXMATRIX R;
		D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(45)));   // 회전의 중심이 되는 축은 변형되지 않는다.  
													   // 실수형 데이터 뒤에 f가 붙어있으면 float 
													   // 아니라면 기본 double 형이다.
		std::cout << "Rotation Matrix" << std::endl;
		std::cout << R._11 << " " << R._12 << " " << R._13 << " " << R._14 << std::endl;
		std::cout << R._21 << " " << R._22 << " " << R._23 << " " << R._24 << std::endl;
		std::cout << R._31 << " " << R._32 << " " << R._33 << " " << R._34 << std::endl;
		std::cout << R._41 << " " << R._42 << " " << R._43 << " " << R._44 << std::endl;


		// 스 * 자(자전) * 이 * 공(공전) * 부(부모)
		// S * R * T * R * P
		// 순서를 꼭 지킬 것
		world = S * T;  // S와 T를 만들고 서로 world에 결합하자! 
						// 주의할점 행렬은 교환법칙이 성립하지 않는다. T * S != S * T 라는 뜻이다.
	}

	// Create Constant Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;      // CPU - Write , GPU - Read   // CPU가 접근 가능하게
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(TRANSFORM_DATA);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // Usage가 Dynamic이기 때문에 CPU가 접근 가능하게 설정
		
		auto hr = graphics->GetDevice()->CreateBuffer(&desc, nullptr, &gpu_buffer);
		assert(SUCCEEDED(hr));
	}

	// Create Rasterizer state
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;   // cullmode : 여떤 면을 보이지 않게 할 것인지에 대한 정보
		desc.FrontCounterClockwise = false;    // FrontCounterClockWise : 정점이 그려지는 순서대로 앞면과 뒷면을 정해주는 값 
											  // true 면 뒷면으로 바꿔준다. 즉 반시계가 앞면이 된다.

		// Clipping : 그린 후에 데이터를 잘라내는 것  비용이 더 큼


		// Culling : 그리기 전에 데이터를 잘라내는 것

		auto hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rasterizer_state);
		assert(SUCCEEDED(hr));

	}

	// Create Shader Resource View
	{
		//	auto hr = D3DX11CreateShaderResourceViewFromFileA  // D3DX11이 접두사로 쓰이는 함수들은 더 이상 지원되지 않는다. (구버전 기준)
		//	(
		//		graphics->GetDevice(),
		//		"image.png",
		//		nullptr,
		//		nullptr,
		//		&shader_resource[0],
		//		nullptr
		//	);
		//	assert(SUCCEEDED(hr));

		//	hr = D3DX11CreateShaderResourceViewFromFileA  // D3DX11이 접두사로 쓰이는 함수들은 더 이상 지원되지 않는다. (구버전 기준)
		//	(
		//		graphics->GetDevice(),
		//		"image2.jpg",
		//		nullptr,
		//		nullptr,
		//		&shader_resource[1],
		//		nullptr
		//	);
		//	assert(SUCCEEDED(hr));
		auto hr = D3DX11CreateShaderResourceViewFromFileA  // D3DX11이 접두사로 쓰이는 함수들은 더 이상 지원되지 않는다. (구버전 기준)
		(
			graphics->GetDevice(),
			"image.png",
			nullptr,
			nullptr,
			&shader_resource,
			nullptr
		);
		assert(SUCCEEDED(hr));
	}

	// Create Sampler State
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;   // CLAMP : 맨 마지막 픽셀로 나머지 부분을 채운다.
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;   // WRAP : 원본 픽셀로 나머지 부분을 채운다.
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;   // MIRROR : 인접한 픽셀을 반전시켜 나머지 부분을 채운다. 
													  // BORDER : 나머지 부분을 내가 지정한 색으로 채움
		desc.BorderColor[0] = 1;  // 외곽선 색 지정 R
		desc.BorderColor[1] = 0;  // G
		desc.BorderColor[2] = 0;  // B
		desc.BorderColor[3] = 1;  // A
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ComparisonFunc : 이전과 현재 데이터를 비교하는 방법을 정하는 플래그
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // MIN : 축소 , MAG : 확대 , MIP : 밉맵  
														 // LINEAR : 좀더 퀄리티가 좋아짐 
														 // POINT : 겹쳐지는 픽셀을 그냥 지워버림
		desc.MaxAnisotropy = 16;

		// max(), min() 함수를 쓰기 위해서는 stdafx.h 에 NOMINMAX를 include 해야된다.
		desc.MaxLOD = std::numeric_limits<float>::max();  // std::numeric_limits : 수치 타입의 최대, 최소 오차범위 등의 정보
		desc.MinLOD = std::numeric_limits<float>::min();  // LOD : level of detail
		desc.MipLODBias = 0.0f;

		auto hr = graphics->GetDevice()->CreateSamplerState(&desc, &sampler_state);
		assert(SUCCEEDED(hr));
	}

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;   // 잔디, 철조망 같이 jaggy(톱니바퀴 모양처럼 이미지가 지글거리는 현상)를 해결할 수 있다.
		desc.IndependentBlendEnable = false;   // 각각의 blend를 개별적으로 만들지
		desc.RenderTarget[0].BlendEnable = false;   // OM Stage에 RenderTarget은 총 8장까지 세팅 가능하다.
		

		// 6줄 alpha 값을 적절히 섞어서 외곽선과 배경을 자연스럽게 할 것이다.
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;   // Mask : 특정 비트값을 가지는 이진수의 나열  즉 특정 색깔만 가져올 수도 있다.

		auto hr = graphics->GetDevice()->CreateBlendState(&desc, &blend_state);
		assert(SUCCEEDED(hr));
	}
}

// 항상 메모리를 할당 했으면 잊지 맣고 지우자
Execute::~Execute()
{
	SAFE_RELEASE(blend_state);
	SAFE_RELEASE(sampler_state);

	//SAFE_RELEASE(shader_resource[1]);
	//SAFE_RELEASE(shader_resource[0]);

	SAFE_RELEASE(shader_resource);

	SAFE_RELEASE(rasterizer_state);

	SAFE_RELEASE(gpu_buffer);

	SAFE_RELEASE(pixel_shader);
	SAFE_RELEASE(ps_blob);

	SAFE_RELEASE(input_layout);

	SAFE_RELEASE(vertex_shader);
	SAFE_RELEASE(vs_blob);

	SAFE_RELEASE(index_buffer);
	SAFE_DELETE(indices);

	SAFE_RELEASE(vertex_buffer);   // COM Interface를 해제 할때는 Release()를 사용
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	//world._11 = 50;
	//world._22 = 50;

	//world._41 = 100;
	//world._42 = 100;

	// D3DXMATRIX            - 행 우선 행렬
	// GPU - shader - matrix - 열 우선 행렬

	// 1, 0, 0, 0
	// 0
	// 0
	// 0

	//static float radian = 0.0f;
	//radian += 0.01f;
	//D3DXMATRIX P;   // 매 프레임마다 회전만 하는 부모 행렬을 만들어 봅시다.
	//D3DXMatrixRotationZ(&P, radian);

	//world = world * P;   // 부모 행렬을 곱해주면 부모 행렬에 종속된다.



	// 전치 행렬 : 행과 열을 바꾼 행렬 Transpose  즉 열 우선으로 만듦
	D3DXMatrixTranspose(&cpu_buffer.world, &world);
	D3DXMatrixTranspose(&cpu_buffer.view, &view);
	D3DXMatrixTranspose(&cpu_buffer.projection, &projection);

	//cpu_buffer.world = world;
	//cpu_buffer.view = view;
	//cpu_buffer.projection = projection;

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;  // D3D11_MAPPED_SUBRESOURCE : 상수 버퍼 자원을 갱신시킬 수 있습니다.
	graphics->GetDeviceContext()->Map      // 파이프라인 데이터에 접근할 때는 DeviceContext를 사용합니다.
	(
		gpu_buffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapped_subresource
	);

	memcpy(mapped_subresource.pData, &cpu_buffer, sizeof(TRANSFORM_DATA));

	graphics->GetDeviceContext()->Unmap(gpu_buffer, 0);
}

void Execute::Render()
{
	//uint stride = sizeof(VertexColor);   // Stride : 정점 하나의 크기에 대한 정보
	uint stride = sizeof(VertexTexture);

	uint offset = 0;                     // 정점 정보 중에 어떤 정점부터 사용할 것인지에 대한 정보
	graphics->Begin();
	{

		// IA
		//이제 원하는 것을 그리는 부분
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
		//TODO: input layout                 // -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 이렇게 컴퓨터는 어디서 부터 어디까지 포지션이고 색깔인 모르기 때문에 알려줘야 된다. 
												// 즉 Rayout을 사용해서 어디서 부터 어디까지가 포지션이고 색깔인지 알려준다.
		graphics->GetDeviceContext()->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);		 // index buffer는 IA Stage에 들어갑니다.
		graphics->GetDeviceContext()->IASetInputLayout(input_layout);
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   // 정점을 어떻게 연결할 것인가에 대한 연결성 정보
																									   // 즉 점들을 그리라는 건지 선을 그으라는 건지 안을 채우라는 건지 알려줘야 된다.

		// VS
		// VS는 위에서 정한 정점의 개수만큼 호출된다.
		// GPU는 병렬화 되어있기 때문에 데이터가 많아도 속도에 별 문제 없이 이용 가능
		graphics->GetDeviceContext()->VSSetShader(vertex_shader, nullptr, 0);
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, &gpu_buffer);   // startslot이란 HLSL 코드에서 buffer를 등록할때 입력했던 버퍼 번호를 나타낸다.

		//RS
		graphics->GetDeviceContext()->RSSetState(rasterizer_state);

		//PS
		graphics->GetDeviceContext()->PSSetShader(pixel_shader, nullptr, 0);
		//graphics->GetDeviceContext()->PSSetShaderResources(0, 2, shader_resource);
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, &shader_resource);
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, &sampler_state);

		// OM
		graphics->GetDeviceContext()->OMSetBlendState(blend_state, nullptr, 0xffffffff);

		// 이제 Rendering Pipeline에 세팅이 끝났으니 Draw함수로 그려야 된다.
		//graphics->GetDeviceContext()->Draw(6, 0);   // index buffer 가 세팅되어 있으면 draw 형식이 바뀌게 된다.
		graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
	}
	graphics->End();

}

// 한번 파이프라인에 세팅할 경우 다른 세팅이 없다면 값은 변하지 않는다.