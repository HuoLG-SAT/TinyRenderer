#include"../header/shader.h"
#include"../header/camera.h"
#include"../header/renderer.h"
#include"../header/shadowmapmanager.h"

namespace Renderer
{
	namespace ShaderPath
	{
		constexpr const char* SHADER_DEFAULT_VERTEX_PATH = "resource/shader/vertex.glsl";
		constexpr const char* SHADER_DEFAULT_FRAGMENT_PATH = "resource/shader/fragment.glsl";
		constexpr const char* SHADER_SOLID_VERTEX_PATH = "resource/shader/vertexWithoutShadow.glsl";
		constexpr const char* SHADER_SOLID_FRAGMENT_PATH = "resource/shader/solidColorFragment.glsl";
		constexpr const char* SHADER_GRID_VERTEX_PATH = "resource/shader/gridVertex.glsl";
		constexpr const char* SHADER_GRID_FRAGMENT_PATH = "resource/shader/gridFragment.glsl";
		constexpr const char* SHADER_OUTLINE_VERTEX_PATH = "resource/shader/outlineVertex.glsl";
		constexpr const char* SHADER_OUTLINE_FRAGMENT_PATH = "resource/shader/outlinFragment.glsl";
		constexpr const char* SHADER_CUTOUT_VERTEX_PATH = "resource/shader/vertexWithoutShadow.glsl";
		constexpr const char* SHADER_CUTOUT_FRAGMENT_PATH = "resource/shader/cutout.glsl";
		constexpr const char* SHADER_TRANSPARENT_VERTEX_PATH = "resource/shader/vertexWithoutShadow.glsl";
		constexpr const char* SHADER_TRANSPARENT_FRAGMENT_PATH = "resource/shader/transparentFragment.glsl";
		constexpr const char* SHADER_SCREENPOSTPROCESS_VERTEX_PATH = "resource/shader/screenPostProcessVertex.glsl";
		constexpr const char* SHADER_SCREENPOSTPROCESS_FRAGMENT_PATH = "resource/shader/screenPostProcessFragment.glsl";
		constexpr const char* SHADER_SKYBOX_VERTEX_PATH = "resource/shader/skyboxVertex.glsl";
		constexpr const char* SHADER_SKYBOX_FRAGMENT_PATH = "resource/shader/skyboxFragment.glsl";
		constexpr const char* SHADER_REFLECT_VERTEX_PATH = "resource/shader/reflectVertex.glsl";
		constexpr const char* SHADER_REFLECT_FRAGMENT_PATH = "resource/shader/reflectFragment.glsl";
		constexpr const char* SHADER_REFRACT_VERTEX_PATH = "resource/shader/reflectVertex.glsl";
		constexpr const char* SHADER_REFRACT_FRAGMENT_PATH = "resource/shader/refractFragment.glsl";
		constexpr const char* SHADER_EXPLAND_VERTEX_PATH = "resource/shader/explodeVertex.glsl";
		constexpr const char* SHADER_EXPLAND_GEOMETRY_PATH = "resource/shader/explodeGeometry.glsl";
		constexpr const char* SHADER_EXPLAND_FRAGMENT_PATH = "resource/shader/explodeFragment.glsl";
		constexpr const char* SHADER_NORMAL_VERTEX_PATH = "resource/shader/hairVertex.glsl";
		constexpr const char* SHADER_NORMAL_GEOMETRY_PATH = "resource/shader/hairGeometry.glsl";
		constexpr const char* SHADER_NORMAL_FRAGMENT_PATH = "resource/shader/hairFragment.glsl";
		constexpr const char* SHADER_PLANET_VERTEX_PATH = "resource/shader/planetVertex.glsl";
		constexpr const char* SHADER_PLANET_FRAGMENT_PATH = "resource/shader/planetFragment.glsl";
		constexpr const char* SHADER_ROCK_VERTEX_PATH = "resource/shader/rockVertex.glsl";
		constexpr const char* SHADER_ROCK_FRAGMENT_PATH = "resource/shader/planetFragment.glsl";
		constexpr const char* SHADER_DIRECTIONAL_LIGHT_SHADOW_CASTER_VERTEX_PATH = "resource/shader/DLShadowCasterVertex.glsl";
		constexpr const char* SHADER_DIRECTIONAL_LIGHT_SHADOW_CASTER_FRAGMENT_PATH = "resource/shader/DLShadowCasterFragment.glsl";
		constexpr const char* SHADER_SPOT_LIGHT_SHADOW_CASTER_VERTEX_PATH = "resource/shader/SLShadowCasterVertex.glsl";
		constexpr const char* SHADER_SPOT_LIGHT_SHADOW_CASTER_FRAGMENT_PATH = "resource/shader/DLShadowCasterFragment.glsl";
		constexpr const char* SHADER_POINT_LIGHT_SHADOW_CASTER_VERTEX_PATH = "resource/shader/PLShadowCasterVertex.glsl";
		constexpr const char* SHADER_POINT_LIGHT_SHADOW_CASTER_GEOMETRY_PATH = "resource/shader/PLShadowCasterGeometry.glsl";
		constexpr const char* SHADER_POINT_LIGHT_SHADOW_CASTER_FRAGMENT_PATH = "resource/shader/PLShadowCasterFragment.glsl";
	}
}

