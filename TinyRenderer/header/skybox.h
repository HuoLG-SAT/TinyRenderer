#pragma once
#include"shader.h"
#include"renderer.h"
#include"mesh.h"
namespace Renderer
{
	enum class SkyBoxCubeMapType
	{
		Glacier,
		Palace
	};

	class SkyBox
	{
		friend class SkyBoxManager;
	private:
		SkyBox();
		~SkyBox();
		
		bool Init();
		void Draw(Shader& shader);

	private:
		Model* skybox;
	};

	class SkyBoxManager
	{
		friend class SkyBox;
		friend class GUIManager;
		friend class SceneManager;
	public:
		bool InitSkyBox();
		void Draw();
		bool IsEnableSkyBox() const;

		static SkyBoxManager& Instance;

	private:
		SkyBoxManager();
		~SkyBoxManager();
		SkyBoxManager(const SkyBoxManager&) = delete;
		SkyBoxManager(SkyBoxManager&&) = delete;
		SkyBoxManager& operator=(const SkyBoxManager&) = delete;
		SkyBoxManager& operator=(SkyBoxManager&&) = delete;

		void ChooseSkyBoxCubeMap();

	private:
		static SkyBoxManager instance;
		bool isEnableSkyBox;
		SkyBoxCubeMapType skyBoxCubeMapType;

		SkyBox skyBox;
		Shader* skyBoxShader;
		Cubemap* skyBoxCubemap;
	};
}
