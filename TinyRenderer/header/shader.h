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

		inline unsigned int Id() const
		{
			return programId;
		}
		inline void Use() const
		{
			glUseProgram(programId);
		}

		inline void SetFloat(const char* name, float value) const
		{
			glUniform1f(glGetUniformLocation(programId, name), value);
		}
		inline void SetInt(const char* name, int value) const
		{
			glUniform1i(glGetUniformLocation(programId, name), value);
		}
		inline void SetInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
		}
		inline void SetBool(const char* name, bool value) const
		{
			glUniform1i(glGetUniformLocation(programId, name), value ? GL_TRUE : GL_FALSE);
		}
		inline void SetVec3(const char* name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(programId, name), x, y, z);
		}
		inline void SetVec3(const char* name, glm::vec3 value) const
		{
			glUniform3fv(glGetUniformLocation(programId, name), 1, glm::value_ptr(value));
		}
		inline void SetSetMatrix4(const char* name, glm::mat4 value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetSetMatrix4(const char* name, int num, glm::mat4* value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(programId, name), num, GL_FALSE, glm::value_ptr(*value));
		}

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
		unsigned int programId = 0;
		unsigned int vertexId = 0;
		unsigned int geometryId = 0;
		unsigned int fragmentId = 0;

		const char* vertexShaderCode = nullptr;
		const char* geometryShaderCode = nullptr;
		const char* fragmentShaderCode = nullptr;
		
		const char* vertexPath = nullptr;
		const char* geometryPath = nullptr;
		const char* fragmentPath = nullptr;
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