Renderer::Shader::Shader(const char* vertexPath, const char* fragmentPath) :vertexPath(vertexPath), fragmentPath(fragmentPath)
{

}
Renderer::Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) : vertexPath(vertexPath), geometryPath(geometryPath), fragmentPath(fragmentPath)
{

}
Renderer::Shader::~Shader()
{
	glDeleteProgram(programId);
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	if (geometryPath)
	{
		glDeleteShader(geometryId);
	}
	programId = 0;
	vertexId = 0;
	fragmentId = 0;
	geometryId = 0;
}
bool Renderer::Shader::Compile()
{
	//存储代码
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	//读取文件流
	std::ifstream vertexFileStream;
	std::ifstream geometryFileStream;
	std::ifstream fragmentFileStream;
	//字符串流
	std::stringstream vertexStringStream;
	std::stringstream geometryStringStream;
	std::stringstream fragmentStringStream;

	//设置错误码
	vertexFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//打开文件流
		vertexFileStream.open(vertexPath);	
		fragmentFileStream.open(fragmentPath);
		//读取文件流到字符串流
		vertexStringStream << vertexFileStream.rdbuf();
		fragmentStringStream << fragmentFileStream.rdbuf();
		//关闭文件流
		vertexFileStream.close();
		fragmentFileStream.close();
		//将字符串流存储到字符串中
		vertexCode = vertexStringStream.str();
		fragmentCode = fragmentStringStream.str();

		if (geometryPath)
		{
			geometryFileStream.open(geometryPath);
			geometryStringStream << geometryFileStream.rdbuf();
			geometryFileStream.close();
			geometryCode = geometryStringStream.str();
		}
	}
	catch (const std::ifstream::failure& e)
	{
		std::cout << "Read Shader File Error: " << e.code() << " " << e.what() << std::endl;
		return false;
	}

	//将string转为const char*
	vertexShaderCode = vertexCode.c_str();
	fragmentShaderCode = fragmentCode.c_str();
	//创建Shader
	vertexId = glCreateShader(GL_VERTEX_SHADER);
	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	//向Shader中添加代码
	glShaderSource(vertexId, 1, &vertexShaderCode, NULL);
	glShaderSource(fragmentId, 1, &fragmentShaderCode, NULL);
	//编译Shader
	glCompileShader(vertexId);
	glCompileShader(fragmentId);
	//检测是否编译Shader成功
	int success = 0;
	char buffer[1024] = "";
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexId, sizeof(buffer), NULL, buffer);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
		if (geometryPath)
		{
			glDeleteShader(geometryId);
		}
		std::cout << "Vertex Compile Fail: " << buffer << std::endl;
		return false;
	}
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentId, sizeof(buffer), NULL, buffer);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
		if (geometryPath)
		{
			glDeleteShader(geometryId);
		}
		std::cout << "Fragment Compile Fail: " << buffer << std::endl;
		return false;
	}

	//创建Program渲染程序对象
	programId = glCreateProgram();
	//向Program渲染程序对象中添加Shader
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);

	if (geometryPath)
	{
		geometryShaderCode = geometryCode.c_str();
		geometryId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryId, 1, &geometryShaderCode, 0);
		glCompileShader(geometryId);
		glGetShaderiv(geometryId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometryId, sizeof(buffer), NULL, buffer);
			glDeleteShader(vertexId);
			glDeleteShader(fragmentId);
			glDeleteShader(geometryId);
			std::cout << "Geometry Compile Fail: " << buffer << std::endl;
			return false;
		}
		glAttachShader(programId, geometryId);
	}

	//链接Shader
	glLinkProgram(programId);
	//检测是否链接Shader成功
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(buffer), NULL, buffer);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
		if (geometryPath)
		{
			glDeleteShader(geometryId);
		}
		std::cout << "Shader Linked Fail: " << buffer << std::endl;
		return false;
	}
	return true;
}

