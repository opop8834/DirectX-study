#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;  // ���� �������� ���� �⺻���� ������ ��ġ��.
	D3DXCOLOR color;
};

struct VertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;    // Texture Coordinate : �ؽ��� ��ǥ��  (x,y �� ��ü�ϰ� u,v�� ���� ����)
};

struct TRANSFORM_DATA
{
	D3DXMATRIX world; //4x4
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

//class Graphics;   // ���� ����
class Execute final
{
public:
	Execute();
	~Execute();

	void Update();
	void Render();

private:
	class Graphics* graphics = nullptr;   // �ڷ��� �տ� class Ű����� class ����� ���� ����� �����ϴ�.
	
	//VertexColor* vertices = nullptr;  // CPU �ڿ��� GPU �� �Է��ϱ� ���� ��ȯ�ؾ� �ȴ�.
	VertexTexture* vertices = nullptr;   // texture image�� ���� ��

	ID3D11Buffer* vertex_buffer = nullptr;

	uint* indices = nullptr;
	ID3D11Buffer* index_buffer = nullptr;

	ID3D11InputLayout* input_layout = nullptr;

	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* vs_blob = nullptr;         // Blob : Binary Large Object

	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* ps_blob = nullptr;

	// D3DXMATRIX �ڷ������� ����� ���� �� �ִ�.
	// ������ Ŀ���� ���� �����͵鵵 ���� Ŀ���� �ȴ�.
	D3DXMATRIX world; //4x4
	D3DXMATRIX view;
	D3DXMATRIX projection;
	/*
		 1 0 0 0  // x���� 1ũ�� 
		 0 1 0 0  // y���� 1ũ��
		 0 0 1 0  // z���� 1ũ��
		 0 0 0 1  // 0,0,0�� ��ġ

	    y    z
		^   /
		|  /
		| /
		|-------> x
		(0, 0, 0)

	   // ����� �׻� ������ķ� �ʱ�ȭ �ؾ� �Ѵ�.
	   // �̷��� �밢���� 1�ΰ��� �׵� ��� �̶�� �Ѵ�. ���� 1

	   OpenGL - RH         // ȭ�� ������ ������ ��ǥ
	   Directx - LH           // directx�� �޼� ��ǥ�踦 ���,  ȭ������ ���� ��ǥ
	*/

	TRANSFORM_DATA cpu_buffer;
	ID3D11Buffer* gpu_buffer = nullptr;  // Constant Buffer : ��� ����

	ID3D11RasterizerState* rasterizer_state = nullptr;

	// Resource View : Texture �ڿ��� ��� �뵵�� ��Ȯ�� ���ִ� ����
	//ID3D11ShaderResourceView* shader_resource[2];
	ID3D11ShaderResourceView* shader_resource;

	// Texture
	// ID3D11Texture2D
	// RTV
	// DSV
	// SRV : �ؽ��� �����͸� �б⸸ �� �� �ִ� �ڿ� ��
	// UAV

	ID3D11SamplerState* sampler_state = nullptr;
	ID3D11BlendState* blend_state = nullptr;

};

// Rendering Pipeline  �������� �ϱ� ���� ����Ǿ�� �ϴ� �ܰ�� 
// �ܰ�� IA - VS - RS - PS - OM 
// VS, PS �ܰ�� �ڵ��� �����ϴ�.
//  IA, RS, OM �ܰ�� �ڵ��� �Ұ��� �ϴ�.

// IA (Input Assembler Stage) : �⺻ �����Ͱ� ���� �Է� ������ �ܰ� - Vertex(DirectX�� �⺻ �����ʹ� �����̴�.)
// ������ ����ڸ��� ������ �پ��ϱ⿡ DirectX������ �ڷ����� �������� �ʴ´�. �� customize�� �ʿ�

// VS : Vertex shader
// Shader : GPU�� ���۽�Ű�� �Լ����� ���� , ������ �������� �̷�� ���µ� ���꼺�� ������ HLSL (Hight Level Shader Language)�� ������ ��, GLSL
// VS �ܰ迡���� �������� ���� ��ȯ�� ���ش�.  �׸��� ����ȭ��Ű�� ����ȭ�� ������ RS���� �ٽ� �÷��ش�.
// - Transform
//Local : ������ǥ��
//World : �������
//View : �������� ���� (�þ�)
//Projection : ���� ���� (������ �� ���� ���ߴ� ��)   // 3d �����͸� 2d �����ͷ� ��ȯ��Ű�� ��
//Clip : -1 ~ 1 ����ȭ homogeneous space : �������� (3���� ������ �Ѵܰ� Ȯ����� �������� ��ġ���� �ľ�)
	  //�ڿ� �߰��Ǵ� �� w�� 0�̸� ������ ǥ���ϴ� �����̰�, 1�̸� ��ġ�� ��Ÿ����
    // DirectX���� Projection�� Clip space�� �����Ͽ� �ִ�.

// RS : Rasterizer : 3D �����͸� 2D �����ͷ� �ٲ��ִ� ��
// Clipping
// NDC :  -1 ~ 1 ����ȭ Euclidean space : ǥ������ ����,  NDC : Normalized Device Coordinate ���� ��ȯ�� ������ �׳� ȭ�鿡 �°� ��µǴ� ����
// Back face culling : ������ �ʴ� ���� �����ϴ� ��  (�޸�, �ո� �����ؼ� �ڸ� �� ����)
// Viewport transform : ����ȭ �Ȱ��� ������ �÷��ش�.
// scan transform



/*
	DirectX -> API -> GPU

	Rendering Pipeline
	IA -> VS -> RS -> PS -> OM

	DirectX 11 ���α׷��� : ������ ���������� �ܰ迡 �ڿ��� �����Ͽ� ����

	 IA - Input Assembler
	vertex - ����

	ID3D11Buffer -> vertex buffer
	ID3D11Buffer -> index buffer : �ߺ� ������ ���̱� ���� ��ȣ�� �ٿ���
	ID3D11InputLayout(VS�� ������ ����)
	D3D11_PRIMITIVE_TOPOLOGY : ���Ἲ ����

	:  Constant Buffer�� Shader Resource View�� ��� Shader�� ���õ� �� �ִ�.

	 VS - Vertex Shader
	- Constant Buffer

	������ȯ
	- Local - World - View - Projection - Clip  -|->  - NDC - Viewport
	Vertex Shader������ �� ������ ���� World, View, Projection�� ���� ��ȯ�� �����Ѵ�.


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


