#pragma once

namespace Renderer
{
	namespace ScreenConfig
	{
		constexpr const unsigned int SCREEN_WIDTH = 1280;
		constexpr const unsigned int SCREEN_HEIGHT = 720;
	}

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

	int Execute();
}