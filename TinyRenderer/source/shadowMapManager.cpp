#include"../header/shadowMapManager.h"
#include"../header/window.h"

Renderer::ShadowMapManager::ShadowMapManager()
{
	memset(PLFBO, 0, sizeof(PLFBO));
	memset(PLDBO, 0, sizeof(PLFBO));
	memset(pointViews, 0, sizeof(pointViews));
	memset(pointSpaceMatrixs, 0, sizeof(pointSpaceMatrixs));
}
Renderer::ShadowMapManager::~ShadowMapManager()
{
	glDeleteFramebuffers(1, &DLFBO);
	glDeleteTextures(1, &DLDBO);
	DLFBO = 0;
	DLDBO = 0;

	glDeleteFramebuffers(1, &SLFBO);
	glDeleteTextures(1, &SLDBO);
	SLFBO = 0;
	SLDBO = 0;

	for (int i = 0; i < MAX_POINT_NUM; i++)
	{
		glDeleteFramebuffers(1, &PLFBO[i]);
		glDeleteTextures(1, &PLDBO[i]);
		PLFBO[i] = 0;
		PLDBO[i] = 0;
	}

	shadowCastGOs.clear();
}
bool Renderer::ShadowMapManager::Init()
{
	if (!LoadShader())
	{
		std::cout << "Load Shadow Caster Shader Fail!" << std::endl;
		return false;
	}
	if (!InitDLFBO() || !InitSLFBO() || !InitPLFBO())
	{
		std::cout << "Init Shadow Maping Fail!" << std::endl;
		return false;
	}
	return true;
}
bool Renderer::ShadowMapManager::InitDLFBO()
{
	//生成帧缓冲区
	glGenFramebuffers(1, &DLFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, DLFBO);

	//生成深度缓冲区
	glGenTextures(1, &DLDBO);
	glBindTexture(GL_TEXTURE_2D, DLDBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	
	//绑定深度附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DLDBO, 0);
	
	//设置当前绑定的帧缓冲区的颜色缓冲区禁止读写操作
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	//帧缓冲区检测
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Directional Light FBO Init Fail! Frame Buffer Is No Complete!" << std::endl;
		return false;
	}

	//解除绑定
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	directionalLightProjection = glm::ortho(DIRECTIONAL_LIGHT_LEFT, DIRECTIONAL_LIGHT_RIGHT, DIRECTIONAL_LIGHT_BOTTOM, DIRECTIONAL_LIGHT_TOP, DIRECTIONAL_LIGHT_NEAR, DIRECTIONAL_LIGHT_FAR);
	directionalLightView = glm::lookAt(glm::normalize(-LightManagerI.directionalLight.direction) * (DIRECTIONAL_LIGHT_FAR / 2), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	directionalLightSpaceMatrix = directionalLightProjection * directionalLightView;

	return true;
}
bool Renderer::ShadowMapManager::InitSLFBO()
{
	//生成帧缓冲区
	glGenFramebuffers(1, &SLFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, SLFBO);

	//生成深度缓冲区
	glGenTextures(1, &SLDBO);
	glBindTexture(GL_TEXTURE_2D, SLDBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	//绑定深度附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, SLDBO, 0);

	//设置颜色缓冲区禁止读写操作
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//帧缓冲区检测
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Spot Light FBO Init Fail! Frame Buffer Is No Complete!" << std::endl;
		return false;
	}

	//解除绑定
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	spotLightProjection = glm::perspective(glm::radians(SPOT_LIGHT_FOV), static_cast<float>(SHADOW_MAP_WIDTH) / SHADOW_MAP_HEIGHT, SPOT_LIGHT_NEAR, SPOT_LIGHT_FAR);
	spotLightView = glm::lookAt(LightManagerI.spotLight.position, LightManagerI.spotLight.position + glm::normalize(LightManagerI.spotLight.direction), glm::vec3(0.0f, 1.0f, 0.0f));
	spotLightSpaceMatrix = spotLightProjection * spotLightView;

	return true;
}
bool Renderer::ShadowMapManager::InitPLFBO()
{
	pointProjection = glm::perspective(glm::radians(POINT_LIGHT_FOV), static_cast<float>(SHADOW_MAP_WIDTH) / SHADOW_MAP_HEIGHT, POINT_LIGHT_NEAR, POINT_LIGHT_FAR);

	for (int i = 0; i < MAX_POINT_NUM; i++)
	{
		//生成帧缓冲区
		glGenFramebuffers(1, &PLFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, PLFBO[i]);

		//生成深度缓冲区
		glGenTextures(1, &PLDBO[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, PLDBO[i]);
		for (int face = 0; face < 6; face++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

		//绑定到帧缓冲
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, PLDBO[i], 0);

		//设置颜色缓冲区禁止读写操作
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		//帧缓冲区检测
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Point Light FBO " + std::to_string(i) + " Init Fail! Frame Buffer Is No Complete! Error:" << glGetError() << std::endl;
			return false;
		}

		UpdataPointtLightSpaceMatrix(i);
	}

	//解除绑定
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
bool Renderer::ShadowMapManager::LoadShader()
{
	directionalLightShadowCasterShader = ShaderManagerI.GetShader(ShaderType::DirectionalLightShadowCaster);
	if (!directionalLightShadowCasterShader)
	{
		std::cout << "Directional Light Shadow Caster Shader Is Empty!" << std::endl;
		return false;
	}

	spotLightShadowCasterShader = ShaderManagerI.GetShader(ShaderType::SpotLightShadowCaster);
	if (!spotLightShadowCasterShader)
	{
		std::cout << "Spot Light Shadow Caster Shader Is Empty!" << std::endl;
		return false;
	}

	pointLightShadowCasterShader = ShaderManagerI.GetShader(ShaderType::PointLightShadowCaster);
	if (!pointLightShadowCasterShader)
	{
		std::cout << "Point Light Shadow Caster Shader Is Empty!" << std::endl;
		return false;
	}

	return true;
}
void Renderer::ShadowMapManager::UpdataDirectionaLightSpaceMatrix()
{
	directionalLightView = glm::lookAt(glm::normalize(-LightManagerI.directionalLight.direction) * (DIRECTIONAL_LIGHT_FAR / 2), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	directionalLightSpaceMatrix = directionalLightProjection * directionalLightView;
}
void Renderer::ShadowMapManager::UpdataSpotLightSpaceMatrix()
{
	spotLightView = glm::lookAt(LightManagerI.spotLight.position, LightManagerI.spotLight.position + glm::normalize(LightManagerI.spotLight.direction), glm::vec3(0.0f, 1.0f, 0.0f));
	spotLightSpaceMatrix = spotLightProjection * spotLightView;
}
void Renderer::ShadowMapManager::UpdataPointtLightSpaceMatrixs()
{
	for (int i = 0; i < LightManagerI.pointLightNum && i < MAX_POINT_NUM; i++)
	{
		UpdataPointtLightSpaceMatrix(i);
	}
}
void Renderer::ShadowMapManager::UpdataPointtLightSpaceMatrix(int i)
{
	pointViews[i][POINT_LIGHT_CUBE_MAP_POSITIVE_X_INDEX] = glm::lookAt(LightManagerI.pointLights[i].position, LightManagerI.pointLights[i].position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));	//+X方向
	pointViews[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_X_INDEX] = glm::lookAt(LightManagerI.pointLights[i].position, LightManagerI.pointLights[i].position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));	//-X方向
	pointViews[i][POINT_LIGHT_CUBE_MAP_POSITIVE_Y_INDEX] = glm::lookAt(LightManagerI.pointLights[i].position, LightManagerI.pointLights[i].position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));	//+Y方向
	pointViews[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_Y_INDEX] = glm::lookAt(LightManagerI.pointLights[i].position, LightManagerI.pointLights[i].position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));	//-Y方向
	pointViews[i][POINT_LIGHT_CUBE_MAP_POSITIVE_Z_INDEX] = glm::lookAt(LightManagerI.pointLights[i].position, LightManagerI.pointLights[i].position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));	//+Z方向
	pointViews[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_Z_INDEX] = glm::lookAt(LightManagerI.pointLights[i].position, LightManagerI.pointLights[i].position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));	//-Z方向

	pointSpaceMatrixs[i][POINT_LIGHT_CUBE_MAP_POSITIVE_X_INDEX] = pointProjection * pointViews[i][POINT_LIGHT_CUBE_MAP_POSITIVE_X_INDEX];
	pointSpaceMatrixs[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_X_INDEX] = pointProjection * pointViews[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_X_INDEX];
	pointSpaceMatrixs[i][POINT_LIGHT_CUBE_MAP_POSITIVE_Y_INDEX] = pointProjection * pointViews[i][POINT_LIGHT_CUBE_MAP_POSITIVE_Y_INDEX];
	pointSpaceMatrixs[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_Y_INDEX] = pointProjection * pointViews[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_Y_INDEX];
	pointSpaceMatrixs[i][POINT_LIGHT_CUBE_MAP_POSITIVE_Z_INDEX] = pointProjection * pointViews[i][POINT_LIGHT_CUBE_MAP_POSITIVE_Z_INDEX];
	pointSpaceMatrixs[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_Z_INDEX] = pointProjection * pointViews[i][POINT_LIGHT_CUBE_MAP_NEGATIVE_Z_INDEX];
}
void Renderer::ShadowMapManager::ShadowMapping()
{
	if (shadowCastGOs.size() <= 0)
	{
		return;
	}

	glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
	glCullFace(GL_FRONT); //渲染状态,影响OpenGL上下文的全部帧缓冲区

	ShadowMappingDirectionalLight();
	ShadowMappingSpotLight();
	ShadowMappingPointLight();

	WindowI.ResetWindowViewPort();
	glCullFace(GL_BACK);
}
void Renderer::ShadowMapManager::ShadowMappingDirectionalLight()
{
	if (!directionalLightShadowCasterShader)
	{
		std::cout << "Directional Light Shadow Caster Shader Is Empty!" << std::endl;
		return;
	}
	if (!LightManagerI.isOpenDirectionalLight || !isEnableDirectionalShadowCaster)
	{
		return;
	}

	//生成平行光ShadowMap阴影映射纹理
	glBindFramebuffer(GL_FRAMEBUFFER, DLFBO);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	directionalLightShadowCasterShader->Use();
	directionalLightShadowCasterShader->SetSetMatrix4("directionalLightSpaceMatrix", directionalLightSpaceMatrix);
	for (auto& pair : shadowCastGOs)
	{
		if (!pair.second)
		{
			continue;
		}

		pair.second->DrawShadowMap(*directionalLightShadowCasterShader, ShaderType::DirectionalLightShadowCaster);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::ShadowMapManager::ShadowMappingSpotLight()
{
	if (!spotLightShadowCasterShader)
	{
		std::cout << "Spot Light Shadow Caster Shader Is Empty!" << std::endl;
		return;
	}
	if (!LightManagerI.isOpenSpotLight || !isEnableSpotLightShadowCaster)
	{
		return;
	}

	//生成聚光灯ShadowMap阴影映射纹理
	glBindFramebuffer(GL_FRAMEBUFFER, SLFBO);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	spotLightShadowCasterShader->Use();
	spotLightShadowCasterShader->SetSetMatrix4("spotLightSpaceMatrix", spotLightSpaceMatrix);
	for (auto& pair : shadowCastGOs)
	{
		if (!pair.second)
		{
			continue;
		}

		pair.second->DrawShadowMap(*spotLightShadowCasterShader, ShaderType::SpotLightShadowCaster);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::ShadowMapManager::ShadowMappingPointLight()
{
	if (!pointLightShadowCasterShader)
	{
		std::cout << "Point Light Shadow Caster Shader Is Empty!" << std::endl;
		return;
	}
	if (!LightManagerI.isOpenPointLight || !isEnablePointLightShadowCaster)
	{
		return;
	}

	//生成点光源ShadowMap阴影映射纹理
	pointLightShadowCasterShader->Use();
	pointLightShadowCasterShader->SetFloat("far", POINT_LIGHT_FAR);
	for (int i = 0; i < LightManagerI.pointLightNum && i < MAX_POINT_NUM; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, PLFBO[i]);
		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		pointLightShadowCasterShader->SetVec3("lightPos", LightManagerI.pointLights[i].position);
		pointLightShadowCasterShader->SetSetMatrix4("pointLightSpaceMatrix", POINT_LIGHT_CUBE_MAP_FACE_NUM, pointSpaceMatrixs[i]);
		for (auto& pair : shadowCastGOs)
		{
			if (!pair.second)
			{
				continue;
			}

			pair.second->DrawShadowMap(*pointLightShadowCasterShader, ShaderType::PointLightShadowCaster);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::ShadowMapManager::AddShadowCaster(int id, GameObject* gameObject)
{
	if (shadowCastGOs.count(id) > 0)
	{
		return;
	}

	shadowCastGOs.emplace(id, gameObject);
}
void Renderer::ShadowMapManager::RemoveShadowCaster(int id)
{
	if (shadowCastGOs.count(id) <= 0)
	{
		return;
	}

	shadowCastGOs.erase(id);
}

Renderer::ShadowMapManager& Renderer::ShadowMapManager::Instance = instance;
Renderer::ShadowMapManager Renderer::ShadowMapManager::instance;