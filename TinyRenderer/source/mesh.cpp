#include"../header/mesh.h"
#include"../header/shader.h"
#include"../depend/stb_image/stb_image.h"
#include"../header/resourceManager.h"
void Renderer::Material::Apply(Shader& shader,ShaderType shaderType)
{
	switch (shaderType)
	{
		case Renderer::ShaderType::Default:
			ApplyDefaultMaterial(shader);
			break;

		case Renderer::ShaderType::Cutout:
			ApplyCutoutMaterila(shader);
			break;

		case Renderer::ShaderType::Transparent:
			ApplyTransparentMaterial(shader);
			break;

		case Renderer::ShaderType::Expland:
			ApplyExplandMaterial(shader);
			break;

		case Renderer::ShaderType::Planet:
			ApplyPlanetMaterial(shader);
			break;
	}
}
void Renderer::Material::ApplyDefaultMaterial(Shader& shader)
{
	shader.SetFloat(MATERIAL_SHINESS, shininess);
	shader.SetVec3(MATERIAL_DIFFUSE_COLOR, diffuseColor);
	shader.SetVec3(MATERIAL_SPECULAR_COLOR, specularColor);
	shader.SetVec3(MATERIAL_AMBIENT_COLOR, ambientColor);
	int i = 0;
	for (auto& pair : textures)
	{
		if (pair.second)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, pair.second->id);
			switch (pair.second->type)
			{
				case TextureType::Diffuse:
					shader.SetInt(MATERIAL_DIFFUSE_MAP, i);
					break;

				case TextureType::Specular:
					shader.SetInt(MATERIAL_SPECULAR_MAP, i);
					break;
			}
			i++;
		}
	}
}
void Renderer::Material::ApplyCutoutMaterila(Shader& shader)
{
	if (textures.count(TextureType::Diffuse) > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[TextureType::Diffuse]->id);
		shader.SetInt(CUTOUT_TEXTURE, 0);
	}
}
void Renderer::Material::ApplyTransparentMaterial(Shader& shader)
{
	if (textures.count(TextureType::Diffuse) > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[TextureType::Diffuse]->id);
		shader.SetInt(TRANSPARENT_TEXTURE, 0);
	}
}
void Renderer::Material::ApplyExplandMaterial(Shader& shader)
{
	if (textures.count(TextureType::Diffuse) > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[TextureType::Diffuse]->id);
		shader.SetInt(EXPLANDE_TEXTURE, 0);
	}
}
void Renderer::Material::ApplyPlanetMaterial(Shader& shader)
{
	if (textures.count(TextureType::Diffuse) > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[TextureType::Diffuse]->id);
		shader.SetInt(PLANET_TEXTURE, 0);
	}
}
void Renderer::Material::ApplyRockMaterial(Shader& shader)
{
	if (textures.count(TextureType::Diffuse) > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[TextureType::Diffuse]->id);
		shader.SetInt(ROCK_TETXURE, 0);
	}
}

Renderer::Mesh::Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> indexs,Material material)
	:vertexs(vertexs), indexs(indexs), material(material)
{
	SetupMesh();
}
Renderer::Mesh::~Mesh() = default;
void Renderer::Mesh::Dispose()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	VAO = 0;
	VBO = 0;
	EBO = 0;
}
void Renderer::Mesh::Draw(Shader& shader, ShaderType shaderType)
{
	if (shaderType == Renderer::ShaderType::Default		||
		shaderType == Renderer::ShaderType::Cutout		||
		shaderType == Renderer::ShaderType::Transparent || 
		shaderType == Renderer::ShaderType::Expland		||
		shaderType == Renderer::ShaderType::Planet)
	{
		material.Apply(shader, shaderType);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexs.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Renderer::Mesh::SetupMesh()
{
	//VAO
	glGenVertexArrays(1, &VAO);		//创建VAO顶点数组对象
	glBindVertexArray(VAO);			//绑定VAO顶点数组对象
	//VBO
	glGenBuffers(1, &VBO);																						//创建VBO顶点缓冲区对象															
	glBindBuffer(GL_ARRAY_BUFFER, VBO);																			//绑定VBO顶点缓冲区	
	glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(Vertex), vertexs.data(), GL_STATIC_DRAW);				//向绑定的VBO顶点缓冲区填充顶点数据	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);									//配置顶点位置属性	
	glEnableVertexAttribArray(0);																				//启用顶点位置属性	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));			//配置顶点法线属性	
	glEnableVertexAttribArray(1);																				//启用顶点法线属性	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));			//配置顶点纹理坐标属性	
	glEnableVertexAttribArray(2);																				//启用顶点纹理坐标属性	
	//EBO
	glGenBuffers(1, &EBO);																			 			//创建EBO元素缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);																	//绑定EBO元素缓冲区对象
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexs.size() * sizeof(unsigned int), indexs.data(), GL_STATIC_DRAW);	//填充数据
	//解除绑定
	glBindVertexArray(0);						//解除绑定VAO顶点数组对象
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//解除绑定VBO顶点缓冲区
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//解除绑定EBO元素缓冲区对象
}

unsigned int Renderer::TextureUtility::LoadTexture(const char* path, bool isSRGB)
{
	int width, height, channel;
	unsigned char* data = stbi_load(path, &width, &height, &channel, 0);
	if (!data)
	{
		std::cout << "Load Texture Fail Path:" << path << std::endl;
		stbi_image_free(data);
		return -1;
	}
	unsigned int textureFormat = isSRGB ? GL_SRGB : GL_RGB;
	unsigned int dataFormat = GL_RGB;
	switch (channel)
	{
		case 3: 
			textureFormat = isSRGB ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
			break;

		case 4: 
			textureFormat = isSRGB ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
			break;
	}
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return textureId;
}
unsigned int Renderer::CubemapUtility::LoadCubemap(const std::vector<const char*>& path, bool isSRGB)
{
	unsigned int cubemapId;
	glGenTextures(1, &cubemapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

	int width, height, channel;
	for (int i = 0; i < path.size(); i++)
	{
		unsigned char* data = stbi_load(path[i], &width, &height, &channel, 0);
		if (!data)
		{
			std::cout << "Load Cube Map Fail In Path" << path[i] << std::endl;
			stbi_image_free(data);
			glDeleteTextures(1, &cubemapId);
			return -1;
		}

		unsigned int textureFormat = isSRGB ? GL_SRGB : GL_RGB;
		unsigned int dataFormat = GL_RGB;
		switch (channel)
		{
			case 3:
				textureFormat = isSRGB ? GL_SRGB : GL_RGB;
				dataFormat = GL_RGB;
				break;

			case 4:
				textureFormat = isSRGB ? GL_SRGB_ALPHA : GL_RGBA;
				dataFormat = GL_RGBA;
				break;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return cubemapId;
}