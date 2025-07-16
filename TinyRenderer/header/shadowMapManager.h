#include"lightManager.h"
#include"gameObject.h"

namespace Renderer
{
#define SHADOW_MAP_WIDTH (2048)
#define SHADOW_MAP_HEIGHT (2048)

#define DIRECTIONAL_LIGHT_NEAR (1.0f)
#define DIRECTIONAL_LIGHT_FAR (100.0f)
#define DIRECTIONAL_LIGHT_LEFT (-10.0f)
#define DIRECTIONAL_LIGHT_RIGHT (10.0f)
#define DIRECTIONAL_LIGHT_BOTTOM (-10.0f)
#define DIRECTIONAL_LIGHT_TOP (10.0f)

#define SPOT_LIGHT_FOV (45.0f)
#define SPOT_LIGHT_NEAR (0.1f)
#define SPOT_LIGHT_FAR (100.0f)

#define POINT_LIGHT_FOV (90.0f)
#define POINT_LIGHT_NEAR (0.1f)
#define POINT_LIGHT_FAR (25.0f)
#define POINT_LIGHT_CUBE_MAP_FACE_NUM (6)
#define POINT_LIGHT_CUBE_MAP_POSITIVE_X_INDEX (0)
#define	POINT_LIGHT_CUBE_MAP_NEGATIVE_X_INDEX (1)
#define POINT_LIGHT_CUBE_MAP_POSITIVE_Y_INDEX (2)
#define	POINT_LIGHT_CUBE_MAP_NEGATIVE_Y_INDEX (3)
#define POINT_LIGHT_CUBE_MAP_POSITIVE_Z_INDEX (4)
#define	POINT_LIGHT_CUBE_MAP_NEGATIVE_Z_INDEX (5)

	class ShadowMapManager
	{
		friend class ShaderManager;
		friend class LightManager;
		friend class GUIManager;
	public:
		bool Init();
		void UpdataDirectionaLightSpaceMatrix();
		void UpdataSpotLightSpaceMatrix();
		void UpdataPointtLightSpaceMatrixs();
		void UpdataPointtLightSpaceMatrix(int index);
		void ShadowMapping();
		void AddShadowCaster(int id, GameObject* gameObject);
		void RemoveShadowCaster(int id);

		static ShadowMapManager& Instance;

	private:
		ShadowMapManager();
		~ShadowMapManager();
		ShadowMapManager(const ShadowMapManager&) = delete;
		ShadowMapManager(ShadowMapManager&&) = delete;
		ShadowMapManager& operator=(const ShadowMapManager&) = delete;
		ShadowMapManager& operator=(ShadowMapManager&&) = delete;

		bool LoadShader();

		bool InitDLFBO();
		bool InitSLFBO();
		bool InitPLFBO();

		void ShadowMappingDirectionalLight();
		void ShadowMappingSpotLight();
		void ShadowMappingPointLight();

	private:
		//平行光
		unsigned int DLFBO = 0;  //平行光帧缓冲区
		unsigned int DLDBO = 0;  //平行光深度缓冲区
		bool isEnableDirectionalShadowCaster = true;
		glm::mat4 directionalLightProjection;
		glm::mat4 directionalLightView;
		glm::mat4 directionalLightSpaceMatrix;
		Shader* directionalLightShadowCasterShader = nullptr;

		//聚光灯
		unsigned int SLFBO = 0;  //聚光灯帧缓冲区
		unsigned int SLDBO = 0;  //聚光灯深度缓冲区
		bool isEnableSpotLightShadowCaster = true;
		glm::mat4 spotLightProjection;
		glm::mat4 spotLightView;
		glm::mat4 spotLightSpaceMatrix;
		Shader* spotLightShadowCasterShader = nullptr;

		//点光源
		unsigned int PLFBO[MAX_POINT_NUM];
		unsigned int PLDBO[MAX_POINT_NUM];
		bool isEnablePointLightShadowCaster = true;
		glm::mat4 pointProjection;
		glm::mat4 pointViews[MAX_POINT_NUM][POINT_LIGHT_CUBE_MAP_FACE_NUM];
		glm::mat4 pointSpaceMatrixs[MAX_POINT_NUM][POINT_LIGHT_CUBE_MAP_FACE_NUM];
		Shader* pointLightShadowCasterShader = nullptr;

		std::unordered_map<int,GameObject*> shadowCastGOs; //投射阴影的物体
		static ShadowMapManager instance;
	};
}