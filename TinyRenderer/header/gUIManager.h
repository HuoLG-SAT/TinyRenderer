#pragma once
#include"../depend/imgui/imgui.h"
#include"../depend/imgui/imgui_impl_glfw.h"
#include"../depend/imgui/imgui_impl_opengl3.h"

namespace Renderer
{
	class GameObject;
	class GUIManager
	{
	public:
		bool InitImGui();
		void MarkStart();
		void RenderGUI();

		static GUIManager& Instance;

	private:
		GUIManager();
		~GUIManager();
		GUIManager(const GUIManager&) = delete;
		GUIManager(GUIManager&&) = delete;
		GUIManager& operator=(const GUIManager&) = delete;
		GUIManager& operator=(GUIManager&&) = delete;

		void MainMenuGUI();
		void LightGUI();
		void ScreenPostProcessGUI();
		void SkyBoxGUI();
		void PlanetGUI();
		void SceneGUI();
		void GameObjectGUI();
		void GameObjectGUI(GameObject& gameObject);
		void DrawGameObjectDefaultShaderGUI(GameObject& gameObject);
		void DrawGameObjectSolidColorShaderGUI(GameObject& gameObject);
		void DrawGameObjectCutoutShaderGUI(GameObject& gameObject);
		void DrawGameObjectTransparentShaderGUI(GameObject& gameObject);
		void DrawGameObjectReflectShaderGUI(GameObject& gameObject);
		void DrawGameObjectRefractShaderGUI(GameObject& gameObject);
		void DrawGameObjectExplandShaderGUI(GameObject& gameObject);

	private:
		ImGuiIO* io = nullptr;
		bool isShowScene = false;
		bool isShowLight = false;
		bool isShowScreenPostProcess = false;
		bool isShowSkyBox = false;
		bool isShowPlanet = false;
		static GUIManager instance;
	};
}
