#pragma once
#include"gameObject.h"
#include<vector>
#include<map>
namespace Renderer
{
	class SceneManager
	{
	public:
		void Updata();
		void AddGameObject();
		void DrawGeometry();
		void DrawTransparent();
		void SetDirty();
		GameObject* GetGameObject(int id);

		static SceneManager& Instance;

	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

		void SortTransparentGOs();

	public:
		std::unordered_map<int, GameObject*> gameObjectMap;
		std::vector<GameObject*> gameObjectList;
		std::vector<GameObject*> defaultShaderGOs;
		std::vector<GameObject*> solidColorShaderGOs;
		std::vector<GameObject*> outLineShaderGOs;
		std::vector<GameObject*> cutoutShaderGOs;
		std::vector<GameObject*> transparentShaderGosList;
		std::vector<GameObject*> reflectShaderGOs;
		std::vector<GameObject*> refractShaderGOs;
		std::vector<GameObject*> explandShaderGOs;
		std::vector<GameObject*> normalShadeGOs;
		std::map<float, GameObject*> transparentShaderGos;
		bool isDirty;
		int ids;
		static SceneManager instance;
	};
}