#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_map>
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
namespace Renderer
{
#define SHADER_DEFAULT_VERTEX_PATH ("resource/shader/vertex.glsl")
#define SHADER_DEFAULT_FRAGMENT_PATH ("resource/shader/fragment.glsl")
#define SHADER_SOLID_VERTEX_PATH ("resource/shader/vertexWithoutShadow.glsl")
#define SHADER_SOLID_FRAGMENT_PATH ("resource/shader/solidColorFragment.glsl")
#define SHADER_GRID_VERTEX_PATH ("resource/shader/gridVertex.glsl")
#define SHADER_GRID_FRAGMENT_PATH ("resource/shader/gridFragment.glsl")
#define SHADER_OUTLINE_VERTEX_PATH ("resource/shader/outlineVertex.glsl")
#define SHADER_OUTLINE_FRAGMENT_PATH ("resource/shader/outlinFragment.glsl")
#define SHADER_CUTOUT_VERTEX_PATH ("resource/shader/vertexWithoutShadow.glsl")
#define SHADER_CUTOUT_FRAGMENT_PATH ("resource/shader/cutout.glsl")
#define SHADER_TRANSPARENT_VERTEX_PATH ("resource/shader/vertexWithoutShadow.glsl")
#define SHADER_TRANSPARENT_FRAGMENT_PATH ("resource/shader/transparentFragment.glsl")
#define SHADER_SCREENPOSTPROCESS_VERTEX_PATH ("resource/shader/screenPostProcessVertex.glsl")
#define SHADER_SCREENPOSTPROCESS_FRAGMENT_PATH ("resource/shader/screenPostProcessFragment.glsl")
#define SHADER_SKYBOX_VERTEX_PATH ("resource/shader/skyboxVertex.glsl")
#define SHADER_SKYBOX_FRAGMENT_PATH ("resource/shader/skyboxFragment.glsl")
#define SHADER_REFLECT_VERTEX_PATH ("resource/shader/reflectVertex.glsl")
#define SHADER_REFLECT_FRAGMENT_PATH ("resource/shader/reflectFragment.glsl")
#define SHADER_REFRACT_VERTEX_PATH ("resource/shader/reflectVertex.glsl")
#define SHADER_REFRACT_FRAGMENT_PATH ("resource/shader/refractFragment.glsl")
#define SHADER_EXPLAND_VERTEX_PATH ("resource/shader/explodeVertex.glsl")
#define SHADER_EXPLAND_GEOMETRY_PATH ("resource/shader/explodeGeometry.glsl")
#define SHADER_EXPLAND_FRAGMENT_PATH ("resource/shader/explodeFragment.glsl")
#define SHADER_NORMAL_VERTEX_PATH ("resource/shader/hairVertex.glsl")
#define SHADER_NORMAL_GEOMETRY_PATH ("resource/shader/hairGeometry.glsl")
#define SHADER_NORMAL_FRAGMENT_PATH ("resource/shader/hairFragment.glsl")
#define SHADER_PLANET_VERTEX_PATH ("resource/shader/planetVertex.glsl")
#define SHADER_PLANET_FRAGMENT_PATH ("resource/shader/planetFragment.glsl")
#define SHADER_ROCK_VERTEX_PATH ("resource/shader/rockVertex.glsl")
#define SHADER_ROCK_FRAGMENT_PATH ("resource/shader/planetFragment.glsl")
#define SHADER_DIRECTIONAL_LIGHT_SHADOW_CASTER_VERTEX_PATH ("resource/shader/DLShadowCasterVertex.glsl")
#define SHADER_DIRECTIONAL_LIGHT_SHADOW_CASTER_FRAGMENT_PATH ("resource/shader/DLShadowCasterFragment.glsl")
#define SHADER_SPOT_LIGHT_SHADOW_CASTER_VERTEX_PATH ("resource/shader/SLShadowCasterVertex.glsl")
#define SHADER_SPOT_LIGHT_SHADOW_CASTER_FRAGMENT_PATH ("resource/shader/DLShadowCasterFragment.glsl")
#define SHADER_POINT_LIGHT_SHADOW_CASTER_VERTEX_PATH ("resource/shader/PLShadowCasterVertex.glsl")
#define SHADER_POINT_LIGHT_SHADOW_CASTER_GEOMETRY_PATH ("resource/shader/PLShadowCasterGeometry.glsl")
#define SHADER_POINT_LIGHT_SHADOW_CASTER_FRAGMENT_PATH ("resource/shader/PLShadowCasterFragment.glsl")

#define PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK ("PVCUBO")
#define PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NAMES "projection","view","cameraPos"
#define PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NUM (3)
#define PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT (0)

#define DIRECTIONAL_SPOT_LIGHT_UNIFORM_BLOCK ("DSLUBO")
#define DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NAMES "directionalLightSpaceMatrix","spotLightSpaceMatrix"
#define DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NUM (2)
#define DIRECTIONAL_SPOT_LIGHT_UNIFORM_BLOCK_BIND_POINT (1)

