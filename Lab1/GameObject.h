#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"

class GameObject
{
public:
	GameObject(const std::string& meshFilePath, const std::string& textureFilePath, bool isOneChannel = false);
	~GameObject();
	Mesh mesh;
	Texture texture;
	Transform transform;

private:
};

