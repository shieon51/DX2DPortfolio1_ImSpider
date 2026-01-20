//VS ConstantBuffer
cbuffer VPBuffer : register(b0)
{
	float4x4 view;
	matrix projection;
}

cbuffer WorldBuffer : register(b1)
{
	matrix world;
}

//PS ConstantBuffer
cbuffer ColorBuffer : register(b0)
{
	float4 tintColor;
	float4 removeColor;
}

cbuffer FrameBuffer : register(b1)
{
	float fillAmount;
}

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
		
	return output;
}

Texture2D frontMap : register(t0);
Texture2D backMap : register(t1);

SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{	
	float4 frontColor = frontMap.Sample(samp, input.uv); // 앞 이미지
	float4 backColor = backMap.Sample(samp, input.uv); // 뒷 이미지
		
	float4 baseColor = frontColor;
	
	if(input.uv.x > fillAmount)
	{
		baseColor = backColor;
	}
	
	if (baseColor.r == removeColor.r &&
		baseColor.g == removeColor.g &&
		baseColor.b == removeColor.b)
		baseColor.a = 0.0f;
	
	return baseColor * tintColor;
}