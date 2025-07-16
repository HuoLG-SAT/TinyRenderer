#include"../header/grid.h"
#include"../header/shader.h"
#include"../header/camera.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
Renderer::GridManager::GridManager():enableGrid(true), gridShader(nullptr)
{

}
Renderer::GridManager::~GridManager() = default;
void Renderer::GridManager::Draw()
{
	if (!enableGrid)
	{
		return;
	}

	gridShader->Use();
	gridShader->SetSetMatrix4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0, -2.0f, 0)));
	gridShader->SetFloat("gridFadeDistance", GRID_FADE_DISTANCE);
	gridShader->SetVec3("gridLineColor", glm::vec3(1.0f));
	grid.Draw(*gridShader);
}
bool Renderer::GridManager::LoadGrid()
{
	gridShader = ShaderManagerI.GetShader(ShaderType::Grid);
	grid.InitGrid(50, 5, 1);
	grid.SetupGrid();
	return gridShader;
}
Renderer::GridManager& Renderer::GridManager::Instance = instance;
Renderer::GridManager Renderer::GridManager::instance;

Renderer::Grid::Grid()
	: VAO(0),VBO(0)
{ 
	
}
Renderer::Grid::~Grid()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	VAO = 0;
	VBO = 0;
}
void Renderer::Grid::Draw(Shader& shader)
{
	glStencilMask(0x00);	//关闭模板缓冲区写入
	glEnable(GL_BLEND);		//开启颜色混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, points.size());
	glBindVertexArray(0);
	glDisable(GL_BLEND);	//取消颜色混合
	glStencilMask(0xFF);	//开启模板缓冲区写入
}
void Renderer::Grid::InitGrid(int gridSize, int bigGridSet, int smallGridSet)
{
	for (int i = -gridSize; i <= gridSize; i += smallGridSet)
	{
		bool isMainLine = (fmod(i, bigGridSet) == 0);

		//处理X方向
		GridPoint x1;
		x1.position = glm::vec3(i, 0, -gridSize);
		x1.intensity = isMainLine ? 0.8f : 0.3f;
		points.push_back(x1);

		GridPoint x2;
		x2.position = glm::vec3(i, 0, gridSize);
		x2.intensity = isMainLine ? 0.8f : 0.3f;
		points.push_back(x2);

		//处理Z方向
		GridPoint z1;
		z1.position = glm::vec3(-gridSize, 0, i);
		z1.intensity = isMainLine ? 0.8f : 0.3f;
		points.push_back(z1);

		GridPoint z2;
		z2.position = glm::vec3(gridSize, 0, i);
		z2.intensity = isMainLine ? 0.8f : 0.3f;
		points.push_back(z2);
	}
}
void Renderer::Grid::SetupGrid()
{
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GridPoint) * points.size(), points.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, intensity));
	glEnableVertexAttribArray(1);
	//解除绑定
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}