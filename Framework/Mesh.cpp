#include "Mesh.h"

namespace Framework
{
    Mesh::Mesh(ID3D11Device * device,
        ID3D11DeviceContext * deviceContext,
        std::vector<Vertex>& vertices,
        std::vector<DWORD>& indices,
        std::vector<Texture> & textures,
        DirectX::SimpleMath::Matrix transformMatrix)
    {
        this->deviceContext = deviceContext;
        this->textures = textures;
        this->transformMatrix = transformMatrix;

        HRESULT hr = this->vertexbuffer.Initialize(device, vertices.data(), vertices.size());

        hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
    }

    Mesh::Mesh(const Mesh & mesh)
    {
        this->deviceContext = mesh.deviceContext;
        this->indexbuffer = mesh.indexbuffer;
        this->vertexbuffer = mesh.vertexbuffer;
        this->textures = mesh.textures;
        this->transformMatrix = mesh.transformMatrix;
    }

    void Mesh::Draw()
    {
        UINT offset = 0;

        for (int i = 0; i < textures.size(); i++)
        {
            if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
            {
                this->deviceContext->PSSetShaderResources(0, 1, textures[i].GetTextureResourceViewAddress());
                break;
            }
        }

        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
        this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
    }

    const DirectX::SimpleMath::Matrix& Mesh::GetTransform()
    {
        return this->transformMatrix;
    }
}