bool Renderer::Shader::BindUniform(const char* uniformBlockName, unsigned int blindPointIndex)
{
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(programId, uniformBlockName);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << "Uniform Block Invalid! Uniform Block Name:" << uniformBlockName << std::endl;
		return false;
	}
	glUniformBlockBinding(programId, uniformBlockIndex, blindPointIndex);
	return true;
}
void Renderer::Shader::BindUniform(unsigned int uniformBlockIndex, unsigned int blindPointIndex)
{
	glUniformBlockBinding(programId, uniformBlockIndex, blindPointIndex);
}
unsigned int Renderer::Shader::GetUniformBlockIndex(const char* uniformBlockName)
{
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(programId, uniformBlockName);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << "Get Uniform Block Index Fail! Uniform Block Name:" << uniformBlockName << std::endl;
		return -1;
	}
	return uniformBlockIndex;
}
void Renderer::Shader::GetUniformBlockSize(unsigned int uniformBlockIndex, int* blockSize)
{
	glGetActiveUniformBlockiv(programId, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, blockSize);
}
void Renderer::Shader::GetUniformVariableIndexsInUniformBlock(int uniformVariableNum, const char* const* uniformVariableNames, unsigned int* unfiormVariableIndexs)
{
	glGetUniformIndices(programId, uniformVariableNum, uniformVariableNames, unfiormVariableIndexs);
}
void Renderer::Shader::GetUniformVariableOffsetInUniformBlock(int uniformVariableNum,const unsigned int* unfiormVariableIndexs, int* uniformVariableOffsets)
{
	glGetActiveUniformsiv(programId, uniformVariableNum, unfiormVariableIndexs, GL_UNIFORM_OFFSET, uniformVariableOffsets);
}

