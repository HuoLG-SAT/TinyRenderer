#include"renderer.h"
#include"../header/model.h"
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
#include"../header/mathUtility.h"

namespace Renderer
{
#define MAX_ROCK_INSTANCE_NUM (1000)
#define ROCK_RADIUS (25.0f)
#define ROCK_OFFSET (2.5f)

	class GPUInstanceManager
	{
		friend class GUIManager;

	public:
		bool Init();
		void Draw();

		static GPUInstanceManager& Instance;

	private:
		GPUInstanceManager();
		~GPUInstanceManager();
		GPUInstanceManager(const GPUInstanceManager&) = delete;
		GPUInstanceManager(GPUInstanceManager&&) = delete;
		GPUInstanceManager& operator=(const GPUInstanceManager&) = delete;
		GPUInstanceManager& operator=(GPUInstanceManager&&) = delete;

	private:
		Model* plant = nullptr;
		Model* rock = nullptr;
		Shader* plantShader = nullptr;
		Shader* rockShader = nullptr;

		bool isEnableDrawPlant = false;

		unsigned int RVBO = 0; //�洢��ʯʵ����Modelģ�ͱ任��������

		static GPUInstanceManager instance;
	};

	namespace GPUInstanceUtility
	{
		inline glm::mat4 GenerateRockModel(int index)
		{
			float angle = static_cast<float>(index) / static_cast<float>(MAX_ROCK_INSTANCE_NUM) * 360.0f;
			return glm::translate(glm::mat4(1.0f), glm::vec3(cos(angle) * ROCK_RADIUS + MathUtility::Random::Range(0, 2 * ROCK_OFFSET) - ROCK_OFFSET, (MathUtility::Random::Range(0, 2 * ROCK_OFFSET) - ROCK_OFFSET) / 2.0f, sin(angle) * ROCK_RADIUS + MathUtility::Random::Range(0, 2 * ROCK_OFFSET) - ROCK_OFFSET)) * //X��Z��Ϊ��Ӧλ�� + [-offset, offset]��ƫ����,y��Ϊ [-offset, offset]��ƫ����
				glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(MathUtility::Random::Range(0, 360))), glm::vec3(static_cast<float>(MathUtility::Random::Range(0, 1) / 10.0f), static_cast<float>(MathUtility::Random::Range(0, 1) / 10.0f), static_cast<float>(MathUtility::Random::Range(0, 1) / 10.0f))) * //�����������Ϊ����ת0-360��
				glm::scale(glm::mat4(1.0f), glm::vec3(0.05f + static_cast<float>(MathUtility::Random::Range(0, 20) / 100.0f)));	//����0.05 - 0.25��
		}
	}
}