#include"../header/transform.h"
#include"../header/gameobject.h"

Renderer::Transform::Transform(GameObject& gameObject) : gameObject(gameObject)
{

}
Renderer::Transform::~Transform() = default;

glm::mat4& Renderer::Transform::Model()
{
	if (isDirty || (gameObject.renderInfo.displayMode != DisplayMode::Static))
	{
		isDirty = false;
		model = glm::translate(glm::mat4(1.0f), position) * 
			glm::eulerAngleYXZ(glm::radians(rotate.y),		//yawÆ«º½½Ç
							   glm::radians(rotate.x),		//pitch¸©Ñö½Ç
							   glm::radians(rotate.z)) *	//roll·­¹ö½Ç
			glm::scale(glm::mat4(1.0f), scale);
	}
	return model;
}