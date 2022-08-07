struct VertexInput
{
	float4 position : POSITION0;   // float4�� float�� �����Ͱ� 4�� ��� �ִ� HLSL �ڷ����̴�. 
								   // POSITION0���� ���� Semantic (�������� ��ó�� ���ҿ� ���� �и��� �ǹ̸� �ο��ϱ� ���� Ű����) �̴�. 
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;  // SV : System Value ��� ������ �߿��� ���̶�� ��
    float2 uv : TEXCOORD0;
};

// Shader������ cbuffer��� Ÿ������ ����� �ݴϴ�.
cbuffer TransformBuffer : register(b0)   // -> 0 ~ 13   // ��� ���۴� �ݵ�� 16byte ������ �����ؾ� �ȴ�.  �ִ� 4096 * 16byte
{
    //row_major matrix world;   // row_major�� �� �켱 ��ķ� �ٲ� �� �ִ�.
    //row_major matrix view;   // �ٵ� shader�� �� �켱 ��ķ� ����°� �� ����
    //row_major matrix proj;
    matrix world;
    matrix view;
    matrix proj;
};


PixelInput VS(VertexInput input)
{
	// ��ġ ���͸� �ٸ� �������� �̵���Ű�� ���ؼ��� ���Ϳ� ��ġ�� �����ָ� �ȴ�.
	// 1 x 4		 4 x 4   �� �� ->  1 x 4�� ����
	// 1, 2, 3, 1  * 0000
	//				 0000
	//				 0000
	//				 0000
	PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
	output.uv = input.uv;

	return output;
}
// shader�� �������� �������� ������ �ϱ⿡ �ϳ��� ���Ͽ� ��� �ۼ��ص� ������.

//Texture2D �ڷ������� �ؽ��� �ڿ��� �޾ƾ� �մϴ�.
Texture2D source_texture1 : register(t0);  // �ؽ��� �ڿ��̶�� �ǹ��� 't'�� ����� �Ѵ�.
Texture2D source_texture2 : register(t1);   // �ؽ��� �ڿ��� 128���� ������ ������ 0������ 127������ ��� �����մϴ�.

SamplerState samp : register(s0);  // ���ø��� �����̸�, s0�� ���÷� �ڿ��̶�� �ǹ��� 's'�� ���ش�.  
								   // ���÷� �ڿ��� 16���� ������ ������ 0������ 15������ ��� ����
								   // SamplerState�� �������� �ʾƵ� �⺻���� ���� �Ǿ��ִ�.
// Pixel Shader �� Rasterizer�� ������ �ȼ� ������ŭ ȣ��
// SV_Target �ø�ƽ���� ���� ���õ� ���� ���� Ÿ���� ����϶�� �˷��ش�. �� ������ ��ȭ���� �׸��� ����.
float4 PS(PixelInput input) : SV_Target
{
    //float4 color = source_texture.Sample(samp, input.uv);
    //float4 color = 0.0f;   // x,y,z�� 0���� �ʱ�ȭ
	
	// uv ��ǥ�� ���� �ٸ� �̹����� �ֱ�
	//if(input.uv.x < 1.0f)
 //   {
 //       color = source_texture1.Sample(samp, input.uv);
 //   }
 //   else
 //   {
 //       color = source_texture2.Sample(samp, float2(input.uv.x - 1.0f, input.uv.y)); // uv��ǥ�� ���Ƿ� �ø� �� ������ ���̴����� ���� �� 0~1 ������ ����� �Ѵ�.
 //   }
	
    //clip(color.a - 0.9f);   // HLSL�� clip() �Լ��� ���ڷ� �޴� �����Ͱ� 0���� ������ �ش� �����͸� ����Ѵ�.
							// �� alpha���� �����Ͽ� ����� �����ϰ� ����� �� ���̴�.
	// ���� clip ���� �ٸ� ���
	//if(color.a < 0.1f)
 //   {
 //       discard;
 //   }
	
    float4 color = source_texture1.Sample(samp, input.uv);
	
    return color; // Sampling : ������ �����͸� �����ϴ� ��
}