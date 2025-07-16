#pragma once
#include"../depend/imgui/imgui.h"
#include"../depend/imgui/imgui_impl_glfw.h"
#include"../depend/imgui/imgui_impl_opengl3.h"

namespace Renderer
{
#define WOOD_DIFFUSE_INDEX 0
#define WALL_DIFFUSE_INDEX 1
#define BOX_DIFFUSE_INDEX 2
#define ICON_DIFFUSE_INDEX 3
#define GRESS_CUTOOUT_DIFFUSE_INDEX 4
#define FACE_CUTOOUT_DIFFUSE_INDEX 5
#define WINDOW_TRANSPARENT_DIFFUSE_INDEX 6
#define CUSTOM_DIFFUSE_INDEX 7

#define IRONY_SPECULAR_INDEX 0
#define WHITE_SPECULAR_INDEX 1
#define CUSTOM_SPECULAR_INDEX 2

#define DIFFUSE_MAP_ITEMS u8"ľ����",u8"ǽ",u8"ľ����",u8"ͼ��",u8"��",u8"Ц��",u8"͸������",u8"�Զ�������"
#define SPECULAR_MAP_ITEMS u8"������",u8"��ɫ",u8"�Զ�������"
#define DIFFUSE_MAP_NUM 8
#define SPECULAR_MAP_NUM 3

#define MODEL_ITEMS u8"������",u8"��",u8"ƽ��",u8"�Զ���ģ��"
#define MODEL_NUM 4

#define SHADER_ITEMS u8"Ĭ��Shader",u8"��ɫShader",u8"͸���и�Shader",u8"͸��Shader",u8"����Shader",u8"����Shader",u8"����Shader"
#define SHADER_NUM 7

#define DISPLAY_ITEMS u8"��ֹ",u8"��X��ҡ��",u8"��Y��ҡ��",u8"��Z��ҡ��",u8"��X����ת",u8"��Y����ת",u8"��Z����ת"
#define DISPLAY_NUM 7

#define SCREEN_POST_PROCESS_ITEMS u8"��",u8"�Ҷ�ͼ",u8"��ɫ��ת",u8"��",u8"ģ��",u8"��ԵӲ��"
#define SCREEN_POST_PROCESS_NUM 6

#define SKYBOX_CUBEMAP_ITEMS u8"����",u8"����"
#define SKYBOX_CUBEMAP_NUM 2

#define DIRECTIONAL_MOVE_AXIS_ITEMS u8"X��",u8"Y��",u8"Z��"
#define DIRECTIONAL_MOVE_AXIS_NUM 3

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
		ImGuiIO* io;
		bool isShowScene;
		bool isShowLight;
		bool isShowScreenPostProcess;
		bool isShowSkyBox;
		bool isShowPlanet;
		static GUIManager instance;
	};
}
