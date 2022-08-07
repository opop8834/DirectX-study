#include "stdafx.h"
#include "Execute.h"
#include "Graphics.h"  // ��� ���� �߰�

Execute::Execute()
{
	// �ҿ����� �����̶�� ���� �����Ϸ��� �ش� �ڷ����� �𸣱� ������ �ش� �ڷ����� ��� ������ �������ش�.
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));



//			|
//	1		|		3
//			|
//----------|------------   //�ð� �������� ȸ���ϴ� ���� �ﰢ���� ���� ���̴�.
//			|				�ð� �������� ����� �簢��
//	0		|		2       �ð� �����̸� �⺻������ �ո� �̶�� ���� �ǹ��Ѵ�. �� �޸��� �ݽð�
//			|

	// Vertex Data
	//{
	//	vertices = new VertexColor[4];   // �޸𸮸� �Ҵ� ������ ������ ������
	//	vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);  //0
	//	vertices[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//	vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);  //1
	//	vertices[1].color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	//	vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);  //2
	//	vertices[2].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//	vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);  //3
	//	vertices[3].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//	// 6���� �������� �ߺ��Ǵ� �� �� 4���� ������ �̿��غ���
	//}
	{
		vertices = new VertexTexture[4];   // �޸𸮸� �Ҵ� ������ ������ ������
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);  //0
		vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);         // uv��ǥ�� �̹��� ������ ���� �ڷ��̴�. 
														  // �׸��� �ؽ��� ��ǥ��� ���� ����� �������̰� 0~1 ������ ����ȭ�Ǿ� �ִ�.

		vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);  //1
		vertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);   // 1�� ��ġ�� 0,0

		vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);  //2
		vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);

		vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);  //3
		vertices[3].uv = D3DXVECTOR2(1.0f, 0.0f);
	}
	
	// Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;   //  Usage : GPU �� CPU�� ���� �����͸� �аų� �� ������ �����ϴ� �÷��� 
											  //  D3D11_USAGE_DEFAULT : GPU�� �����͸� �а� �� �� �ִ�.  CPU�� X 
											  //  D3D11_USAGE_IMMUTABLE : GPU�� �����͸� ���� �� �ִ�. (����� �Ұ���)
		
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // Buffer�� �����͸� ��� ������������ ����� ���� �������ִ� �÷��״�.
		
	    //desc.ByteWidth = sizeof(VertexColor) * 4;
		desc.ByteWidth = sizeof(VertexTexture) * 4;

		D3D11_SUBRESOURCE_DATA sub_data;   // CPU �����͸� GPU�� ���� ��ų �� �ִ�.
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
		sub_data.pSysMem = vertices;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &sub_data, &vertex_buffer);
		assert(SUCCEEDED(hr));
	}

	//Index Data
	{
		// �߰�ȣ�� ���Ͽ� �ٷ� �Ҵ��� �����ϴ�.
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
		auto hr = D3DX11CompileFromFileA // D3DX11�� �����ϴ� �Լ����� �ֽ� DirectX SDK���� �������� �ʴ´�.
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

		// �� ����ϱ� DirexX�� �ڿ��� ���鶧�� ID3D11Device�� ����Ѵ�.
		hr = graphics->GetDevice()->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader);
		assert(SUCCEEDED(hr));
	}

	// Vertex Buffer�� Vertex Shader�� �Է� �����ʹ� �׻� ��ġ�Ͽ��� �Ѵ�.
	// �� VS�� ������ ����� �Ʒ��� �� ������ inputlayout���� ���Ͽ� ���� �ٸ��� �� ��Ʈ����.

	// Input Layout
	// ������ ������ �ٲ�� Input Layout�� �ٲ��� �Ѵ�.
	{
		//D3D11_INPUT_ELEMENT_DESC layout_desc[]

		//	// Instancing : �ϳ��� ������Ʈ�� ���� ���� ���� �ν��Ͻ��� �� ���� �׸��� ȣ��� ������ �ϴ� ���
		//{                          // 4BYTE�� RGB������ ���� FLOAT
		//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},   // vector3 �������� �Ѱܵ� �ڵ����� �ڿ� �ϳ��� �߰��Ǽ� float4������ �ȴ�.
		//	{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//};
		D3D11_INPUT_ELEMENT_DESC layout_desc[]	
		{ 
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},  
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},  // Vector2 �����̾ RG�� ���� �ش�.
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

		// bytecode ������ blob�� ���ֱ�
		hr = graphics->GetDevice()->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader);
		assert(SUCCEEDED(hr));
	}

	// Create World View Projection
	{
		// �Լ��� Ȱ���ؼ� ��� ����� ���� ��ķ� ����� �ݽô�.
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);

		D3DXVECTOR3 position(0, 0, 0);
		D3DXVECTOR3 target(0, 0, 1);
		D3DXVECTOR3 up(0, 1, 0);

		D3DXMatrixLookAtLH(&view, &position, &target, &up);
		D3DXMatrixOrthoLH(&projection, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 1);   // ��� ���� �߽�����
		//D3DXMatrixOrthoOffCenterLH(&view, 0, Settings::Get().GetWidth(), 0, Settings::Get().GetHeight(), 0, 1);  // ������ ���� ������� �����ߴ� �޸�(Window ��ǥ��� ���� ����� 0,0�̰� y���� �������� �ִ�.
		//D3DXMatrixOrthoOffCenterLH(&view, 0, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 0, 1);   // ���� �ϴ� �ո� 
																									             // �̷��� � ��ǥ�踦 ��������� �������� �����̴�.

		// Perspective : ����� ���� �ü�, ����ü, ���ٹ� ����
		// Orthographic : ������ü�� ǥ��, ���ٹ� ���� ��
		// ���� ������� ���� ������ ���� ���� �� ������ �ִ�.


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
		D3DXMatrixScaling(&S, 500, 500, 1);   // ������ ���ý� 1�̻��� ���� �־���� �ȴ�. 0�� ������ �ȵ�
												  // �̷��� �����ϰ� ������ ���̸� �ٲ� �� �ִ�.

		std::cout << "Scale Matrix" << std::endl;
		std::cout << S._11 << " " << S._12 << " " << S._13 << " " << S._14 << std::endl;
		std::cout << S._21 << " " << S._22 << " " << S._23 << " " << S._24 << std::endl;
		std::cout << S._31 << " " << S._32 << " " << S._33 << " " << S._34 << std::endl;
		std::cout << S._41 << " " << S._42 << " " << S._43 << " " << S._44 << std::endl;

		std::cout << std::endl;

		D3DXMATRIX T;
		D3DXMatrixTranslation(&T, 0, 0, 0);   // ������ �̵������ִ� �Լ� �ٵ� ������ ������ ũ�Ⱑ �ƴ϶� 1�� ���õǾ� �ִ�. 
													  // �� ���� �����ϰ� ��ġ�� �����ϴ� �Լ��� ���� �����͸� �������� �ʴ´�.

		std::cout << "Translation Matrix" << std::endl;
		std::cout << T._11 << " " << T._12 << " " << T._13 << " " << T._14 << std::endl;
		std::cout << T._21 << " " << T._22 << " " << T._23 << " " << T._24 << std::endl;
		std::cout << T._31 << " " << T._32 << " " << T._33 << " " << T._34 << std::endl;
		std::cout << T._41 << " " << T._42 << " " << T._43 << " " << T._44 << std::endl;

		std::cout << std::endl;

		D3DXMATRIX R;
		D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(45)));   // ȸ���� �߽��� �Ǵ� ���� �������� �ʴ´�.  
													   // �Ǽ��� ������ �ڿ� f�� �پ������� float 
													   // �ƴ϶�� �⺻ double ���̴�.
		std::cout << "Rotation Matrix" << std::endl;
		std::cout << R._11 << " " << R._12 << " " << R._13 << " " << R._14 << std::endl;
		std::cout << R._21 << " " << R._22 << " " << R._23 << " " << R._24 << std::endl;
		std::cout << R._31 << " " << R._32 << " " << R._33 << " " << R._34 << std::endl;
		std::cout << R._41 << " " << R._42 << " " << R._43 << " " << R._44 << std::endl;


		// �� * ��(����) * �� * ��(����) * ��(�θ�)
		// S * R * T * R * P
		// ������ �� ��ų ��
		world = S * T;  // S�� T�� ����� ���� world�� ��������! 
						// �������� ����� ��ȯ��Ģ�� �������� �ʴ´�. T * S != S * T ��� ���̴�.
	}

	// Create Constant Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;      // CPU - Write , GPU - Read   // CPU�� ���� �����ϰ�
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(TRANSFORM_DATA);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // Usage�� Dynamic�̱� ������ CPU�� ���� �����ϰ� ����
		
		auto hr = graphics->GetDevice()->CreateBuffer(&desc, nullptr, &gpu_buffer);
		assert(SUCCEEDED(hr));
	}

	// Create Rasterizer state
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;   // cullmode : ���� ���� ������ �ʰ� �� �������� ���� ����
		desc.FrontCounterClockwise = false;    // FrontCounterClockWise : ������ �׷����� ������� �ո�� �޸��� �����ִ� �� 
											  // true �� �޸����� �ٲ��ش�. �� �ݽð谡 �ո��� �ȴ�.

		// Clipping : �׸� �Ŀ� �����͸� �߶󳻴� ��  ����� �� ŭ


		// Culling : �׸��� ���� �����͸� �߶󳻴� ��

		auto hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rasterizer_state);
		assert(SUCCEEDED(hr));

	}

	// Create Shader Resource View
	{
		//	auto hr = D3DX11CreateShaderResourceViewFromFileA  // D3DX11�� ���λ�� ���̴� �Լ����� �� �̻� �������� �ʴ´�. (������ ����)
		//	(
		//		graphics->GetDevice(),
		//		"image.png",
		//		nullptr,
		//		nullptr,
		//		&shader_resource[0],
		//		nullptr
		//	);
		//	assert(SUCCEEDED(hr));

		//	hr = D3DX11CreateShaderResourceViewFromFileA  // D3DX11�� ���λ�� ���̴� �Լ����� �� �̻� �������� �ʴ´�. (������ ����)
		//	(
		//		graphics->GetDevice(),
		//		"image2.jpg",
		//		nullptr,
		//		nullptr,
		//		&shader_resource[1],
		//		nullptr
		//	);
		//	assert(SUCCEEDED(hr));
		auto hr = D3DX11CreateShaderResourceViewFromFileA  // D3DX11�� ���λ�� ���̴� �Լ����� �� �̻� �������� �ʴ´�. (������ ����)
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
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;   // CLAMP : �� ������ �ȼ��� ������ �κ��� ä���.
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;   // WRAP : ���� �ȼ��� ������ �κ��� ä���.
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;   // MIRROR : ������ �ȼ��� �������� ������ �κ��� ä���. 
													  // BORDER : ������ �κ��� ���� ������ ������ ä��
		desc.BorderColor[0] = 1;  // �ܰ��� �� ���� R
		desc.BorderColor[1] = 0;  // G
		desc.BorderColor[2] = 0;  // B
		desc.BorderColor[3] = 1;  // A
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ComparisonFunc : ������ ���� �����͸� ���ϴ� ����� ���ϴ� �÷���
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // MIN : ��� , MAG : Ȯ�� , MIP : �Ӹ�  
														 // LINEAR : ���� ����Ƽ�� ������ 
														 // POINT : �������� �ȼ��� �׳� ��������
		desc.MaxAnisotropy = 16;

		// max(), min() �Լ��� ���� ���ؼ��� stdafx.h �� NOMINMAX�� include �ؾߵȴ�.
		desc.MaxLOD = std::numeric_limits<float>::max();  // std::numeric_limits : ��ġ Ÿ���� �ִ�, �ּ� �������� ���� ����
		desc.MinLOD = std::numeric_limits<float>::min();  // LOD : level of detail
		desc.MipLODBias = 0.0f;

		auto hr = graphics->GetDevice()->CreateSamplerState(&desc, &sampler_state);
		assert(SUCCEEDED(hr));
	}

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;   // �ܵ�, ö���� ���� jaggy(��Ϲ��� ���ó�� �̹����� ���۰Ÿ��� ����)�� �ذ��� �� �ִ�.
		desc.IndependentBlendEnable = false;   // ������ blend�� ���������� ������
		desc.RenderTarget[0].BlendEnable = false;   // OM Stage�� RenderTarget�� �� 8����� ���� �����ϴ�.
		

		// 6�� alpha ���� ������ ��� �ܰ����� ����� �ڿ������� �� ���̴�.
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;   // Mask : Ư�� ��Ʈ���� ������ �������� ����  �� Ư�� ���� ������ ���� �ִ�.

		auto hr = graphics->GetDevice()->CreateBlendState(&desc, &blend_state);
		assert(SUCCEEDED(hr));
	}
}

