#include "GameObject.h"



GameObject::GameObject(const std::string& meshFilePath, const std::string& textureFilePath, bool isOneChannel)
{
	mesh.loadModel(meshFilePath);
	texture.init(textureFilePath, isOneChannel);
	transform.SetPos(glm::vec3(0, 0, -20));
}

GameObject::~GameObject()
{
}
