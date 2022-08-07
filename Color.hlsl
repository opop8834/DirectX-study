struct VertexInput
{
	float4 position : POSITION0;   // float4�� float�� �����Ͱ� 4�� ��� �ִ� HLSL �ڷ����̴�. 
								   // POSITION0���� ���� Semantic (�������� ��ó�� ���ҿ� ���� �и��� �ǹ̸� �ο��ϱ� ���� Ű����) �̴�. 
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;  // SV : System Value ��� ������ �߿��� ���̶�� ��
	float4 color : COLOR0;
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
	output.color = input.color;

	return output;
}
// shader�� �������� �������� ������ �ϱ⿡ �ϳ��� ���Ͽ� ��� �ۼ��ص� ������.


// Pixel Shader �� Rasterizer�� ������ �ȼ� ������ŭ ȣ��
// SV_Target �ø�ƽ���� ���� ���õ� ���� ���� Ÿ���� ����϶�� �˷��ش�. �� ������ ��ȭ���� �׸��� ����.
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}