#include"../header/transform.h"
#include"../header/gameObject.h"
Renderer::Transform::Transform(GameObject& gameObject)
	:position(0.0f), rotate(0.0f),scale(1.0f), model(1.0f),isDirty(true), gameObject(gameObject)
{

}
Renderer::Transform::~Transform() = default;
void Renderer::Transform::SetDirty()
{
	isDirty = true;
}
glm::vec3& Renderer::Transform::Position()
{
	return position;
}
void Renderer::Transform::Position(glm::vec3 position)
{
	isDirty = true;
	this->position = position;
}
glm::vec3& Renderer::Transform::Rotate()
{
	return rotate;
}
void Renderer::Transform::Rotate(glm::vec3 rotate)
{
	isDirty = true;
	this->rotate = rotate;
}
glm::vec3& Renderer::Transform::Scale()
{
	return scale;
}
void Renderer::Transform::Scale(glm::vec3 scale)
{
	isDirty = true;
	this->scale = scale;

}
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