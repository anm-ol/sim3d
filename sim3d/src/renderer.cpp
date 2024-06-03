#include "Engine.h"
#include "shader.h"
#include "renderer.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;
using namespace glm;


int render() {

	int screen_height = 480, screen_width = 720;

	//intialize glfw and make a basic window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Sim3D", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//init engine with max coordinates i.e wall location
	Engine engine = Engine(500.0f, 500.0f, 500.0f);

	//temporary cube coords
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	std::vector<vec3> vertices1;
	std::vector<unsigned int> indices;
	std::vector<vec3> normals;
	generateSphereVertices(vertices1, indices, 1.0f/2, 25, 25);

	//initialising shader.h to load, compile and link shaders
	Shader sh = Shader("shader/basicvshader.glsl", "shader/basicfshader.glsl");
	
	//create and bind buffers
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices1.size(), &vertices1[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);


	//take world coordinates (3D) and
	//  return perspective projection coordinates as seen by camera to be displayed on 2d screen

	//display a static sphere at window center 
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		sh.use();
		//defining model,view,projection matrices
		glm::mat4 model = mat4(1.0f);
		glm::mat4 view = mat4(1.0f);
		glm::mat4 proj = mat4(1.0f);
		//moves cube model in the direction 2,2,2
		//model = translate(model, vec3(0,0,2));
		//model = scale(model, vec3(3.0f));
		model = rotate(model, (float)glfwGetTime(), vec3(1, 1, 0));
		model = rotate(model, sin((float)glfwGetTime()), vec3(0, 1, 1));
		view = translate(view, glm::vec3(0, 0, -3));
		proj = glm::perspective(radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
		//set mvp matrix as uniform
		sh.setMatrix4f("model", model);
		sh.setMatrix4f("view", view);
		sh.setMatrix4f("projection", proj);

		//glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(),GL_UNSIGNED_INT, 0 );

		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	return 0;
}

//generate set of 3d vertices for sphere to be rendered with triangles
//takes references of target vertex and index array as input
void generateSphereVertices(std::vector<vec3>& vertices, std::vector<unsigned int>& indices, float size, int hres, int vres)
{
	float hstep = radians(180.f / hres);
	float vstep = radians(360.0f / vres);

	//std::vector <vec3> vertices;
	vertices.push_back(vec3(0));
	//std::vector <unsigned int> indices;
	for (int i = 1; i <= hres; i++) 
	{
		for (int j = 1; j <= vres; j++) {
			float theta = j*vstep;
			float phi= i*hstep;
			vec3 vertex = vec3(size * sin(phi) * cos(theta), size * sin(phi) * sin(theta), size * cos(phi));
			vertices.push_back(vertex);

			if (i == 0) {
				if (j <= hres) {
					indices.push_back(0);
					indices.push_back(j);
					indices.push_back((j + 1));
				}
			}
			else
			{
				indices.push_back( i * (hres - 1) + j - 1);
				indices.push_back( i * (hres - 1) + j);
				indices.push_back((i + 1) * (hres - 1) +j-1);
				indices.push_back((i + 1) * (hres - 1) + j);
				indices.push_back( i * (hres - 1) + j);
				indices.push_back((i + 1) * (hres - 1) + j - 1);
			}
		}
	}
}

std::vector<vec3> generateNormal(std::vector<unsigned int>& indices, std::vector<vec3>& vertices)
{
	std::vector<vec3> norms;
	for (int i = 0; i < indices.size(); i += 3) {
		vec3 vertex1 = vertices[i];
		vec3 vertex2 = vertices[i];
		vec3 vertex3 = vertices[i + 2];
		vec3 normal = glm::cross((vertex1 - vertex2), vertex3);
		norms.push_back(normal);
		norms.push_back(normal);
		norms.push_back(normal);
	}
	return norms;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}