// �׻� �޸𸮸� �Ҵ� ������ ���� �İ� ������
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

	SAFE_RELEASE(vertex_buffer);   // COM Interface�� ���� �Ҷ��� Release()�� ���
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	//world._11 = 50;
	//world._22 = 50;

	//world._41 = 100;
	//world._42 = 100;

	// D3DXMATRIX            - �� �켱 ���
	// GPU - shader - matrix - �� �켱 ���

	// 1, 0, 0, 0
	// 0
	// 0
	// 0

	//static float radian = 0.0f;
	//radian += 0.01f;
	//D3DXMATRIX P;   // �� �����Ӹ��� ȸ���� �ϴ� �θ� ����� ����� ���ô�.
	//D3DXMatrixRotationZ(&P, radian);

	//world = world * P;   // �θ� ����� �����ָ� �θ� ��Ŀ� ���ӵȴ�.



	// ��ġ ��� : ��� ���� �ٲ� ��� Transpose  �� �� �켱���� ����
	D3DXMatrixTranspose(&cpu_buffer.world, &world);
	D3DXMatrixTranspose(&cpu_buffer.view, &view);
	D3DXMatrixTranspose(&cpu_buffer.projection, &projection);

	//cpu_buffer.world = world;
	//cpu_buffer.view = view;
	//cpu_buffer.projection = projection;

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;  // D3D11_MAPPED_SUBRESOURCE : ��� ���� �ڿ��� ���Ž�ų �� �ֽ��ϴ�.
	graphics->GetDeviceContext()->Map      // ���������� �����Ϳ� ������ ���� DeviceContext�� ����մϴ�.
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
	//uint stride = sizeof(VertexColor);   // Stride : ���� �ϳ��� ũ�⿡ ���� ����
	uint stride = sizeof(VertexTexture);

	uint offset = 0;                     // ���� ���� �߿� � �������� ����� �������� ���� ����
	graphics->Begin();
	{

		// IA
		//���� ���ϴ� ���� �׸��� �κ�
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
		//TODO: input layout                 // -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // �̷��� ��ǻ�ʹ� ��� ���� ������ �������̰� ������ �𸣱� ������ �˷���� �ȴ�. 
												// �� Rayout�� ����ؼ� ��� ���� �������� �������̰� �������� �˷��ش�.
		graphics->GetDeviceContext()->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);		 // index buffer�� IA Stage�� ���ϴ�.
		graphics->GetDeviceContext()->IASetInputLayout(input_layout);
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   // ������ ��� ������ ���ΰ��� ���� ���Ἲ ����
																									   // �� ������ �׸���� ���� ���� ������� ���� ���� ä���� ���� �˷���� �ȴ�.

		// VS
		// VS�� ������ ���� ������ ������ŭ ȣ��ȴ�.
		// GPU�� ����ȭ �Ǿ��ֱ� ������ �����Ͱ� ���Ƶ� �ӵ��� �� ���� ���� �̿� ����
		graphics->GetDeviceContext()->VSSetShader(vertex_shader, nullptr, 0);
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, &gpu_buffer);   // startslot�̶� HLSL �ڵ忡�� buffer�� ����Ҷ� �Է��ߴ� ���� ��ȣ�� ��Ÿ����.

		//RS
		graphics->GetDeviceContext()->RSSetState(rasterizer_state);

		//PS
		graphics->GetDeviceContext()->PSSetShader(pixel_shader, nullptr, 0);
		//graphics->GetDeviceContext()->PSSetShaderResources(0, 2, shader_resource);
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, &shader_resource);
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, &sampler_state);

		// OM
		graphics->GetDeviceContext()->OMSetBlendState(blend_state, nullptr, 0xffffffff);

		// ���� Rendering Pipeline�� ������ �������� Draw�Լ��� �׷��� �ȴ�.
		//graphics->GetDeviceContext()->Draw(6, 0);   // index buffer �� ���õǾ� ������ draw ������ �ٲ�� �ȴ�.
		graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
	}
	graphics->End();

}

// �ѹ� ���������ο� ������ ��� �ٸ� ������ ���ٸ� ���� ������ �ʴ´�.