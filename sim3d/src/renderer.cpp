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

const int VRES = 50;
const int HRES = 50;
int render(Engine& engine) {

	int screen_height = 800, screen_width = 1200;

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
	//Engine engine = Engine(500.0f, 500.0f, 500.0f);

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
	std::vector<float> vertices1;
	std::vector<unsigned int> indices;
	std::vector<vec3> normals;

	//uncomment this if you want to see something on screen
	//generateSphereMesh(vertices1, vec3(0), 1.0f, HRES, VRES);

	/*currently not working, comment this out if you want to see smth*/generateAll(engine, vertices1);

	glEnable(GL_DEPTH_TEST);
	//initialising shader.h to load, compile and link shaders
	Shader sh = Shader("shader/lightvshader.glsl", "shader/lightfshader.glsl");
	
	//create and bind buffers
	unsigned int vbo1, vao1, EBO, vbo2;
	glGenBuffers(1, &vbo1);
	glGenVertexArrays(1, &vao1);
	//glGenBuffers(1, &EBO);


	glBindVertexArray(vao1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices1.size(), &vertices1[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//display a static sphere at window center 
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao1);
		sh.use();
		//defining model,view,projection matrices
		glm::mat4 model = mat4(1.0f);
		glm::mat4 view = mat4(1.0f);
		glm::mat4 proj = mat4(1.0f);

		vec3 center = vec3(0);
		model = rotate(model, (float)glfwGetTime()*1.0f, vec3(1, 1, 0));
		model = rotate(model, sin((float)glfwGetTime()*0.7f), vec3(0, 1, 1));
		view = translate(view, glm::vec3(0, 0, -90));
		proj = glm::perspective(radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
		//set mvp matrix as uniform

		center = view * model * vec4(center,1.0);
		center = vec3(center.x, center.y, center.z);
		sh.setVec3f("center", center);
		sh.setMatrix4f("model", model);
		sh.setMatrix4f("view", view);
		sh.setMatrix4f("projection", proj);

		//glBindVertexArray(vao1);
		//glDrawElements(GL_TRIANGLES, indices.size(),GL_UNSIGNED_INT, 0 );
		glDrawArrays(GL_TRIANGLES, 0, vertices1.size());

		processInput(window);
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	return 0;
}

//generate set of 3d vertices for sphere to be rendered with triangles
//takes references of target vertex and index array as input
void generateSphereMesh(std::vector<float>& vertices, vec3 center, float size, int hres, int vres)
{
	float hstep = radians(180.f / hres);
	float vstep = radians(360.0f / vres);
	vec3 v1, v2, v3, normal;
	for (int i = 1; i <= hres; i++)
	{
		for (int j = 0; j <vres; j++) {
			float theta = j*vstep;
			float phi= i*hstep;

			//first layer
			if (i == 1)
			{
				v1 = center + vertexthetaphi(size, 0.0f, 0.0f);
				v2 = center + vertexthetaphi(size, (j)*vstep, hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); //north pole point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);

				//std::cout << v1.x << v1.y << v1.z;
				//std::cout << v2.x, v2.y, v2.z;
				//std::cout << v3.x, v3.y, v3.z;
			}

			else if (i == hres) {
				v1 = center + vertexthetaphi(size, 0.0f, i * hstep);
				v2 = center + vertexthetaphi(size, (j)*vstep, (i - 1) * hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); //south pole point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);

			}
			else
			{
				//first triangle
				v1 = center + vertexthetaphi(size, (j)*vstep, (i - 1) * hstep);
				v2 = center + vertexthetaphi(size, (j)*vstep, (i)*hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); // point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);


				//second triangle
				v1 = center + vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				v2 = center + vertexthetaphi(size, (j)*vstep, (i)*hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, (i)*hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); // point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);

			}
		}
	}
}

vec3 vertexthetaphi(float size, float theta, float phi) {
	return vec3(size * sin(phi) * cos(theta), size * sin(phi) * sin(theta), size * cos(phi));
}

std::vector<vec3> generateNormal(std::vector<unsigned int>& indices, std::vector<vec3>& vertices)
{
	std::vector<vec3> norms;
	for (int i = 0; i < vertices.size(); i += 3) {
		vec3 vertex1 = vertices[i];
		vec3 vertex2 = vertices[i + 1];
		vec3 vertex3 = vertices[i + 2];
		vec3 normal = glm::cross((vertex1 - vertex2), vertex3);
		norms.push_back(normal);
		norms.push_back(normal);
		norms.push_back(normal);
	}
	return norms;
}

void generateWallvertices(Engine& engine, std::vector<float> &vertices) 
{

}

//generate meshes for all particles and walls
void generateAll(Engine& engine, std::vector<float>& vertices)
{	
	vertices.clear();
	for (int i = 0; i < engine.particles.size(); i++)
	{
		particle p = engine.particles[i];
		generateSphereMesh(vertices, p.pos, p.size, HRES, VRES);
		generateWallvertices(engine, vertices);
	}

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}