Renderer::ShaderManager::ShaderManager()
{
	memset(PVCUniformVariableOffset, 0, sizeof(PVCUniformVariableOffset));
	memset(DSLUniformVariableOffset, 0, sizeof(DSLUniformVariableOffset));
}
Renderer::ShaderManager::~ShaderManager()
{
	for (auto& pair : shaders)
	{
		if (pair.second)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}
	shaders.clear();

	glDeleteBuffers(1, &PVCUBO);
	PVCUBO = 0;

	glDeleteBuffers(1, &DSLUBO);
	DSLUBO = 0;
}
bool Renderer::ShaderManager::InitUniformBuffer()
{
	return InitPVCUBO() && InitDSLUBO();
}
bool Renderer::ShaderManager::InitPVCUBO()
{
	//获取默认Shader并根据默认Shader获取Uniform块的数据
	if (shaders.count(ShaderType::Default) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* defaultShader = shaders[ShaderType::Default];
	if (!defaultShader)
	{
		std::cout << "Default Shader Is Empty!" << std::endl;
		return false;
	}
	defaultShader->Use();

	//绑定PVCUBO
	//获取UniformBlockId
	unsigned int uniformBlockIndex = defaultShader->GetUniformBlockIndex(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK);
	if (uniformBlockIndex == -1)
	{
		std::cout << "Uniform Block Index Is Invalid!" << std::endl;
		return false;
	}
	//根据UniformBlockId获取块大小
	int blockSize;
	defaultShader->GetUniformBlockSize(uniformBlockIndex, &blockSize);
	//获取UniformBlock中的变量索引
	const char* uniformVariableNames[PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NUM] = { PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NAMES };
	unsigned int uniformVariableIndexs[PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NUM];
	defaultShader->GetUniformVariableIndexsInUniformBlock(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NUM, uniformVariableNames, uniformVariableIndexs);
	//获取UniformBlock中的变量的偏移量
	defaultShader->GetUniformVariableOffsetInUniformBlock(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_VARIABLE_NUM, uniformVariableIndexs, PVCUniformVariableOffset);
	//创建UBO对象
	glGenBuffers(1, &PVCUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, PVCUBO);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, PVCUniformVariableOffset[0], sizeof(glm::mat4), glm::value_ptr(CameraI.Projection));
	glBufferSubData(GL_UNIFORM_BUFFER, PVCUniformVariableOffset[1], sizeof(glm::mat4), glm::value_ptr(CameraI.View));
	glBufferSubData(GL_UNIFORM_BUFFER, PVCUniformVariableOffset[2], sizeof(glm::vec3), glm::value_ptr(CameraI.Position));
	//将UBO绑定到绑定点0
	glBindBufferBase(GL_UNIFORM_BUFFER, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT, PVCUBO);
	//将UniformBlock绑定到绑定点0
	defaultShader->BindUniform(uniformBlockIndex, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT);

	//绑定剩余Shader的PVCUBO缓冲区
	if (shaders.count(ShaderType::SolidColor) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* solidColorShader = shaders[ShaderType::SolidColor];
	if (!solidColorShader)
	{
		std::cout << "Solid Color Shader Is Empty!" << std::endl;
		return false;
	}
	solidColorShader->Use();
	if (!solidColorShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Solid Color Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Cutout) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* cutoutShader = shaders[ShaderType::Cutout];
	if (!cutoutShader)
	{
		std::cout << "Cutout Shader Is Empty!" << std::endl;
		return false;
	}
	cutoutShader->Use();
	if (!cutoutShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Cutout Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Transparent) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* transparentShader = shaders[ShaderType::Transparent];
	if (!transparentShader)
	{
		std::cout << "Transparent Shader Is Empty!" << std::endl;
		return false;
	}
	transparentShader->Use();
	if (!transparentShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Transparent Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Reflect) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* reflectShader = shaders[ShaderType::Reflect];
	if (!reflectShader)
	{
		std::cout << "Reflect Shader Is Empty!" << std::endl;
		return false;
	}
	reflectShader->Use();
	if (!reflectShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Reflect Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Refract) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* refractShader = shaders[ShaderType::Refract];
	if (!refractShader)
	{
		std::cout << "Refract Shader Is Empty!" << std::endl;
		return false;
	}
	refractShader->Use();
	if (!refractShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Refract Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::OutLine) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* outLineShader = shaders[ShaderType::OutLine];
	if (!outLineShader)
	{
		std::cout << "Outline Shader Is Empty!" << std::endl;
		return false;
	}
	outLineShader->Use();
	if (!outLineShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "OutLine Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::SkyBox) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* skyboxShader = shaders[ShaderType::SkyBox];
	if (!skyboxShader)
	{
		std::cout << "SkyBox Shader Is Empty!" << std::endl;
		return false;
	}
	skyboxShader->Use();
	if (!skyboxShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Skybox Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Grid) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* gridShader = shaders[ShaderType::Grid];
	if (!gridShader)
	{
		std::cout << "Grid Shader Is Empty!" << std::endl;
		return false;
	}
	gridShader->Use();
	if (!gridShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Grid Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Expland) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* explandShader = shaders[ShaderType::Expland];
	if (!explandShader)
	{
		std::cout << "Expland Shader Is Empty!" << std::endl;
		return false;
	}
	explandShader->Use();
	if (!explandShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Expland Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Normal) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* normalShader = shaders[ShaderType::Normal];
	if (!normalShader)
	{
		std::cout << "Normal Shader Is Empty!" << std::endl;
		return false;
	}
	normalShader->Use();
	if (!normalShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Normal Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Planet) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* planetShader = shaders[ShaderType::Planet];
	if (!planetShader)
	{
		std::cout << "Planet Shader Is Empty!" << std::endl;
		return false;
	}
	planetShader->Use();
	if (!planetShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Planet Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	if (shaders.count(ShaderType::Rock) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* rockShader = shaders[ShaderType::Rock];
	if (!rockShader)
	{
		std::cout << "Rock Shader Is Empty!" << std::endl;
		return false;
	}
	rockShader->Use();
	if (!rockShader->BindUniform(PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK, PROJECTION_VIEW_CAMERAPOSITION_UNIFORM_BLOCK_BIND_POINT))
	{
		std::cout << "Rock Shader Bind Uniform Fail!" << std::endl;
		return false;
	}

	//解除绑定
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return true;
}
bool Renderer::ShaderManager::InitDSLUBO()
{
	if (shaders.count(ShaderType::Default) <= 0)
	{
		std::cout << "Shader Pool Is Not Have Default Shader!" << std::endl;
		return false;
	}
	Shader* defaultShader = shaders[ShaderType::Default];
	if (!defaultShader)
	{
		std::cout << "Default Shader Is Empty!" << std::endl;
		return false;
	}
	defaultShader->Use();

	//绑定DSLUBO
	//获取UniformBlock索引
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(defaultShader->programId, DIRECTIONAL_SPOT_LIGHT_UNIFORM_BLOCK);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << "Uniform Block Index Is Invalid!" << std::endl;
		return false;
	}
	//根据索引获取UniformBlock信息
	int blockSize = 0;
	glGetActiveUniformBlockiv(defaultShader->programId, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	//根据名称获取UniformBlock中的Uniform变量索引
	const char* DSLUniformVariableNames[DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NUM] = { DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NAMES };
	unsigned int DSLUniformVariableIndexs[DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NUM];
	glGetUniformIndices(defaultShader->programId, DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NUM, DSLUniformVariableNames, DSLUniformVariableIndexs);
	//获取Uniform变量数据
	glGetActiveUniformsiv(defaultShader->programId, DIRECTIONAL_SPOT_LIGHT_UNIFORM_VARIABLE_NUM, DSLUniformVariableIndexs, GL_UNIFORM_OFFSET, DSLUniformVariableOffset);
	//创建UBO
	glGenBuffers(1, &DSLUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, DSLUBO);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, DSLUniformVariableOffset[0], sizeof(glm::mat4), glm::value_ptr(ShadowMapManagerI.directionalLightSpaceMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, DSLUniformVariableOffset[1], sizeof(glm::mat4), glm::value_ptr(ShadowMapManagerI.spotLightSpaceMatrix));
	//将UBO绑定到绑定点1
	glBindBufferBase(GL_UNIFORM_BUFFER, DIRECTIONAL_SPOT_LIGHT_UNIFORM_BLOCK_BIND_POINT, DSLUBO);
	//将UniformBlock绑定到绑定点0
	glUniformBlockBinding(defaultShader->programId, uniformBlockIndex, DIRECTIONAL_SPOT_LIGHT_UNIFORM_BLOCK_BIND_POINT);
	//解除绑定
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return true;
}
bool Renderer::ShaderManager::LoadShader()
{
	Shader* defaultShader = new Shader(ShaderPath::SHADER_DEFAULT_VERTEX_PATH, ShaderPath::SHADER_DEFAULT_FRAGMENT_PATH);
	if (!defaultShader->Compile())
	{
		std::cout << "Default Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* solidColorShader = new Shader(ShaderPath::SHADER_SOLID_VERTEX_PATH, ShaderPath::SHADER_SOLID_FRAGMENT_PATH);
	if (!solidColorShader->Compile())
	{
		std::cout << "Solid Color Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* gridShader = new Shader(ShaderPath::SHADER_GRID_VERTEX_PATH, ShaderPath::SHADER_GRID_FRAGMENT_PATH);
	if (!gridShader->Compile())
	{
		std::cout << "Grid Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* outLineShader = new Shader(ShaderPath::SHADER_OUTLINE_VERTEX_PATH, ShaderPath::SHADER_OUTLINE_FRAGMENT_PATH);
	if (!outLineShader->Compile())
	{
		std::cout << "OutLine Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* cutoutShader = new Shader(ShaderPath::SHADER_CUTOUT_VERTEX_PATH, ShaderPath::SHADER_CUTOUT_FRAGMENT_PATH);
	if (!cutoutShader->Compile())
	{
		std::cout << "Cutout Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* transparentShader = new Shader(ShaderPath::SHADER_TRANSPARENT_VERTEX_PATH, ShaderPath::SHADER_TRANSPARENT_FRAGMENT_PATH);
	if (!transparentShader->Compile())
	{
		std::cout << "Transparent Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* screenPostProcessShader = new Shader(ShaderPath::SHADER_SCREENPOSTPROCESS_VERTEX_PATH, ShaderPath::SHADER_SCREENPOSTPROCESS_FRAGMENT_PATH);
	if (!screenPostProcessShader->Compile())
	{
		std::cout << "Screen Post Process Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* skyBoxShader = new Shader(ShaderPath::SHADER_SKYBOX_VERTEX_PATH, ShaderPath::SHADER_SKYBOX_FRAGMENT_PATH);
	if (!skyBoxShader->Compile())
	{
		std::cout << "Sky Box Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* reflectShader = new Shader(ShaderPath::SHADER_REFLECT_VERTEX_PATH, ShaderPath::SHADER_REFLECT_FRAGMENT_PATH);
	if (!reflectShader->Compile())
	{
		std::cout << "Reflect Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* refractShader = new Shader(ShaderPath::SHADER_REFRACT_VERTEX_PATH, ShaderPath::SHADER_REFRACT_FRAGMENT_PATH);
	if (!refractShader->Compile())
	{
		std::cout << "Refract Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* explandShader = new Shader(ShaderPath::SHADER_EXPLAND_VERTEX_PATH, ShaderPath::SHADER_EXPLAND_GEOMETRY_PATH, ShaderPath::SHADER_EXPLAND_FRAGMENT_PATH);
	if (!explandShader->Compile())
	{
		std::cout << "Expland Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* normalShader = new Shader(ShaderPath::SHADER_NORMAL_VERTEX_PATH, ShaderPath::SHADER_NORMAL_GEOMETRY_PATH, ShaderPath::SHADER_NORMAL_FRAGMENT_PATH);
	if (!normalShader->Compile())
	{
		std::cout << "Normal Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* planetShader = new Shader(ShaderPath::SHADER_PLANET_VERTEX_PATH, ShaderPath::SHADER_PLANET_FRAGMENT_PATH);
	if (!planetShader->Compile())
	{
		std::cout << "Planet Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* rockShader = new Shader(ShaderPath::SHADER_ROCK_VERTEX_PATH, ShaderPath::SHADER_ROCK_FRAGMENT_PATH);
	if (!rockShader->Compile())
	{
		std::cout << "Rock Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* directionalLightShadowCasterShader = new Shader(ShaderPath::SHADER_DIRECTIONAL_LIGHT_SHADOW_CASTER_VERTEX_PATH, ShaderPath::SHADER_DIRECTIONAL_LIGHT_SHADOW_CASTER_FRAGMENT_PATH);
	if (!directionalLightShadowCasterShader->Compile())
	{
		std::cout << "Directional Light Shadow Caster Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* spotLightShadowCasterShader = new Shader(ShaderPath::SHADER_SPOT_LIGHT_SHADOW_CASTER_VERTEX_PATH, ShaderPath::SHADER_SPOT_LIGHT_SHADOW_CASTER_FRAGMENT_PATH);
	if (!spotLightShadowCasterShader->Compile())
	{
		std::cout << "Spot Light Shadow Caster Shader Compile Fail!" << std::endl;
		return false;
	}
	Shader* pointLightShadowCasterShader = new Shader(ShaderPath::SHADER_POINT_LIGHT_SHADOW_CASTER_VERTEX_PATH, ShaderPath::SHADER_POINT_LIGHT_SHADOW_CASTER_GEOMETRY_PATH, ShaderPath::SHADER_POINT_LIGHT_SHADOW_CASTER_FRAGMENT_PATH);
	if (!pointLightShadowCasterShader->Compile())
	{
		std::cout << "Point Light Shadow Caster Shader Compile Fail!" << std::endl;
		return false;
	}
	shaders.emplace(Renderer::ShaderType::Default, defaultShader);
	shaders.emplace(Renderer::ShaderType::SolidColor, solidColorShader);
	shaders.emplace(Renderer::ShaderType::Grid, gridShader);
	shaders.emplace(Renderer::ShaderType::OutLine, outLineShader);
	shaders.emplace(Renderer::ShaderType::Cutout, cutoutShader);
	shaders.emplace(Renderer::ShaderType::Transparent, transparentShader);
	shaders.emplace(Renderer::ShaderType::ScreenPostScreen, screenPostProcessShader);
	shaders.emplace(Renderer::ShaderType::SkyBox, skyBoxShader);
	shaders.emplace(Renderer::ShaderType::Reflect, reflectShader);
	shaders.emplace(Renderer::ShaderType::Refract, refractShader);
	shaders.emplace(Renderer::ShaderType::Expland, explandShader);
	shaders.emplace(Renderer::ShaderType::Normal, normalShader);
	shaders.emplace(Renderer::ShaderType::Planet, planetShader);
	shaders.emplace(Renderer::ShaderType::Rock, rockShader);
	shaders.emplace(Renderer::ShaderType::DirectionalLightShadowCaster, directionalLightShadowCasterShader);
	shaders.emplace(Renderer::ShaderType::SpotLightShadowCaster, spotLightShadowCasterShader);
	shaders.emplace(Renderer::ShaderType::PointLightShadowCaster, pointLightShadowCasterShader);
	return true;
}
void Renderer::ShaderManager::Updata()
{
	UpdataPVCUBO();
	UpdataDSLUBO();
}
void Renderer::ShaderManager::UpdataPVCUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, PVCUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, PVCUniformVariableOffset[1], sizeof(glm::mat4), glm::value_ptr(CameraI.View));
	glBufferSubData(GL_UNIFORM_BUFFER, PVCUniformVariableOffset[2], sizeof(glm::vec3), glm::value_ptr(CameraI.Position));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Renderer::ShaderManager::UpdataDSLUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, DSLUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, DSLUniformVariableOffset[0], sizeof(glm::mat4), glm::value_ptr(ShadowMapManagerI.directionalLightSpaceMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, DSLUniformVariableOffset[1], sizeof(glm::mat4), glm::value_ptr(ShadowMapManagerI.spotLightSpaceMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
Renderer::Shader* Renderer::ShaderManager::GetShader(ShaderType type)
{
	if (shaders.count(type) > 0)
	{
		return shaders[type];
	}

	return nullptr;
}
Renderer::ShaderManager& Renderer::ShaderManager::Instance = instance;
Renderer::ShaderManager Renderer::ShaderManager::instance;