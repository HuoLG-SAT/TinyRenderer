#include"../header/instance.h"
#include"../header/resourceManager.h"
Renderer::GPUInstanceManager::GPUInstanceManager() = default;
Renderer::GPUInstanceManager::~GPUInstanceManager()
{
	if (plant)
	{
		delete plant;
		plant = nullptr;
	}
	if (rock)
	{
		delete rock;
		rock = nullptr;
	}

	glDeleteBuffers(1, &RVBO);
	RVBO = 0;
}
bool Renderer::GPUInstanceManager::Init()
{
	plantShader = ShaderManagerI.GetShader(ShaderType::Planet);
	if (!plantShader)
	{
		std::cout << "Plane Shader Is Empty!" << std::endl;
		return false;
	}
	rockShader = ShaderManagerI.GetShader(ShaderType::Rock);
	if (!rockShader)
	{
		std::cout << "Rock Shader Is Empty!" << std::endl;
		return false;
	}

	Model* plantModel = ResourceManagerI.LoadModel(PLANET_MODEL_PATH);
	if (!plantModel)
	{
		std::cout << "Plant Mode Is Empty!" << std::endl;
		return false;
	}
	Model* rockModel = ResourceManagerI.LoadModel(ROCK_MODEL_PATH);
	if (!rockModel)
	{
		std::cout << "Rock Mode Is Empty!" << std::endl;
		return false;
	}
 	plant = new Model(*plantModel);
	rock = new Model(*rockModel);

	glm::mat4* models = new glm::mat4[MAX_ROCK_INSTANCE_NUM];
	for (int i = 0; i < MAX_ROCK_INSTANCE_NUM; i++)
	{
		models[i] = std::move(GPUInstanceUtility::GenerateRockModel(i));
	}
	glGenBuffers(1, &RVBO);
	glBindBuffer(GL_ARRAY_BUFFER, RVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_ROCK_INSTANCE_NUM, models, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (auto& mesh : rock->meshs)
	{
		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, RVBO);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);

		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);

		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] models;
	models = nullptr;

	return true;
}
void Renderer::GPUInstanceManager::Draw()
{
	if (!isEnableDrawPlant)
	{
		return;
	}

	//绘制行星
	plantShader->Use();
	plant->Draw(*plantShader, ShaderType::Planet);

	//绘制岩石
	rockShader->Use();
	for (auto& mesh : rock->meshs)
	{
		mesh.material.ApplyRockMaterial(*rockShader);
		glBindVertexArray(mesh.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, mesh.indexs.size(), GL_UNSIGNED_INT, (void*)0, MAX_ROCK_INSTANCE_NUM);
	}
}
Renderer::GPUInstanceManager& Renderer::GPUInstanceManager::Instance = instance;
Renderer::GPUInstanceManager Renderer::GPUInstanceManager::instance;
