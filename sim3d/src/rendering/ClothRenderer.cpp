#include "ClothRenderer.h"
#include "SpringHandler.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "shader.h"
#include "camera.h"
#include <vector>

void pushVec3(std::vector<float>& vertices, vec3 v);


//int offset = 5;
int offset = (int)sizeof(vertex)/4;

ClothRenderer::ClothRenderer(SpringHandler &handler, Shader shader) 
	:ourhandler(handler), shader(shader)
{
	num_x = ourhandler.num_x;
	num_y = ourhandler.num_y;
	//generating buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	//setting buffer data
	generateTexMesh();
	loadTexture("textures/paint.jpg");

	glBufferData(GL_ARRAY_BUFFER, tempvert.size() * sizeof(vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat), (void*)((6) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void ClothRenderer::render(vec3 &cameraPos, mat4& view, mat4& projection)
{
	glBindVertexArray(VAO);
	shader.use();
	//updating cloth vertices data
	setUpdatedMesh();

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, tempvert.size() * sizeof(vertex), &tempvert[0]);

	shader.setMatrix4f("view", view);
	shader.setMatrix4f("projection", projection);
	shader.setVec3f("cameraPos", cameraPos);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void ClothRenderer::setLights(std::vector<pointLight> lights) {
	shader.use();
	shader.setLights("ourlights", lights);
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

void ClothRenderer::setNormals()
{
	for (int ID : ourhandler.particleIDs) {

	}
}
void ClothRenderer::generateTexMesh()
{
	vertices.clear();
	vec3 vertex1, vertex2, vertex3;
	vec3 normal;
	float u1, u2, u3, v1, v2, v3;
	
	for (int y = 0; y < num_y; y++)
	{
		for (int x = 0; x < num_x; x++)
		{
			int index = y * num_x + x;
			auto ID = ourhandler.particleIDs[index];
			vertex temp;
			temp.pos = ourhandler.particlePositions[index];
			temp.uv = vec2((float)x / num_x, (float)y / num_y);
			temp.normal = vec3(0);
			tempvert.push_back(temp);
		}
	}
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

			normal = normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));

			indices.push_back(index);
			tempvert[index].normal += normal;
			indices.push_back(index + 1);
			tempvert[index + 1].normal += normal;
			indices.push_back(index + num_x);
			tempvert[index + num_x].normal += normal;

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

			normal = -normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
			
			indices.push_back(index + 1);
			tempvert[index + 1].normal += normal;
			indices.push_back(index + num_x);
			tempvert[index + num_x].normal += normal;
			indices.push_back(index + num_x + 1);
			tempvert[index + num_x + 1].normal += normal;
		}
	}
}

void ClothRenderer::setUpdatedMesh()
{
	vec3 v1, v2, v3, normal;
	int index, vertexindex = 0;
	for (int y = 0; y < num_y; y++)
	{
		for (int x = 0; x < num_x; x++)
		{
			index = y * num_x + x;
			tempvert[index].pos = ourhandler.particlePositions[index];
			tempvert[index].normal = vec3(0);
		}
	}
	for (int y = 0; y < num_y - 1; y++)
	{
		for (int x = 0; x < num_x - 1; x++)
		{
			index = y * num_x + x;
			//triangle 1
			v1 = ourhandler.particlePositions[index];
			v2 = ourhandler.particlePositions[index + 1];
			v3 = ourhandler.particlePositions[index + num_x];
			normal = glm::cross(v2 - v1, v3 - v1);
			tempvert[index].normal += normal;
			tempvert[index + 1].normal += normal;
			tempvert[index + num_x].normal += normal;

			
			//triangle 2
			v1 = ourhandler.particlePositions[index + 1];
			v2 = ourhandler.particlePositions[index + num_x];
			v3 = ourhandler.particlePositions[index + num_x + 1]; 
			normal = -glm::cross(v2 - v1, v3 - v1);
			tempvert[index + 1].normal += normal;
			tempvert[index + num_x].normal += normal;
			tempvert[index + num_x + 1].normal += normal;
		}
	}
}

void setVertex(std::vector<float>& v, glm::vec3& vertex, int index)
{
	v[index] = vertex.x;
	v[index + 1] = vertex.y;
	v[index + 2] = vertex.z;
}
