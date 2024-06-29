#include "ClothRenderer.h"
#include "SpringHandler.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "shader.h"

void pushVec3(std::vector<float>& vertices, vec3 v);

ClothRenderer::ClothRenderer(SpringHandler &handler, Shader shader) 
	:ourhandler(handler), shader(shader)
{
	num_x = ourhandler.num_x;
	num_y = ourhandler.num_y;
	//generating buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//setting buffer data
	generateTexMesh();
	loadTexture("textures/pepe.png");

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void ClothRenderer::render(glm::mat4& view, glm::mat4& projection)
{
	glBindVertexArray(VAO);
	shader.use();
	//updating cloth vertices data
	setUpdatedMesh();

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), &vertices[0]);

	shader.setMatrix4f("view", view);
	shader.setMatrix4f("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 5 / 3);

	glBindVertexArray(0);
}
void ClothRenderer::loadTexture(const char* filepath)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind texture image
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	shader.use();
	shader.setInt("texture1", 0);
}

void ClothRenderer::generateTexMesh()
{
	vertices.clear();
	vec3 vertex1, vertex2, vertex3;
	float u1, u2, u3, v1, v2, v3;
	
	for (int y = 0; y < num_y - 1; y++)
	{
		for (int x = 0; x < num_x - 1; x++)
		{
			int index = y * num_x + x;
			auto ID = ourhandler.particleIDs[index];
			//triangle 1
			vertex1 = ourhandler.particlePositions[index];
			vertex2 = ourhandler.particlePositions[index + 1];
			vertex3 = ourhandler.particlePositions[index + num_x];

			u1 = (float)x / num_x;
			v1 = (float)y / num_y;
			u2 = (float)x / num_x + 1.0/num_x;
			v2 = (float)y / num_y;
			u3 = (float)x / num_x;
			v3 = (float)y / num_y + 1.0/num_y;

			pushVec3(vertices, vertex1); vertices.push_back(u1); vertices.push_back(v1);
			pushVec3(vertices, vertex2); vertices.push_back(u2); vertices.push_back(v2);
			pushVec3(vertices, vertex3); vertices.push_back(u3); vertices.push_back(v3);

			//triangle 2
			vertex1 = ourhandler.particlePositions[index + 1];
			vertex2 = ourhandler.particlePositions[index + num_x];
			vertex3 = ourhandler.particlePositions[index + num_x + 1]; 
			u1 = (float)x / num_x + 1.0 / num_x;
			v1 = (float)y / num_y;
			u2 = (float)x / num_x;
			v2 = (float)y / num_y + 1.0 / num_y;
			u3 = (float)x / num_x + 1.0 / num_x;
			v3 = (float)y / num_y + 1.0 / num_y;

			pushVec3(vertices, vertex1); vertices.push_back(u1); vertices.push_back(v1);
			pushVec3(vertices, vertex2); vertices.push_back(u2); vertices.push_back(v2);
			pushVec3(vertices, vertex3); vertices.push_back(u3); vertices.push_back(v3);
		}
	}
}

void ClothRenderer::setUpdatedMesh()
{
	vec3 v1, v2, v3;
	int index, vertexindex = 0;
	for (int y = 0; y < num_y - 1; y++)
	{
		for (int x = 0; x < num_x - 1; x++)
		{
			index = y * num_x + x;
			//offset between consecutive vertex data, 6 is number of vertices per quad
			auto ID = ourhandler.particleIDs[index];
			//triangle 1
			v1 = ourhandler.particlePositions[index];
			v2 = ourhandler.particlePositions[index + 1];
			v3 = ourhandler.particlePositions[index + num_x];
			setVertex(vertices, v1, vertexindex);
			setVertex(vertices, v2, vertexindex + 5);
			setVertex(vertices, v3, vertexindex + 10);

			vertexindex += 15;
			//triangle 2
			v1 = ourhandler.particlePositions[index + 1];
			v2 = ourhandler.particlePositions[index + num_x];
			v3 = ourhandler.particlePositions[index + num_x + 1];

			setVertex(vertices, v1, vertexindex);
			setVertex(vertices, v2, vertexindex + 5);
			setVertex(vertices, v3, vertexindex + 10);

			vertexindex += 15;
		}
	}
}

void setVertex(std::vector<float>& v, glm::vec3& vertex, int index)
{
	v[index] = vertex.x;
	v[index + 1] = vertex.y;
	v[index + 2] = vertex.z;
}
