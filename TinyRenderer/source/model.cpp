#include"../header/model.h"
#include"../header/resourceManager.h"
#include<iostream>
Renderer::Model::Model() = default;
Renderer::Model::~Model() = default;
bool Renderer::Model::Load(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	//将多边形转换为三角形、翻转UV坐标
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Model Load Error: " << importer.GetErrorString() << " In Path: " << path << std::endl;
		return false;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	ProcessNode(scene->mRootNode, scene);
	return true;
}
void Renderer::Model::Draw(Shader& shader, ShaderType shaderType)
{
	for (auto& mesh : meshs)
	{
		mesh.Draw(shader, shaderType);
	}
}
void Renderer::Model::SetModelMap(TextureType type, Texture* texture)
{
	if (!texture)
	{
		std::cout << "Set Texture Is Empty!" << std::endl;
		return;
	}

	for (auto& mesh : meshs)
	{
		if (mesh.material.textures.count(type) > 0)
		{
			mesh.material.textures[type] = texture;
		}
		else
		{
			mesh.material.textures.emplace(type, texture);
		}
	}
}
void Renderer::Model::SetModelMap(TextureType type, const std::string& path)
{
	SetModelMap(type, ResourceManagerI.GetTexture(path));
}
void Renderer::Model::SetModelMap(TextureType type, unsigned int value)
{
	SetModelMap(type, ResourceManagerI.GetTexture(value));
}
Renderer::Texture* Renderer::Model::GetModeMap(TextureType type)
{
	for (auto& mesh : meshs)
	{
		if (mesh.material.textures.count(type) > 0)
		{
			return ResourceManagerI.MmapRGBTexture(mesh.material.textures[type]);
		}
	}
	return nullptr;
}
void Renderer::Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		meshs.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
Renderer::Mesh Renderer::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertexs;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
		vertex.texCoord = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
		vertexs.push_back(vertex);
	}

	std::vector<unsigned int> indexs;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indexs.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertexs, indexs, ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex]));
}
Renderer::Material Renderer::Model::ProcessMaterial(aiMaterial* _aiMaterial)
{
	Material material;
	//处理漫反射
	aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
	material.diffuseColor = glm::clamp((_aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) ? glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b) : glm::vec3(0.8f), 0.65f, 0.9f);
	//处理高光反射
	aiColor3D specularColor(0.0f, 0.0f, 0.0f);
	material.specularColor = glm::clamp((_aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) ? glm::vec3(specularColor.r, specularColor.g, specularColor.b) : glm::vec3(0.65f), 0.55f, 0.75f);
	//环境光反射
	aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
	material.ambientColor = glm::clamp((_aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS) ? glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b) : glm::vec3(0.55f), 0.55f, 0.65f);
	//光泽度
	material.shininess = glm::clamp((_aiMaterial->Get(AI_MATKEY_SHININESS, material.shininess) == AI_SUCCESS) ? material.shininess : 75.0f, 75.0f, 255.0f);
	//漫反射纹理
	Texture* diffuseTexture = ProcessTexture(_aiMaterial, aiTextureType_DIFFUSE, TextureType::Diffuse, true);
	diffuseTexture = diffuseTexture ? diffuseTexture : ResourceManagerI.GetTexture(HUOLG_TEXTURE_PATH);
	if (diffuseTexture != nullptr && material.textures.count(TextureType::Diffuse) <= 0)
	{
		material.textures.emplace(TextureType::Diffuse, diffuseTexture);
	}
	//高光反射纹理
	Texture* specularTexture = ProcessTexture(_aiMaterial, aiTextureType_SPECULAR, TextureType::Specular, true);
	specularTexture = specularTexture ? specularTexture : ResourceManagerI.GetTexture(WHITE_TEXTURE_PATH);
	if (specularTexture != nullptr && material.textures.count(TextureType::Specular) <= 0)
	{
		material.textures.emplace(TextureType::Specular, specularTexture);
	}
	return material;
}
Renderer::Texture* Renderer::Model::ProcessTexture(aiMaterial* _aiMaterial, aiTextureType type, TextureType textureType,bool isSRGB)
{
	if (_aiMaterial->GetTextureCount(type) > 0)
	{
		aiString name;
		_aiMaterial->GetTexture(type, 0, &name);
		std::string path = directory + "\\" + std::string(name.C_Str());
		return Renderer::ResourceManagerI.LoadTexture(path, textureType, std::string(name.C_Str()), isSRGB);
	}
	return nullptr;
}
void Renderer::Model::Dispose()
{
	for (auto& mesh : meshs)
	{
		mesh.Dispose();
	}
	meshs.clear();
}