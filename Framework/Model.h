#pragma once
#include "ConstBuffers.h"
#include "Mesh.h"
#include "Texture.h"
#include "Vertex.h"
#include "Framework/Forward.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Framework/ConstantBuffer.h"
#include "Framework/Graphics.h"
#include "Framework/IGame.h"
#include "Framework/IndexBuffer.h"
#include "Framework/Shaders.h"
#include "Framework/VertexBuffer.h"
#include "Framework/XYZGameComponent.h"

namespace Framework
{
    class Model : public XYZGameComponent
    {
    public:
        Model(const std::string& modelPath, float size = 1.0f, const std::string& texturePath = "");
        void Initialize(IGame* game) override;
        void Update(Transform transform) override;
        void Draw() override;
        void ClearResources() override;
        void Rotate(RotDirection direction, float angle) override;
        void Translate(float x, float y, float z) override;
        DirectX::SimpleMath::Matrix GetWorld() override { return m_world; }
    private:
        ID3D11Device * device = nullptr;
        ID3D11DeviceContext * context = nullptr;
        std::vector<Mesh> meshes;

        void ProcessNode(aiNode * node, const aiScene * scene, DirectX::SimpleMath::Matrix parentTransformMatrix);
        Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene, DirectX::SimpleMath::Matrix transformMatrix);
        TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
        int GetTextureIndex(aiString* pStr);

        IGame* instance;
        float size = 1.0f;

        std::string assetPath;
        std::string texturePath;

        ConstantBuffer<ConstBuffers::VSVertex> pConstBuffer;
        IndexBuffer pIndexBuffer;
        VertexBuffer<Vertex> pVertexBuffer;
        PixelShader pPixelShader;
        VertexShader pVertexShader;

        DirectX::SimpleMath::Vector3 position;
        
        DirectX::SimpleMath::Matrix tempMatrix = DirectX::SimpleMath::Matrix::Identity;
		
        DirectX::SimpleMath::Matrix m_world = DirectX::SimpleMath::Matrix::Identity;
        DirectX::SimpleMath::Matrix m_view = DirectX::SimpleMath::Matrix::Identity;
        DirectX::SimpleMath::Matrix m_proj = DirectX::SimpleMath::Matrix::Identity;
    };
}

