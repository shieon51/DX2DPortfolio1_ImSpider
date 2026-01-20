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

//ValueBuffer (점멸 스위치, 점멸 시 색상)
cbuffer ValueBuffer : register(b2)
{
    int isSwitchOn;
}

cbuffer ValueBuffer : register(b3)
{
    float4 overlayColor;
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
SamplerState samp : register(s0);

float4 Grayscale2(float4 baseColor)
{	
    float scale = dot(baseColor.rgb, float3(0.3f, 0.59f, 0.11f));

    return float4(scale.xxx, baseColor.a);
}

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + curFrame / maxFrame;
	
    float4 baseColor = map.Sample(samp, uv);
	
    if (baseColor.r == removeColor.r &&
		baseColor.g == removeColor.g &&
		baseColor.b == removeColor.b)
        baseColor.a = 0.0f;
	
    if (isSwitchOn == 1) // 오버레이 기능 켜지면 (피격 시)
        baseColor = Grayscale2(baseColor) + overlayColor;
	
    return baseColor * tintColor;
}