struct VertexInput
{
	float4 position : POSITION0;   // float4는 float형 데이터가 4개 들어 있는 HLSL 자료형이다. 
								   // POSITION0같은 값은 Semantic (데이터의 출처와 역할에 대한 분명한 의미를 부여하기 위한 키워드) 이다. 
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;  // SV : System Value 라는 뜻으로 중요한 값이라는 것
    float2 uv : TEXCOORD0;
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
	output.uv = input.uv;

	return output;
}
// shader는 진입점을 기준으로 컴파일 하기에 하나의 파일에 모두 작성해도 괜찮다.

//Texture2D 자료형으로 텍스쳐 자원을 받아야 합니다.
Texture2D source_texture1 : register(t0);  // 텍스쳐 자원이라는 의미의 't'를 써줘야 한다.
Texture2D source_texture2 : register(t1);   // 텍스쳐 자원은 128개의 슬롯을 가지며 0번부터 127번까지 사용 가능합니다.

SamplerState samp : register(s0);  // 샘플링한 상태이며, s0는 샘플러 자원이라는 의미의 's'를 써준다.  
								   // 샘플러 자원은 16개의 슬롯을 가지며 0번부터 15번까지 사용 가능
								   // SamplerState는 세팅하지 않아도 기본값이 세팅 되어있다.
// Pixel Shader 는 Rasterizer가 지정한 픽셀 개수만큼 호출
// SV_Target 시멘틱으로 현재 세팅된 메인 렌더 타겟을 사용하라고 알려준다. 즉 세팅한 도화지에 그림을 찍어낸다.
float4 PS(PixelInput input) : SV_Target
{
    //float4 color = source_texture.Sample(samp, input.uv);
    //float4 color = 0.0f;   // x,y,z에 0으로 초기화
	
	// uv 좌표에 따라서 다른 이미지를 넣기
	//if(input.uv.x < 1.0f)
 //   {
 //       color = source_texture1.Sample(samp, input.uv);
 //   }
 //   else
 //   {
 //       color = source_texture2.Sample(samp, float2(input.uv.x - 1.0f, input.uv.y)); // uv좌표를 임의로 늘릴 순 있지만 셰이더에서 추출 시 0~1 범위로 맞춰야 한다.
 //   }
	
    //clip(color.a - 0.9f);   // HLSL에 clip() 함수는 인자로 받는 데이터가 0보다 작으면 해당 데이터를 폐기한다.
							// 즉 alpha값을 참고하여 배경을 투명하게 만들어 줄 것이다.
	// 위에 clip 말고 다른 방법
	//if(color.a < 0.1f)
 //   {
 //       discard;
 //   }
	
    float4 color = source_texture1.Sample(samp, input.uv);
	
    return color; // Sampling : 임의의 데이터를 추출하는 것
}