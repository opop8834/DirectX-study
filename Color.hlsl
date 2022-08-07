struct VertexInput
{
	float4 position : POSITION0;   // float4는 float형 데이터가 4개 들어 있는 HLSL 자료형이다. 
								   // POSITION0같은 값은 Semantic (데이터의 출처와 역할에 대한 분명한 의미를 부여하기 위한 키워드) 이다. 
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;  // SV : System Value 라는 뜻으로 중요한 값이라는 것
	float4 color : COLOR0;
};

// Shader에서는 cbuffer라는 타입으로 만들어 줍니다.
cbuffer TransformBuffer : register(b0)   // -> 0 ~ 13   // 상수 버퍼는 반드시 16byte 단위로 정렬해야 된다.  최대 4096 * 16byte
{
    //row_major matrix world;   // row_major로 행 우선 행렬로 바꿀 수 있다.
    //row_major matrix view;   // 근데 shader의 열 우선 행렬로 만드는게 더 빠름
    //row_major matrix proj;
    matrix world;
    matrix view;
    matrix proj;
};


PixelInput VS(VertexInput input)
{
	// 위치 벡터를 다른 공간으로 이동시키기 위해서는 벡터와 위치를 곱해주면 된다.
	// 1 x 4		 4 x 4   의 곱 ->  1 x 4가 나옴
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
// shader는 진입점을 기준으로 컴파일 하기에 하나의 파일에 모두 작성해도 괜찮다.


// Pixel Shader 는 Rasterizer가 지정한 픽셀 개수만큼 호출
// SV_Target 시멘틱으로 현재 세팅된 메인 렌더 타겟을 사용하라고 알려준다. 즉 세팅한 도화지에 그림을 찍어낸다.
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}