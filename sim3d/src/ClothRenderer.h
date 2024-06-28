#pragma once
#include "shader.h"
#include "SpringHandler.h"
#include <stb_image.h>

class ClothRenderer
{
	unsigned int VAO, VBO, textureID;
	Shader shader;

	ClothRenderer();
	void loadTexture(const char* texturefile);
	void generateTexMesh();
	void setUpdatedMesh();
};