	enum class ShaderType
	{
		//物体Shader
		Default,
		SolidColor,
		Cutout,
		Transparent,
		Reflect,
		Refract,
		Expland,

		//物体多Pass通道Shader
		OutLine,
		Normal,

		//GPU实例化Shader
		Planet,
		Rock,

		//网格Shader
		Grid,

		//屏幕后处理Shader
		ScreenPostScreen,

		//天空盒Shader
		SkyBox,

		//阴影投射Shader
		DirectionalLightShadowCaster,
		SpotLightShadowCaster,
		PointLightShadowCaster
	};

	class Shader
	{
		friend class ShaderManager;
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
		~Shader();

		bool Compile();
		unsigned int Id() const;

		void Use();

		void SetFloat(const char* name, float value);
		void SetInt(const char* name, int value);
		void SetInt(const std::string& name, int value);
		void SetBool(const char* name, bool value);
		void SetVec3(const char* name, float x, float y, float z);
		void SetVec3(const char* name, glm::vec3 value);
		void SetSetMatrix4(const char* name, glm::mat4 value);
		void SetSetMatrix4(const char* name, int num, glm::mat4* value);

		bool BindUniform(const char* uniformBlockName,unsigned int blindPointIndex);
		void BindUniform(unsigned int unfiormBlockIndex, unsigned int blindPointIndex);
		unsigned int GetUniformBlockIndex(const char* uniformBlockName);
		void GetUniformBlockSize(unsigned int uniformBlockIndex, int* blockSize);
		void GetUniformVariableIndexsInUniformBlock(int uniformVariableNum, const char* const* uniformVariableNames, unsigned int* unfiormVariableIndexs);
		void GetUniformVariableOffsetInUniformBlock(int uniformVariableNum, const unsigned int* unfiormVariableIndexs, int* uniformVariableOffsets);

	private:
		Shader() = delete;
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;


	private:
		unsigned int programId;
		unsigned int vertexId;
		unsigned int geometryId;
		unsigned int fragmentId;

		const char* vertexShaderCode;
		const char* geometryShaderCode;
		const char* fragmentShaderCode;
		
		const char* vertexPath;
		const char* geometryPath;
		const char* fragmentPath;
	};

	class ShaderManager
	{
	public:
		bool InitUniformBuffer();
		bool LoadShader();
		void Updata();
		void UpdataPVCUBO();
		void UpdataDSLUBO();
		Shader* GetShader(ShaderType type);
		static ShaderManager& Instance;

	private:
		ShaderManager();
		~ShaderManager();
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager(ShaderManager&&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
		ShaderManager& operator=(ShaderManager&&) = delete;

		bool InitPVCUBO();
		bool InitDSLUBO();

	private:
		std::unordered_map<ShaderType, Shader*> shaders;

		unsigned int PVCUBO = 0;
		int PVCUniformVariableOffset[PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NUM];

		unsigned int DSLUBO = 0;
		int DSLUniformVariableOffset[DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NUM];

		static ShaderManager instance;
	};
}
