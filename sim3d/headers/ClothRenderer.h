#pragma once
#include "shader.h"
#include "GraphicObjects.h"
#include "SpringHandler.h"
#include <vector>

class SpringHandler;
class Shader;

void setVertex(std::vector<float>& v, glm::vec3& vertex, int index);

struct vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;

	vertex() : pos(0), normal(0), uv(0)
	{}
	vertex(vec3 pos, vec3 norm, vec2 uv) 
		: pos(pos), normal(norm), uv(uv)
	{}
	void pushVertex(std::vector<float>& vertices) {
		vertices.push_back(pos.x); vertices.push_back(pos.y); vertices.push_back(pos.z);
		vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
		vertices.push_back(uv.x); vertices.push_back(uv.y);
	}
};
class ClothRenderer
{
public:
	unsigned int VAO, VBO, textureID;
	int num_x, num_y;
	Shader shader;
	std::vector<float> vertices;

	SpringHandler& ourhandler;
	ClothRenderer(SpringHandler &ourhandler, Shader shader);
	void render(vec3 &camerapos, mat4 &view, mat4 &projection);
	void loadTexture(const char* texturefile);
	void generateTexMesh();
	void setNormals();
	void setLights(std::vector<pointLight> lights);
	void setUpdatedMesh();
};