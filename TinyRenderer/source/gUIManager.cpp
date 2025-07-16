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

	glDisable(GL_FRAMEBUFFER_SRGB);		//ȡ��Gamma����,��ֹUI����
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glEnable(GL_FRAMEBUFFER_SRGB);		//�ָ�Gamma����
}
void Renderer::GUIManager::MainMenuGUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"�����˵�"))
		{
			if (ImGui::MenuItem(u8"��"))
			{
				isShowScene = true;
			}
			if (ImGui::MenuItem(u8"�ر�"))
			{
				isShowScene = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"���ղ˵�"))
		{
			if (ImGui::MenuItem(u8"��"))
			{
				isShowLight = true;
			}
			if (ImGui::MenuItem(u8"�ر�"))
			{
				isShowLight = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"����˵�"))
		{
			if (ImGui::MenuItem(u8"��"))
			{
				isShowScreenPostProcess = true;
			}
			if (ImGui::MenuItem(u8"�ر�"))
			{
				isShowScreenPostProcess = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"��պв˵�"))
		{
			if (ImGui::MenuItem(u8"��"))
			{
				isShowSkyBox = true;
			}
			if (ImGui::MenuItem(u8"�ر�"))
			{
				isShowSkyBox = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"���Ǵ��˵�"))
		{
			if (ImGui::MenuItem(u8"��"))
			{
				isShowPlanet = true;
			}
			if (ImGui::MenuItem(u8"�ر�"))
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
	ImGui::Begin(u8"������Ϣ");

	ImGui::Text(u8"ƽ�й�");
	ImGui::BeginChild("ƽ�й�", ImVec2(450, 120), true);
	ImGui::Checkbox(u8"�Ƿ���ƽ�й�", &Renderer::LightManagerI.isOpenDirectionalLight);
	if (Renderer::LightManagerI.isOpenDirectionalLight)
	{
		DirectionalLight& directionalLight = Renderer::LightManagerI.directionalLight;
		ImGui::ColorEdit3(u8"��������ɫ", glm::value_ptr(directionalLight.diffuse));
		ImGui::ColorEdit3(u8"���淴����ɫ", glm::value_ptr(directionalLight.specular));
		if (ImGui::Checkbox(u8"����", &directionalLight.isPickUp))
		{
			if (directionalLight.isPickUp && directionalLight.isSelfMovement)
			{
				directionalLight.isSelfMovement = false;
			}
		}
		if (ImGui::Checkbox(u8"���˶�", &directionalLight.isSelfMovement))
		{
			if (directionalLight.isSelfMovement && directionalLight.isPickUp)
			{
				directionalLight.isPickUp = false;
			}
		}
		if (directionalLight.isSelfMovement)
		{
			ImGui::SliderFloat(u8"�˶��ٶ�", &directionalLight.speed, 0.0f, 3.0f);
			const char* axisItems[] = { DIRECTIONAL_MOVE_AXIS_ITEMS };
			ImGui::Combo(u8"ѡ���˶���", reinterpret_cast<int*>(&directionalLight.moveAxis), axisItems, DIRECTIONAL_MOVE_AXIS_NUM);
		}
		ImGui::Checkbox(u8"�Ƿ�����ԴͶ����Ӱ", &Renderer::ShadowMapManagerI.isEnableDirectionalShadowCaster);
	}
	ImGui::EndChild();
	ImGui::NewLine();

	ImGui::Text(u8"���Դ");
	ImGui::BeginChild("���Դ", ImVec2(450, 200), true);
	ImGui::Checkbox(u8"�Ƿ������Դ", &Renderer::LightManagerI.isOpenPointLight);
	if (Renderer::LightManagerI.isOpenPointLight)
	{
		if (ImGui::Button(u8"���ӵ��Դ����"))
		{
			Renderer::LightManagerI.AddPointNum();
		}
		if (ImGui::Button(u8"���ٵ��Դ����"))
		{
			Renderer::LightManagerI.DecreasePointNum();
		}
		ImGui::Checkbox(u8"�Ƿ�����ԴͶ����Ӱ##", &Renderer::ShadowMapManagerI.isEnablePointLightShadowCaster);
		int pointLightNum = Renderer::LightManagerI.pointLightNum;
		ImGui::Text(u8"���Դ����%d", pointLightNum);
		for (int i = 0; i < pointLightNum && i < MAX_POINT_NUM; i++)
		{
			if (ImGui::TreeNode((std::string(u8"���Դ ") + std::to_string(i + 1)).c_str()))
			{
				PointLight& pointLight = Renderer::LightManagerI.pointLights[i];
				ImGui::InputFloat3(u8"λ��", glm::value_ptr(pointLight.position));
				ImGui::ColorEdit3(u8"��������ɫ", glm::value_ptr(pointLight.diffuse));
				ImGui::ColorEdit3(u8"���淴����ɫ", glm::value_ptr(pointLight.specular));
				ImGui::SliderFloat(u8"����˥��ϵ��", &pointLight.constant, 0.5f, 1.5f);
				ImGui::SliderFloat(u8"����˥��ϵ��", &pointLight.linear, 0.01f, 0.2f);
				ImGui::SliderFloat(u8"����˥��ϵ��", &pointLight.quadratic, 0.01f, 0.05f);
				ImGui::Checkbox(u8"������Դ", &pointLight.isPickUp);
				ImGui::TreePop();
			}
		}
	}
	ImGui::EndChild();
	ImGui::NewLine();

	ImGui::Text(u8"�۹��");
	ImGui::BeginChild("�۹��", ImVec2(450, 120), true);
	ImGui::Checkbox(u8"�Ƿ����۹��", &Renderer::LightManagerI.isOpenSpotLight);
	if (Renderer::LightManagerI.isOpenSpotLight)
	{
		SpotLight& spotLight = Renderer::LightManagerI.spotLight;
		ImGui::InputFloat3(u8"λ��##", glm::value_ptr(spotLight.position));
		ImGui::ColorEdit3(u8"��������ɫ##", glm::value_ptr(spotLight.diffuse));
		ImGui::ColorEdit3(u8"���淴����ɫ##", glm::value_ptr(spotLight.specular));
		ImGui::SliderFloat(u8"���й��##", &spotLight.inCutOff, 0.01f, 15.0f);
		ImGui::SliderFloat(u8"���й��##", &spotLight.outCutOff, 0.01f, 20.0f);
		ImGui::Checkbox(u8"����##", &spotLight.isPickUp);
		ImGui::Checkbox(u8"�Ƿ�����ԴͶ����Ӱ####", &Renderer::ShadowMapManagerI.isEnableSpotLightShadowCaster);
	}
	ImGui::EndChild();
	ImGui::NewLine();

	ImGui::Text(u8"������");
	ImGui::BeginChild("������", ImVec2(450, 80), true);
	ImGui::Checkbox(u8"�Ƿ���������", &Renderer::LightManagerI.isOpenAmbient);
	if (Renderer::LightManagerI.isOpenAmbient)
	{
		ImGui::ColorEdit3(u8"��������ɫ", glm::value_ptr(Renderer::LightManagerI.ambient));
	}
	ImGui::EndChild();

	ImGui::End();
}
void Renderer::GUIManager::ScreenPostProcessGUI()
{
	ImGui::Begin(u8"������Ϣ");

	ImGui::Checkbox(u8"�Ƿ�������", &ScreenPostProcessI.isEnableScreenPostProcess);
	if (ScreenPostProcessI.isEnableScreenPostProcess)
	{
		ImGui::Text(u8"����Ч��");
		ImGui::BeginChild("����Ч��", ImVec2(400, 80), true);
		const char* screenPostProcessItems[] = { SCREEN_POST_PROCESS_ITEMS};
		ImGui::Combo(u8"ѡ��ʹ�õĺ���Ч��", reinterpret_cast<int*>(&ScreenPostProcessI.type), screenPostProcessItems, SCREEN_POST_PROCESS_NUM);
		ImGui::EndChild();
	}

	ImGui::End();
}
void Renderer::GUIManager::SkyBoxGUI()
{
	ImGui::Begin(u8"��պ���Ϣ");

	ImGui::Checkbox(u8"�Ƿ�����պ�", &SkyBoxManagerI.isEnableSkyBox);
	if (SkyBoxManagerI.isEnableSkyBox)
	{
		ImGui::Text(u8"��պ�����������");
		ImGui::BeginChild("��պ�����������", ImVec2(400, 80), true);
		const char* skyboxCubemapItems[] = { SKYBOX_CUBEMAP_ITEMS };
		if (ImGui::Combo(u8"ѡ��ʹ�õ���պ�����������", reinterpret_cast<int*>(&SkyBoxManagerI.skyBoxCubeMapType), skyboxCubemapItems, SKYBOX_CUBEMAP_NUM))
		{
			SkyBoxManagerI.ChooseSkyBoxCubeMap();
		}
		ImGui::EndChild();
	}

	ImGui::End();
}
void Renderer::GUIManager::PlanetGUI()
{
	ImGui::Begin(u8"���Ǵ���Ϣ");

	ImGui::BeginChild("�Ƿ������Ǵ�", ImVec2(250, 60), true);
	ImGui::Checkbox(u8"�Ƿ������Ǵ�", &GPUInstanceManagerI.isEnableDrawPlant);
	ImGui::EndChild();

	ImGui::End();
}
void Renderer::GUIManager::SceneGUI()
{
	const glm::vec3& position = Renderer::CameraI.Position();
	ImGui::Begin(u8"������Ϣ");

	ImGui::BeginChild("������Ϣ", ImVec2(250, 60), true);
	ImGui::Text("FPS %d", static_cast<int>(io->Framerate));
	ImGui::Text(u8"�����λ��X��%.1f Y��%.1f Z��%.1f", position.x, position.y, position.z);
	ImGui::EndChild();

	ImGui::BeginChild("����������Ϣ", ImVec2(250, 85), true);
	ImGui::Text(u8"����������Ϣ");
	ImGui::Text(u8"������������ %d", SceneManagerI.gameObjectList.size());
	if (ImGui::Button(u8"���쳡������"))
	{
		Renderer::SceneManagerI.AddGameObject();
	}
	ImGui::EndChild();

	ImGui::BeginChild("��������", ImVec2(250, 60), true);
	ImGui::Checkbox(u8"��������", &GridManagerI.enableGrid);
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
	ImGui::Begin(u8"����ȫ������");
	if (ImGui::TreeNode(("GameObject " + std::to_string(gameObject.id)).c_str()))
	{
		ImGui::Text(u8"����");
		ImGui::BeginChild("����", ImVec2(400, 100), true);
		if (ImGui::InputFloat3(u8"λ��", glm::value_ptr(gameObject.transform.Position()), "%.2f") ||
			ImGui::InputFloat3(u8"��ת", glm::value_ptr(gameObject.transform.Rotate()), "%.2f") ||
			ImGui::InputFloat3(u8"����", glm::value_ptr(gameObject.transform.Scale()), "%.2f"))
		{
			gameObject.transform.SetDirty();
		}
		ImGui::EndChild();

		ImGui::Text(u8"ģ��");
		ImGui::BeginChild("ģ��", ImVec2(400, 80), true);
		const char* modelItems[] = { MODEL_ITEMS };
		if (ImGui::Combo(u8"ѡ��ʹ��ģ��", reinterpret_cast<int*>(&gameObject.renderInfo.modelType), modelItems, MODEL_NUM))
		{
			gameObject.ChooseModel();
		}
		ImGui::EndChild();

		ImGui::Text(u8"Shader");
		ImGui::BeginChild("Shader", ImVec2(400, 150), true);
		const char* shaderItems[] = { SHADER_ITEMS };
		if (ImGui::Combo(u8"ѡ��ʹ��Shader", reinterpret_cast<int*>(&gameObject.renderInfo.shaderType), shaderItems, SHADER_NUM))
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

		ImGui::Text(u8"չʾģʽ");
		ImGui::BeginChild("չʾģʽ", ImVec2(400, 65), true);
		const char* displayItems[] = { DISPLAY_ITEMS };
		ImGui::Combo(u8"ѡ��չʾģʽ", reinterpret_cast<int*>(&gameObject.renderInfo.displayMode), displayItems, DISPLAY_NUM);
		if (gameObject.renderInfo.displayMode != DisplayMode::Static)
		{
			ImGui::InputFloat(u8"ģ����ת�ٶ�", &gameObject.renderInfo.rotateSpeed);
		}
		ImGui::EndChild();

		ImGui::Text(u8"��������");
		ImGui::BeginChild("��������", ImVec2(400, 65), true);
		if(ImGui::InputInt(u8"���ڵ�Id", &gameObject.parentId))
		{
			gameObject.SetParent();
		}
		ImGui::EndChild();

		ImGui::Text(u8"������߿�");
		ImGui::BeginChild("��߿�", ImVec2(400, 65), true);
		if (ImGui::Checkbox(u8"�Ƿ���ƶ�����߿�", &gameObject.renderInfo.isEnableOutLine))
		{
			SceneManagerI.SetDirty();
		}
		if (gameObject.renderInfo.isEnableOutLine)
		{
			ImGui::ColorEdit3(u8"������߿���ɫ", glm::value_ptr(gameObject.renderInfo.outLineColor));
			ImGui::SliderFloat(u8"������������", &gameObject.renderInfo.outLineWidth, 1.01f, 1.1f);
		}
		ImGui::EndChild();

		ImGui::Text(u8"���޳�");
		ImGui::BeginChild("���޳�", ImVec2(400, 65), true);
		ImGui::Checkbox(u8"�Ƿ��������޳�", &gameObject.renderInfo.isEnableCullFace);
		if (gameObject.renderInfo.isEnableCullFace)
		{
			ImGui::Checkbox(u8"�Ƿ�Ϊ�����޳�", &gameObject.renderInfo.isCullBack);
		}
		ImGui::EndChild();

		ImGui::Text(u8"��ӰͶ��");
		ImGui::BeginChild("��ӰͶ��", ImVec2(400, 65), true);
		if (ImGui::Checkbox(u8"�Ƿ�����������Ͷ����Ӱ", &gameObject.renderInfo.isShadowCaster))
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

		ImGui::Text(u8"���߿��ӻ�");
		ImGui::BeginChild("���߿��ӻ�", ImVec2(400, 65), true);
		if (ImGui::Checkbox(u8"�Ƿ����÷��߿��ӻ�", &gameObject.renderInfo.isEnableNormalVisable))
		{
			SceneManagerI.SetDirty();
		}
		if (gameObject.renderInfo.isEnableNormalVisable)
		{
			ImGui::ColorEdit3(u8"���ӻ�������ɫ", glm::value_ptr(gameObject.renderInfo.normalColor));
			ImGui::SliderFloat(u8"���ӻ����߳���", &gameObject.renderInfo.normalLineDistance, 0.05f, 0.5f);
		}
		ImGui::EndChild();

		ImGui::Text(u8"�������");
		ImGui::BeginChild("�������", ImVec2(400, 65), true);
		ImGui::Checkbox(u8"�Ƿ����ö���", &gameObject.isEnable);
		ImGui::EndChild();

		if (ImGui::Button(u8"ɾ��"))
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
	if (ImGui::Combo(u8"ѡ��ʹ������������", reinterpret_cast<int*>(&gameObject.renderInfo.diffuseMapChoose), diffuseMapItems, DIFFUSE_MAP_NUM))
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
	if (ImGui::Combo(u8"ѡ��ʹ�þ��淴������", reinterpret_cast<int*>(&gameObject.renderInfo.specularMapChoose), specularMapItems, SPECULAR_MAP_NUM))
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
	ImGui::ColorEdit3(u8"����ɫ��ɫ", glm::value_ptr(gameObject.renderInfo.solidColor));
}
void Renderer::GUIManager::DrawGameObjectCutoutShaderGUI(GameObject& gameObject)
{
	const char* diffuseMapItems[] = { DIFFUSE_MAP_ITEMS };
	if (ImGui::Combo(u8"ѡ��ʹ������������", reinterpret_cast<int*>(&gameObject.renderInfo.diffuseMapChoose), diffuseMapItems, DIFFUSE_MAP_NUM))
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
	ImGui::SliderFloat(u8"͸���и���ֵ", &gameObject.renderInfo.cutoutValue, 0.0f, 1.0f);
}
void Renderer::GUIManager::DrawGameObjectTransparentShaderGUI(GameObject& gameObject)
{
	const char* diffuseMapItems[] = { DIFFUSE_MAP_ITEMS };
	if (ImGui::Combo(u8"ѡ��ʹ������������", reinterpret_cast<int*>(&gameObject.renderInfo.diffuseMapChoose), diffuseMapItems, DIFFUSE_MAP_NUM))
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
	ImGui::SliderFloat(u8"������", &gameObject.renderInfo.reflectivity, 0.0f, 1.0f);
}
void Renderer::GUIManager::DrawGameObjectRefractShaderGUI(GameObject& gameObject)
{
	ImGui::SliderFloat(u8"���������ʱ�ֵ", &gameObject.renderInfo.refractive, 0.0f, 1.0f);
}
void Renderer::GUIManager::DrawGameObjectExplandShaderGUI(GameObject& gameObject)
{
	ImGui::Checkbox(u8"������ʱ��仯", &gameObject.renderInfo.isFloowTime);
	if (gameObject.renderInfo.isFloowTime)
	{
		ImGui::SliderFloat(u8"�仯��Χ", &gameObject.renderInfo.explandRange, 0.0f, 3.0f);
		ImGui::SliderFloat(u8"�仯�ٶ�", &gameObject.renderInfo.explandSpeed, 0.0f, 3.0f);
	}
	else
	{
		ImGui::SliderFloat(u8"���ų̶�", &gameObject.renderInfo.explandDegress, 0.0f, 3.0f);
	}
}
Renderer::GUIManager& Renderer::GUIManager::Instance = instance;
Renderer::GUIManager Renderer::GUIManager::instance;