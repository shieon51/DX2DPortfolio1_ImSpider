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
	float2 curFrame;
	float2 maxFrame;
}

cbuffer ValueBuffer : register(b2)
{
	int select;
	int scale;
	
	int2 imageSize;
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

Texture2D map : register(t0);
Texture2D secondMap : register(t1);

SamplerState samp : register(s0);

float4 Modulate(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);		
	float4 tex2 = secondMap.Sample(samp, uv);

	return tex1 * tex2;
}

float4 Modulate2X(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return tex1 * tex2 * 2.0f;
}

float4 Add(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return tex1 + tex2;
}

float4 AddSmooth(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return tex1 + tex2 - tex1 * tex2;
}

float4 AddSigned(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return tex1 + tex2 - 0.5f;
}

float4 AddSigned2X(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return (tex1 + tex2 - 0.5f) * 2.0f;
}

float4 Subtract(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return float4((tex1 - tex2).rgb, 1.0f);
}

float4 RevSubtract(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);
	float4 tex2 = secondMap.Sample(samp, uv);

	return float4((tex2 - tex1).rgb, 1.0f);
}

float4 Negative(float2 uv)
{
	float4 tex1 = map.Sample(samp, uv);

	return float4((1.0f - tex1).rgb, 1.0f);
}

float4 PS(PixelInput input) : SV_TARGET
{
	float2 uv = input.uv + curFrame / maxFrame;
	
	float4 baseColor = map.Sample(samp, uv);
	
	if (baseColor.r == removeColor.r &&
		baseColor.g == removeColor.g &&
		baseColor.b == removeColor.b)
		baseColor.a = 0.0f;
	
	if (select == 1)
		return Modulate(input.uv);
	else if (select == 2)
		return Modulate2X(input.uv);
	else if (select == 3)
		return Add(input.uv);
	else if (select == 4)
		return AddSmooth(input.uv);
	else if (select == 5)
		return AddSigned(input.uv);
	else if (select == 6)
		return AddSigned2X(input.uv);
	else if (select == 7)
		return Subtract(input.uv);
	else if (select == 8)
		return RevSubtract(input.uv);
	else if (select == 9)
		return Negative(input.uv);
	
	return baseColor * tintColor;
}