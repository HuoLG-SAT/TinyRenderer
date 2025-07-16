#include"../header/gUIManager.h"
#include"../header/window.h"
#include"../header/renderer.h"
#include"../header/sceneManager.h"
#include"../header/camera.h"
#include"../depend/tinyfiledialogs/tinyfiledialogs.h"
#include"../header/lightManager.h"
#include"../header/resourceManager.h"
#include"../header/screenPostProcess.h"
#include"../header/skybox.h"
#include"../header/grid.h"
#include"../header/instance.h"
#include"../header/shadowMapManager.h"

Renderer::GUIManager::GUIManager()
	:io(nullptr), isShowScene(false), isShowLight(false), isShowScreenPostProcess(false), isShowSkyBox(false), isShowPlanet(false)
{

}
Renderer::GUIManager::~GUIManager()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
bool Renderer::GUIManager::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(Renderer::WindowI.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
	IM_ASSERT(io->Fonts->AddFontFromFileTTF("resource\\fonts\\STKAITI.TTF", 18.0f, nullptr, io->Fonts->GetGlyphRangesChineseSimplifiedCommon()) != nullptr);
	return true;
}
void Renderer::GUIManager::MarkStart()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void Renderer::GUIManager::RenderGUI()
{
	MainMenuGUI();
	if (isShowScene)
	{
		SceneGUI();
		GameObjectGUI();
	}
	if (isShowLight)
	{
		LightGUI();
	}
	if (isShowScreenPostProcess)
	{
		ScreenPostProcessGUI();
	}
	if (isShowSkyBox)
	{
		SkyBoxGUI();
	}
	if (isShowPlanet)
	{
		PlanetGUI();
	}
	ImGui::Render();

	glDisable(GL_FRAMEBUFFER_SRGB);		//取消Gamma矫正,防止UI过亮
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glEnable(GL_FRAMEBUFFER_SRGB);		//恢复Gamma矫正
}
void Renderer::GUIManager::MainMenuGUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"场景菜单"))
		{
			if (ImGui::MenuItem(u8"打开"))
			{
				isShowScene = true;
			}
			if (ImGui::MenuItem(u8"关闭"))
			{
				isShowScene = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"光照菜单"))
		{
			if (ImGui::MenuItem(u8"打开"))
			{
				isShowLight = true;
			}
			if (ImGui::MenuItem(u8"关闭"))
			{
				isShowLight = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"后处理菜单"))
		{
			if (ImGui::MenuItem(u8"打开"))
			{
				isShowScreenPostProcess = true;
			}
			if (ImGui::MenuItem(u8"关闭"))
			{
				isShowScreenPostProcess = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"天空盒菜单"))
		{
			if (ImGui::MenuItem(u8"打开"))
			{
				isShowSkyBox = true;
			}
			if (ImGui::MenuItem(u8"关闭"))
			{
				isShowSkyBox = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"行星带菜单"))
		{
			if (ImGui::MenuItem(u8"打开"))
			{
				isShowPlanet = true;
			}
			if (ImGui::MenuItem(u8"关闭"))
			{
				isShowPlanet = false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void Renderer::GUIManager::LightGUI()
{
	ImGui::Begin(u8"光照信息");

	ImGui::Text(u8"平行光");
	ImGui::BeginChild("平行光", ImVec2(450, 120), true);
	ImGui::Checkbox(u8"是否开启平行光", &Renderer::LightManagerI.isOpenDirectionalLight);
	if (Renderer::LightManagerI.isOpenDirectionalLight)
	{
		DirectionalLight& directionalLight = Renderer::LightManagerI.directionalLight;
		ImGui::ColorEdit3(u8"漫反射颜色", glm::value_ptr(directionalLight.diffuse));
		ImGui::ColorEdit3(u8"镜面反射颜色", glm::value_ptr(directionalLight.specular));
		if (ImGui::Checkbox(u8"拿起", &directionalLight.isPickUp))
		{
			if (directionalLight.isPickUp && directionalLight.isSelfMovement)
			{
				directionalLight.isSelfMovement = false;
			}
		}
		if (ImGui::Checkbox(u8"自运动", &directionalLight.isSelfMovement))
		{
			if (directionalLight.isSelfMovement && directionalLight.isPickUp)
			{
				directionalLight.isPickUp = false;
			}
		}
		if (directionalLight.isSelfMovement)
		{
			ImGui::SliderFloat(u8"运动速度", &directionalLight.speed, 0.0f, 3.0f);
			const char* axisItems[] = { DIRECTIONAL_MOVE_AXIS_ITEMS };
			ImGui::Combo(u8"选择运动轴", reinterpret_cast<int*>(&directionalLight.moveAxis), axisItems, DIRECTIONAL_MOVE_AXIS_NUM);
		}
		ImGui::Checkbox(u8"是否开启光源投射阴影", &Renderer::ShadowMapManagerI.isEnableDirectionalShadowCaster);
	}
	ImGui::EndChild();
	ImGui::NewLine();

	ImGui::Text(u8"点光源");
	ImGui::BeginChild("点光源", ImVec2(450, 200), true);
	ImGui::Checkbox(u8"是否开启点光源", &Renderer::LightManagerI.isOpenPointLight);
	if (Renderer::LightManagerI.isOpenPointLight)
	{
		if (ImGui::Button(u8"增加点光源数量"))
		{
			Renderer::LightManagerI.AddPointNum();
		}
		if (ImGui::Button(u8"减少点光源数量"))
		{
			Renderer::LightManagerI.DecreasePointNum();
		}
		ImGui::Checkbox(u8"是否开启光源投射阴影##", &Renderer::ShadowMapManagerI.isEnablePointLightShadowCaster);
		int pointLightNum = Renderer::LightManagerI.pointLightNum;
		ImGui::Text(u8"点光源数量%d", pointLightNum);
		for (int i = 0; i < pointLightNum && i < MAX_POINT_NUM; i++)
		{
			if (ImGui::TreeNode((std::string(u8"点光源 ") + std::to_string(i + 1)).c_str()))
			{
				PointLight& pointLight = Renderer::LightManagerI.pointLights[i];
				ImGui::InputFloat3(u8"位置", glm::value_ptr(pointLight.position));
				ImGui::ColorEdit3(u8"漫反射颜色", glm::value_ptr(pointLight.diffuse));
				ImGui::ColorEdit3(u8"镜面反射颜色", glm::value_ptr(pointLight.specular));
				ImGui::SliderFloat(u8"常量衰减系数", &pointLight.constant, 0.5f, 1.5f);
				ImGui::SliderFloat(u8"线性衰减系数", &pointLight.linear, 0.01f, 0.2f);
				ImGui::SliderFloat(u8"二次衰减系数", &pointLight.quadratic, 0.01f, 0.05f);
				ImGui::Checkbox(u8"拿起点光源", &pointLight.isPickUp);
				ImGui::TreePop();
			}
		}
	}
	ImGui::EndChild();
	ImGui::NewLine();

	ImGui::Text(u8"聚光灯");
	ImGui::BeginChild("聚光灯", ImVec2(450, 120), true);
	ImGui::Checkbox(u8"是否开启聚光灯", &Renderer::LightManagerI.isOpenSpotLight);
	if (Renderer::LightManagerI.isOpenSpotLight)
	{
		SpotLight& spotLight = Renderer::LightManagerI.spotLight;
		ImGui::InputFloat3(u8"位置##", glm::value_ptr(spotLight.position));
		ImGui::ColorEdit3(u8"漫反射颜色##", glm::value_ptr(spotLight.diffuse));
		ImGui::ColorEdit3(u8"镜面反射颜色##", glm::value_ptr(spotLight.specular));
		ImGui::SliderFloat(u8"内切光角##", &spotLight.inCutOff, 0.01f, 15.0f);
		ImGui::SliderFloat(u8"外切光角##", &spotLight.outCutOff, 0.01f, 20.0f);
		ImGui::Checkbox(u8"拿起##", &spotLight.isPickUp);
		ImGui::Checkbox(u8"是否开启光源投射阴影####", &Renderer::ShadowMapManagerI.isEnableSpotLightShadowCaster);
	}
	ImGui::EndChild();
	ImGui::NewLine();

	ImGui::Text(u8"环境光");
	ImGui::BeginChild("环境光", ImVec2(450, 80), true);
	ImGui::Checkbox(u8"是否开启环境光", &Renderer::LightManagerI.isOpenAmbient);
	if (Renderer::LightManagerI.isOpenAmbient)
	{
		ImGui::ColorEdit3(u8"环境光颜色", glm::value_ptr(Renderer::LightManagerI.ambient));
	}
	ImGui::EndChild();

	ImGui::End();
}
void Renderer::GUIManager::ScreenPostProcessGUI()
{
	ImGui::Begin(u8"后处理信息");

	ImGui::Checkbox(u8"是否开启后处理", &ScreenPostProcessI.isEnableScreenPostProcess);
	if (ScreenPostProcessI.isEnableScreenPostProcess)
	{
		ImGui::Text(u8"后处理效果");
		ImGui::BeginChild("后处理效果", ImVec2(400, 80), true);
		const char* screenPostProcessItems[] = { SCREEN_POST_PROCESS_ITEMS};
		ImGui::Combo(u8"选择使用的后处理效果", reinterpret_cast<int*>(&ScreenPostProcessI.type), screenPostProcessItems, SCREEN_POST_PROCESS_NUM);
		ImGui::EndChild();
	}

	ImGui::End();
}
void Renderer::GUIManager::SkyBoxGUI()
{
	ImGui::Begin(u8"天空盒信息");

	ImGui::Checkbox(u8"是否开启天空盒", &SkyBoxManagerI.isEnableSkyBox);
	if (SkyBoxManagerI.isEnableSkyBox)
	{
		ImGui::Text(u8"天空盒立方体纹理");
		ImGui::BeginChild("天空盒立方体纹理", ImVec2(400, 80), true);
		const char* skyboxCubemapItems[] = { SKYBOX_CUBEMAP_ITEMS };
		if (ImGui::Combo(u8"选择使用的天空盒立方体纹理", reinterpret_cast<int*>(&SkyBoxManagerI.skyBoxCubeMapType), skyboxCubemapItems, SKYBOX_CUBEMAP_NUM))
		{
			SkyBoxManagerI.ChooseSkyBoxCubeMap();
		}
		ImGui::EndChild();
	}

	ImGui::End();
}
void Renderer::GUIManager::PlanetGUI()
{
	ImGui::Begin(u8"行星带信息");

	ImGui::BeginChild("是否开启行星带", ImVec2(250, 60), true);
	ImGui::Checkbox(u8"是否开启行星带", &GPUInstanceManagerI.isEnableDrawPlant);
	ImGui::EndChild();

	ImGui::End();
}
void Renderer::GUIManager::SceneGUI()
{
	const glm::vec3& position = Renderer::CameraI.Position();
	ImGui::Begin(u8"场景信息");

	ImGui::BeginChild("场景信息", ImVec2(250, 60), true);
	ImGui::Text("FPS %d", static_cast<int>(io->Framerate));
	ImGui::Text(u8"摄像机位置X：%.1f Y：%.1f Z：%.1f", position.x, position.y, position.z);
	ImGui::EndChild();

	ImGui::BeginChild("场景对象信息", ImVec2(250, 85), true);
	ImGui::Text(u8"场景对象信息");
	ImGui::Text(u8"场景对象数量 %d", SceneManagerI.gameObjectList.size());
	if (ImGui::Button(u8"创造场景对象"))
	{
		Renderer::SceneManagerI.AddGameObject();
	}
	ImGui::EndChild();

	ImGui::BeginChild("启用网格", ImVec2(250, 60), true);
	ImGui::Checkbox(u8"启用网格", &GridManagerI.enableGrid);
	ImGui::EndChild();

	ImGui::End();
}
void Renderer::GUIManager::GameObjectGUI()
{
	for (auto& gameObjects : Renderer::SceneManagerI.gameObjectList)
	{
		GameObjectGUI(*gameObjects);
	}
}
void Renderer::GUIManager::GameObjectGUI(GameObject& gameObject)
{
	ImGui::PushID(gameObject.id);
	ImGui::Begin(u8"场景全部对象");
	if (ImGui::TreeNode(("GameObject " + std::to_string(gameObject.id)).c_str()))
	{
		ImGui::Text(u8"属性");
		ImGui::BeginChild("属性", ImVec2(400, 100), true);
		if (ImGui::InputFloat3(u8"位置", glm::value_ptr(gameObject.transform.Position()), "%.2f") ||
			ImGui::InputFloat3(u8"旋转", glm::value_ptr(gameObject.transform.Rotate()), "%.2f") ||
			ImGui::InputFloat3(u8"缩放", glm::value_ptr(gameObject.transform.Scale()), "%.2f"))
		{
			gameObject.transform.SetDirty();
		}
		ImGui::EndChild();

		ImGui::Text(u8"模型");
		ImGui::BeginChild("模型", ImVec2(400, 80), true);
		const char* modelItems[] = { MODEL_ITEMS };
		if (ImGui::Combo(u8"选择使用模型", reinterpret_cast<int*>(&gameObject.renderInfo.modelType), modelItems, MODEL_NUM))
		{
			gameObject.ChooseModel();
		}
		ImGui::EndChild();

		ImGui::Text(u8"Shader");
		ImGui::BeginChild("Shader", ImVec2(400, 150), true);
		const char* shaderItems[] = { SHADER_ITEMS };
		if (ImGui::Combo(u8"选择使用Shader", reinterpret_cast<int*>(&gameObject.renderInfo.shaderType), shaderItems, SHADER_NUM))
		{
			SceneManagerI.SetDirty();
		}
		switch (gameObject.renderInfo.shaderType)
		{
			case ShaderType::Default:
				DrawGameObjectDefaultShaderGUI(gameObject);
				break;

			case ShaderType::SolidColor:
				DrawGameObjectSolidColorShaderGUI(gameObject);
				break;

			case ShaderType::Cutout:
				DrawGameObjectCutoutShaderGUI(gameObject);
				break;

			case ShaderType::Transparent:
				DrawGameObjectTransparentShaderGUI(gameObject);
				break;

			case ShaderType::Reflect:
				DrawGameObjectReflectShaderGUI(gameObject);
				break;

			case ShaderType::Refract:
				DrawGameObjectRefractShaderGUI(gameObject);
				break;

			case ShaderType::Expland:
				DrawGameObjectExplandShaderGUI(gameObject);
				break;
		}
		ImGui::EndChild();

		ImGui::Text(u8"展示模式");
		ImGui::BeginChild("展示模式", ImVec2(400, 65), true);
		const char* displayItems[] = { DISPLAY_ITEMS };
		ImGui::Combo(u8"选择展示模式", reinterpret_cast<int*>(&gameObject.renderInfo.displayMode), displayItems, DISPLAY_NUM);
		if (gameObject.renderInfo.displayMode != DisplayMode::Static)
		{
			ImGui::InputFloat(u8"模型旋转速度", &gameObject.renderInfo.rotateSpeed);
		}
		ImGui::EndChild();

		ImGui::Text(u8"父类物体");
		ImGui::BeginChild("父类物体", ImVec2(400, 65), true);
		if(ImGui::InputInt(u8"父节点Id", &gameObject.parentId))
		{
			gameObject.SetParent();
		}
		ImGui::EndChild();

		ImGui::Text(u8"物体外边框");
		ImGui::BeginChild("外边框", ImVec2(400, 65), true);
		if (ImGui::Checkbox(u8"是否绘制对象外边框", &gameObject.renderInfo.isEnableOutLine))
		{
			SceneManagerI.SetDirty();
		}
		if (gameObject.renderInfo.isEnableOutLine)
		{
			ImGui::ColorEdit3(u8"物体外边框颜色", glm::value_ptr(gameObject.renderInfo.outLineColor));
			ImGui::SliderFloat(u8"物体外轮廓宽", &gameObject.renderInfo.outLineWidth, 1.01f, 1.1f);
		}
		ImGui::EndChild();

		ImGui::Text(u8"面剔除");
		ImGui::BeginChild("面剔除", ImVec2(400, 65), true);
		ImGui::Checkbox(u8"是否启用面剔除", &gameObject.renderInfo.isEnableCullFace);
		if (gameObject.renderInfo.isEnableCullFace)
		{
			ImGui::Checkbox(u8"是否为背面剔除", &gameObject.renderInfo.isCullBack);
		}
		ImGui::EndChild();

		ImGui::Text(u8"阴影投射");
		ImGui::BeginChild("阴影投射", ImVec2(400, 65), true);
		if (ImGui::Checkbox(u8"是否向其他物体投射阴影", &gameObject.renderInfo.isShadowCaster))
		{
			if (gameObject.renderInfo.isShadowCaster)
			{
				ShadowMapManagerI.AddShadowCaster(gameObject.id, &gameObject);
			}
			else
			{
				ShadowMapManagerI.RemoveShadowCaster(gameObject.id);
			}
		}
		ImGui::EndChild();

		ImGui::Text(u8"法线可视化");
		ImGui::BeginChild("法线可视化", ImVec2(400, 65), true);
		if (ImGui::Checkbox(u8"是否启用法线可视化", &gameObject.renderInfo.isEnableNormalVisable))
		{
			SceneManagerI.SetDirty();
		}
		if (gameObject.renderInfo.isEnableNormalVisable)
		{
			ImGui::ColorEdit3(u8"可视化法线颜色", glm::value_ptr(gameObject.renderInfo.normalColor));
			ImGui::SliderFloat(u8"可视化法线长度", &gameObject.renderInfo.normalLineDistance, 0.05f, 0.5f);
		}
		ImGui::EndChild();

		ImGui::Text(u8"对象活性");
		ImGui::BeginChild("对象活性", ImVec2(400, 65), true);
		ImGui::Checkbox(u8"是否启用对象", &gameObject.isEnable);
		ImGui::EndChild();

		if (ImGui::Button(u8"删除"))
		{
			gameObject.isDelete = true;
			SceneManagerI.SetDirty();
		}
		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::PopID();
}
void Renderer::GUIManager::DrawGameObjectDefaultShaderGUI(GameObject& gameObject)
{
	const char* diffuseMapItems[] = { DIFFUSE_MAP_ITEMS };
	if (ImGui::Combo(u8"选择使用漫反射纹理", reinterpret_cast<int*>(&gameObject.renderInfo.diffuseMapChoose), diffuseMapItems, DIFFUSE_MAP_NUM))
	{
		gameObject.ChooseDiffuseMap();
	}
	Texture* diffuseMap = gameObject.GetModeMap(TextureType::Diffuse);
	if (diffuseMap != nullptr)
	{
		ImGui::Image(diffuseMap->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
	else
	{
		ImGui::Image(ResourceManagerI.GetTexture(HUOLG_TEXTURE_PATH)->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}

	const char* specularMapItems[] = { SPECULAR_MAP_ITEMS };
	if (ImGui::Combo(u8"选择使用镜面反射纹理", reinterpret_cast<int*>(&gameObject.renderInfo.specularMapChoose), specularMapItems, SPECULAR_MAP_NUM))
	{
		gameObject.ChooseSpecularMap();
	}
	Texture* specularMap = gameObject.GetModeMap(TextureType::Specular);
	if (specularMap != nullptr)
	{
		ImGui::Image(specularMap->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
	else
	{
		ImGui::Image(ResourceManagerI.GetTexture(WHITE_TEXTURE_PATH)->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
}
void Renderer::GUIManager::DrawGameObjectSolidColorShaderGUI(GameObject& gameObject)
{
	ImGui::ColorEdit3(u8"对象纯色颜色", glm::value_ptr(gameObject.renderInfo.solidColor));
}
void Renderer::GUIManager::DrawGameObjectCutoutShaderGUI(GameObject& gameObject)
{
	const char* diffuseMapItems[] = { DIFFUSE_MAP_ITEMS };
	if (ImGui::Combo(u8"选择使用漫反射纹理", reinterpret_cast<int*>(&gameObject.renderInfo.diffuseMapChoose), diffuseMapItems, DIFFUSE_MAP_NUM))
	{
		gameObject.ChooseDiffuseMap();
	}
	Texture* diffuseMap = gameObject.GetModeMap(TextureType::Diffuse);
	if (diffuseMap != nullptr)
	{
		ImGui::Image(diffuseMap->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
	else
	{
		ImGui::Image(ResourceManagerI.GetTexture(HUOLG_TEXTURE_PATH)->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
	ImGui::SliderFloat(u8"透明切割阈值", &gameObject.renderInfo.cutoutValue, 0.0f, 1.0f);
}
void Renderer::GUIManager::DrawGameObjectTransparentShaderGUI(GameObject& gameObject)
{
	const char* diffuseMapItems[] = { DIFFUSE_MAP_ITEMS };
	if (ImGui::Combo(u8"选择使用漫反射纹理", reinterpret_cast<int*>(&gameObject.renderInfo.diffuseMapChoose), diffuseMapItems, DIFFUSE_MAP_NUM))
	{
		gameObject.ChooseDiffuseMap();
	}
	Texture* diffuseMap = gameObject.GetModeMap(TextureType::Diffuse);
	if (diffuseMap != nullptr)
	{
		ImGui::Image(diffuseMap->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
	else
	{
		ImGui::Image(ResourceManagerI.GetTexture(HUOLG_TEXTURE_PATH)->id, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
	}
}
void Renderer::GUIManager::DrawGameObjectReflectShaderGUI(GameObject& gameObject)
{
	ImGui::SliderFloat(u8"反射率", &gameObject.renderInfo.reflectivity, 0.0f, 1.0f);
}
void Renderer::GUIManager::DrawGameObjectRefractShaderGUI(GameObject& gameObject)
{
	ImGui::SliderFloat(u8"介质折射率比值", &gameObject.renderInfo.refractive, 0.0f, 1.0f);
}
void Renderer::GUIManager::DrawGameObjectExplandShaderGUI(GameObject& gameObject)
{
	ImGui::Checkbox(u8"启用随时间变化", &gameObject.renderInfo.isFloowTime);
	if (gameObject.renderInfo.isFloowTime)
	{
		ImGui::SliderFloat(u8"变化范围", &gameObject.renderInfo.explandRange, 0.0f, 3.0f);
		ImGui::SliderFloat(u8"变化速度", &gameObject.renderInfo.explandSpeed, 0.0f, 3.0f);
	}
	else
	{
		ImGui::SliderFloat(u8"扩张程度", &gameObject.renderInfo.explandDegress, 0.0f, 3.0f);
	}
}
Renderer::GUIManager& Renderer::GUIManager::Instance = instance;
Renderer::GUIManager Renderer::GUIManager::instance;