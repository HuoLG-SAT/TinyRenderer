#pragma once
#include"renderer.h"
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
#include<vector>

namespace Renderer
{
	struct GridPoint
	{
		glm::vec3 position;
		float intensity;
	};

	class Grid
	{
		friend class GridManager;

	private:
		Grid();
		~Grid();

		void Draw(Shader& shader);
		void InitGrid(int gridSize, int bigGridSet, int smallGridSet);
		void SetupGrid();

	private:
		std::vector<GridPoint> points;

		unsigned int VAO;
		unsigned int VBO;
	};

#define GRID_FADE_DISTANCE (50)

	class GridManager
	{
		friend class GUIManager;
	public:

		void Draw();
		bool LoadGrid();

		static GridManager& Instance;
	private:
		GridManager();
		~GridManager();
		GridManager(const GridManager&) = delete;
		GridManager(GridManager&&) = delete;
		GridManager& operator=(const GridManager&) = delete;
		GridManager& operator=(GridManager&&) = delete;

	private:
		Grid grid;
		Shader* gridShader;
		bool enableGrid;

		static GridManager instance;
	};
}