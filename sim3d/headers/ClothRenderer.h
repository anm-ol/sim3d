#pragma once
#include "shader.h"
#include "SpringHandler.h"
#include <vector>

class SpringHandler;
class Shader;

void setVertex(std::vector<float>& v, glm::vec3& vertex, int index);

class ClothRenderer
{
public:
	unsigned int VAO, VBO, textureID;
	int num_x, num_y;
	Shader shader;
	std::vector<float> vertices;

	SpringHandler& ourhandler;
	ClothRenderer(SpringHandler &ourhandler, Shader shader);
	void render(mat4& view, mat4& projection);
	void loadTexture(const char* texturefile);
	void generateTexMesh();
	void setUpdatedMesh();
};