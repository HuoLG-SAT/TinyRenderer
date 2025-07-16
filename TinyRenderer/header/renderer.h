#pragma once

namespace Renderer
{
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

	class Window;
	class Camera;
	class GUIManager;
	class SceneManager;
	class ResourceManager;
	class LightManager;
	class ShaderManager;
	class GridManager;
	class Shader;
	class Model;
	class ScreenPostProcess;
	class SkyBoxManager;
	class GPUInstanceManager;
	class ShadowMapManager;

	extern Window& WindowI;
	extern Camera& CameraI;
	extern GUIManager& GUIManagerI;
	extern SceneManager& SceneManagerI;
	extern ResourceManager& ResourceManagerI;
	extern LightManager& LightManagerI;
	extern ShaderManager& ShaderManagerI;
	extern GridManager& GridManagerI;
	extern ScreenPostProcess& ScreenPostProcessI;
	extern SkyBoxManager& SkyBoxManagerI;
	extern GPUInstanceManager& GPUInstanceManagerI;
	extern ShadowMapManager& ShadowMapManagerI;

	bool Init();
	void UserInput();
	void Updata();
	void Render();
	bool IsClose();
}