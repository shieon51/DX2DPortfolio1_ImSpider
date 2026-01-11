#include "Framework.h"

Mesh::~Mesh()
{
    vertexBuffer->Release();
    indexBuffer->Release();
}

void Mesh::Draw(D3D11_PRIMITIVE_TOPOLOGY type)
{
    SetMesh(type);

    deviceContext->DrawIndexed(indices.size(), 0, 0);
}

void Mesh::SetMesh(D3D11_PRIMITIVE_TOPOLOGY type)
{
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(type);
}

void Mesh::CreateMesh()
{
    {
        //VertexBuffer
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = vertices.data();

        device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer);
    }

    {
        //IndexBuffer
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(UINT) * indices.size();
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = indices.data();//첫번째 노드의 주소값

        device->CreateBuffer(&bufferDesc, &subData, &indexBuffer);
    }
}

void Mesh::UpdateVertices()
{
    deviceContext->UpdateSubresource(vertexBuffer, 0, nullptr, vertices.data(), stride, vertices.size());
}
