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

#define DIFFUSE_MAP_ITEMS u8"木箱子",u8"墙",u8"木盒子",u8"图标",u8"草",u8"笑脸",u8"透明窗口",u8"自定义纹理"
#define SPECULAR_MAP_ITEMS u8"铁箱子",u8"白色",u8"自定义纹理"
#define DIFFUSE_MAP_NUM 8
#define SPECULAR_MAP_NUM 3

#define MODEL_ITEMS u8"正方体",u8"球",u8"平面",u8"自定义模型"
#define MODEL_NUM 4

#define SHADER_ITEMS u8"默认Shader",u8"纯色Shader",u8"透明切割Shader",u8"透明Shader",u8"反射Shader",u8"折射Shader",u8"扩张Shader"
#define SHADER_NUM 7

#define DISPLAY_ITEMS u8"静止",u8"绕X轴摇摆",u8"绕Y轴摇摆",u8"绕Z轴摇摆",u8"绕X轴旋转",u8"绕Y轴旋转",u8"绕Z轴旋转"
#define DISPLAY_NUM 7

#define SCREEN_POST_PROCESS_ITEMS u8"无",u8"灰度图",u8"颜色翻转",u8"锐化",u8"模糊",u8"边缘硬化"
#define SCREEN_POST_PROCESS_NUM 6

#define SKYBOX_CUBEMAP_ITEMS u8"冰川",u8"教堂"
#define SKYBOX_CUBEMAP_NUM 2

#define DIRECTIONAL_MOVE_AXIS_ITEMS u8"X轴",u8"Y轴",u8"Z轴"
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
