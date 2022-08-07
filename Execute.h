#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;  // 정점 데이터의 가장 기본적인 정보는 위치다.
	D3DXCOLOR color;
};

struct VertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;    // Texture Coordinate : 텍스쳐 좌표계  (x,y 를 대체하고 u,v로 따로 지정)
};

struct TRANSFORM_DATA
{
	D3DXMATRIX world; //4x4
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

//class Graphics;   // 전방 선언
class Execute final
{
public:
	Execute();
	~Execute();

	void Update();
	void Render();

private:
	class Graphics* graphics = nullptr;   // 자료형 앞에 class 키위드는 class 상단의 전방 선언과 동일하다.
	
	//VertexColor* vertices = nullptr;  // CPU 자원을 GPU 에 입력하기 위해 변환해야 된다.
	VertexTexture* vertices = nullptr;   // texture image를 위한 것

	ID3D11Buffer* vertex_buffer = nullptr;

	uint* indices = nullptr;
	ID3D11Buffer* index_buffer = nullptr;

	ID3D11InputLayout* input_layout = nullptr;

	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* vs_blob = nullptr;         // Blob : Binary Large Object

	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* ps_blob = nullptr;

	// D3DXMATRIX 자료형으로 행렬을 만들 수 있다.
	// 공간이 커지면 안의 데이터들도 같이 커지게 된다.
	D3DXMATRIX world; //4x4
	D3DXMATRIX view;
	D3DXMATRIX projection;
	/*
		 1 0 0 0  // x방향 1크기 
		 0 1 0 0  // y방향 1크기
		 0 0 1 0  // z방향 1크기
		 0 0 0 1  // 0,0,0인 위치

	    y    z
		^   /
		|  /
		| /
		|-------> x
		(0, 0, 0)

	   // 행렬은 항상 단위행렬로 초기화 해야 한다.
	   // 이렇게 대각선이 1인것을 항등 행렬 이라고 한다. 값이 1

	   OpenGL - RH         // 화면 밖으로 나오는 좌표
	   Directx - LH           // directx는 왼손 좌표계를 사용,  화면으로 들어가는 좌표
	*/

	TRANSFORM_DATA cpu_buffer;
	ID3D11Buffer* gpu_buffer = nullptr;  // Constant Buffer : 상수 버퍼

	ID3D11RasterizerState* rasterizer_state = nullptr;

	// Resource View : Texture 자원의 사용 용도를 명확히 해주는 개념
	//ID3D11ShaderResourceView* shader_resource[2];
	ID3D11ShaderResourceView* shader_resource;

	// Texture
	// ID3D11Texture2D
	// RTV
	// DSV
	// SRV : 텍스쳐 데이터를 읽기만 할 수 있는 자원 뷰
	// UAV

	ID3D11SamplerState* sampler_state = nullptr;
	ID3D11BlendState* blend_state = nullptr;

};

// Rendering Pipeline  랜더링을 하기 위해 수행되어야 하는 단계들 
// 단계는 IA - VS - RS - PS - OM 
// VS, PS 단계는 코딩이 가능하다.
//  IA, RS, OM 단계는 코딩이 불가능 하다.

// IA (Input Assembler Stage) : 기본 데이터가 들어가는 입력 조립기 단계 - Vertex(DirectX의 기본 데이터는 정점이다.)
// 정점은 사용자마다 정보가 다양하기에 DirectX에서는 자료형을 제공하지 않는다. 즉 customize가 필요

// VS : Vertex shader
// Shader : GPU를 동작시키는 함수들의 집합 , 원래는 어셈블리어로 이루어 졌는데 생산성의 문제로 HLSL (Hight Level Shader Language)이 나오게 됨, GLSL
// VS 단계에서는 정점들의 공간 변환을 해준다.  그리고 정규화시키고 정규화한 공간을 RS에서 다시 늘려준다.
// - Transform
//Local : 지역좌표계
//World : 월드공간
//View : 보여지는 영역 (시야)
//Projection : 투영 공간 (차원을 한 차원 낮추는 것)   // 3d 데이터를 2d 데이터로 변환시키는 것
//Clip : -1 ~ 1 정규화 homogeneous space : 동차공간 (3차원 공간을 한단계 확장시켜 방향인지 위치인지 파악)
	  //뒤에 추가되는 값 w가 0이면 방향을 표현하는 벡터이고, 1이면 위치를 나타낸다
    // DirectX에서 Projection과 Clip space는 결합하여 있다.

// RS : Rasterizer : 3D 데이터를 2D 데이터로 바꿔주는 것
// Clipping
// NDC :  -1 ~ 1 정규화 Euclidean space : 표준적인 공간,  NDC : Normalized Device Coordinate 공간 변환이 없을때 그냥 화면에 맞게 출력되는 현상
// Back face culling : 보이지 않는 면을 제거하는 것  (뒷면, 앞면 지정해서 자를 수 있음)
// Viewport transform : 정규화 된값을 실제로 늘려준다.
// scan transform



/*
	DirectX -> API -> GPU

	Rendering Pipeline
	IA -> VS -> RS -> PS -> OM

	DirectX 11 프로그래밍 : 각각의 파이프라인 단계에 자원을 생성하여 세팅

	 IA - Input Assembler
	vertex - 정점

	ID3D11Buffer -> vertex buffer
	ID3D11Buffer -> index buffer : 중복 정점을 줄이기 위해 번호를 붙여줌
	ID3D11InputLayout(VS와 밀접한 관계)
	D3D11_PRIMITIVE_TOPOLOGY : 연결성 정보

	:  Constant Buffer와 Shader Resource View는 모든 Shader에 세팅될 수 있다.

	 VS - Vertex Shader
	- Constant Buffer

	공간변환
	- Local - World - View - Projection - Clip  -|->  - NDC - Viewport
	Vertex Shader에서는 각 정점에 대한 World, View, Projection의 공간 변환을 수행한다.


	 RS - Rasterizer

	- NDC - Viewport
	- rasterizer state


	 PS - Pixel Shader
	Shader Resource


	 OM - Output Merger
	- render target
	- blend state



	Resource View
	Buffer          Texture
*/


