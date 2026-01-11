#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
    : stride(stride)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = stride * count;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;

    device->CreateBuffer(&bufferDesc, &initData, &buffer);
}

VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

void VertexBuffer::Set(UINT slot)
{
    deviceContext->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);    
}

void VertexBuffer::Update(void* data, UINT count)
{
    deviceContext->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}
