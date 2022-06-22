﻿#include "Model.h"

#include <d3dcompiler.h>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Framework/Graphics.h"
#include "Framework/IGame.h"
#include "Framework/VertexBuffer.h"
#include "Framework/Color.h"

using namespace DirectX::SimpleMath;

namespace BaseFramework
{
	namespace Colors
	{
		const Color UnloadedTextureColor(100, 100, 100);
		const Color UnhandledTextureColor(250, 0, 0);
	}
	
    Model::Model(const std::string& path, float size) : size(size)
    {
        assetPath = path;
    }

    void Model::Initialize(BaseFramework::IGame* game)
    {
        this->instance = game;
        auto graphics = instance->GetGfx();
        auto device = graphics->GetDevice();
        auto context = graphics->GetContext();

    	this->device = graphics->GetDevice().Get();
    	this->context = graphics->GetContext().Get();

    	Assimp::Importer importer;

    	const aiScene* pScene = importer.ReadFile(assetPath,
													aiProcess_Triangulate |
													aiProcess_ConvertToLeftHanded);

    	this->ProcessNode(pScene->mRootNode, pScene);

    	pConstBuffer.Initialize(device.Get(), context.Get());
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
            {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
        };

        UINT numElements = ARRAYSIZE(layout);

        pVertexShader.Initialize(device, L"VSModel.cso", layout, numElements);
        pPixelShader.Initialize(device,  + L"PSModel.cso");
    }

    void BaseFramework::Model::Update(DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_view,
        DirectX::SimpleMath::Matrix m_proj)
    {
        this->m_view = m_view;
        this->m_proj = m_proj;
        this->m_world = (tempMatrix.Invert() * DirectX::SimpleMath::Matrix::CreateTranslation(m_world.Translation()));
        tempMatrix = Matrix::Identity;
    }

    void BaseFramework::Model::Draw()
    {
        auto graphics = instance->GetGfx();
        auto context = graphics->GetContext();

        pConstBuffer.data = (m_world * m_view * m_proj).Transpose();
        pConstBuffer.ApplyChanges();
        context->VSSetConstantBuffers(0, 1u, pConstBuffer.GetAddressOf());

        context->VSSetShader(pVertexShader.GetShader(), nullptr, 0u);
        context->PSSetShader(pPixelShader.GetShader(), nullptr, 0u);
        context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(pVertexShader.GetInputLayout());

    	for (int i = 0; i < meshes.size(); i++)
    	{
    		meshes[i].Draw();
    	}
    }

    void BaseFramework::Model::ClearResources()
    {
    }

    void BaseFramework::Model::Rotate(RotDirection direction, float angle)
    {
    }

    void BaseFramework::Model::Translate(float x, float y, float z)
    {
    }

    void Model::ProcessNode(aiNode * node, const aiScene * scene)
	{
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(this->ProcessMesh(mesh, scene));
		}

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;

		//Get vertices
		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position.x = mesh->mVertices[i].x * size;
			vertex.position.y = mesh->mVertices[i].y * size;
			vertex.position.z = mesh->mVertices[i].z * size;

			/*vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;*/

			if (mesh->mTextureCoords[0])
			{
				vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(vertex);
		}

		//Get indices
		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		std::vector<Texture> textures;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

		return Mesh(this->device, this->context, vertices, indices, textures);
	}

	TextureStorageType Model::DetermineTextureStorageType(const aiScene * pScene, aiMaterial * pMat, unsigned int index, aiTextureType textureType)
	{
		if (pMat->GetTextureCount(textureType) == 0)
			return TextureStorageType::None;

		aiString path;
		pMat->GetTexture(textureType, index, &path);
		std::string texturePath = path.C_Str();
		//Check if texture is an embedded indexed texture by seeing if the file path is an index #
		if (texturePath[0] == '*')
		{
			if (pScene->mTextures[0]->mHeight == 0)
			{
				return TextureStorageType::EmbeddedIndexCompressed;
			}
			else
			{
				assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
				return TextureStorageType::EmbeddedIndexNonCompressed;
			}
		}
		//Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
		if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
		{
			if (pTex->mHeight == 0)
			{
				return TextureStorageType::EmbeddedCompressed;
			}
			else
			{
				assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
				return TextureStorageType::EmbeddedNonCompressed;
			}
		}
		//Lastly check if texture is a filepath by checking for period before extension name
		if (texturePath.find('.') != std::string::npos)
		{
			return TextureStorageType::Disk;
		}

		return TextureStorageType::None; // No texture exists
	}

	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene)
	{
		std::vector<Texture> materialTextures;
		TextureStorageType storetype = TextureStorageType::Invalid;
		unsigned int textureCount = pMaterial->GetTextureCount(textureType);

		if (textureCount == 0) //If there are no textures
		{
			storetype = TextureStorageType::None;
			aiColor3D aiColor(0.0f, 0.0f, 0.0f);
			switch (textureType)
			{
			case aiTextureType_DIFFUSE:
				pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				if (aiColor.IsBlack()) //If color = black, just use grey
				{
					materialTextures.push_back(Texture(this->device, Colors::UnloadedTextureColor, textureType));
					return materialTextures;
				}
				materialTextures.push_back(Texture(this->device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
				return materialTextures;
			}
		}
		else
		{
			for (UINT i = 0; i < textureCount; i++)
			{
				aiString path;
				pMaterial->GetTexture(textureType, i, &path);
				TextureStorageType storetype = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
				switch (storetype)
				{
				case TextureStorageType::EmbeddedIndexCompressed:
				{
					int index = GetTextureIndex(&path);
					Texture embeddedIndexedTexture(	this->device,
													reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
													pScene->mTextures[index]->mWidth,
													textureType);
					materialTextures.push_back(embeddedIndexedTexture);
					break;
				}
				case TextureStorageType::EmbeddedCompressed:
				{
					const aiTexture * pTexture = pScene->GetEmbeddedTexture(path.C_Str());
					Texture embeddedTexture(this->device,
											reinterpret_cast<uint8_t*>(pTexture->pcData),
											pTexture->mWidth,
											textureType);
					materialTextures.push_back(embeddedTexture);
					break;
				}
				case TextureStorageType::Disk:
				{
					std::string filename = '\\' + path.C_Str();
					Texture diskTexture(this->device, filename, textureType);
					materialTextures.push_back(diskTexture);
					break;
				}
				}
			}
		}

		if (materialTextures.size() == 0)
		{
			materialTextures.push_back(Texture(this->device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
		}
		return materialTextures;

	}

	int Model::GetTextureIndex(aiString * pStr)
	{
		assert(pStr->length >= 2);
		return atoi(&pStr->C_Str()[1]);
	}
}
