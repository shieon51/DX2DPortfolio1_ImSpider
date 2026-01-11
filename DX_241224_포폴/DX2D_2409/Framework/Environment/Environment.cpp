#include "Framework.h"

Environment::Environment()
{
    CreateSamaplerState();
	CreateBlendState();
	CreateRasterizerState();

	SetViewport();

	mainCamera = new Camera();
}

Environment::~Environment()
{
	samplerState->Release();
	delete mainCamera;
}

void Environment::Update()
{
	mainCamera->Update();
}

void Environment::Render()
{
	mainCamera->SetViewProjection();
}

void Environment::PostRender()
{
	mainCamera->SetUIView();
}

void Environment::SetAlphaBlend()
{
	float blendFactor[4] = {};
	deviceContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
}

void Environment::SetAdditiveBlend()
{
	float blendFactor[4] = {};
	deviceContext->OMSetBlendState(additiveBlendState, blendFactor, 0xffffffff);
}

void Environment::CreateSamaplerState()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR; //가로만 미러모드로
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	//LOD(Level Of Detail) : 카메라와의 거리에 따라서 퀄리티를 나누는 기술

	device->CreateSamplerState(&desc, &samplerState);

	deviceContext->PSSetSamplers(0, 1, &samplerState);
}

void Environment::CreateBlendState()
{
	D3D11_BLEND_DESC desc = {};
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&desc, &alphaBlendState);	

	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	device->CreateBlendState(&desc, &additiveBlendState);

	float blendFactor[4] = {};
	deviceContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
}

void Environment::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	deviceContext->RSSetState(rasterizerState);
}

void Environment::SetViewport()
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);
}
