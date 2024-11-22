#include "GameObject.h"



GameObject::GameObject(const std::string& meshFilePath, const std::string& textureFilePath)
{
	mesh.loadModel(meshFilePath);
	texture.init(textureFilePath);
	transform.SetPos(glm::vec3(0, 0, -20));
}

GameObject::~GameObject()
{
}
