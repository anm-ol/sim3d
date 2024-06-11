#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "Engine.h"
#include "camera.h"
#include "GraphicObjects.h"
#include "shader.h"

using namespace glm;


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void calcFrameRate();

class Renderer
{
public:
	Camera camera;
	Engine& engineRef;
	GLFWwindow* window;
	
	pointLight ourlight;
	unsigned int SPHERE_VERT_COUNT, WALL_VERT_COUNT;

	int screen_height, screen_width;
	float lastX, lastY;


	int render(Engine& engine);


	Renderer(Engine& ourengine) : engineRef(ourengine)
	{
		screen_height = 800;
		screen_width = 1200;
		lastX = screen_width / 2.0f;
		lastY = screen_height / 2.0f;
		//Creates a GLFW window
		createWindow(screen_width, screen_height);
		glfwSetWindowUserPointer(window, this);

		Renderer::camera = Camera(glm::vec3(0.0f, 0.0f, -30.0f));
		ourlight = pointLight(); 
	}
	
	
	void processInput(GLFWwindow* window);

	void generateSphereMesh(std::vector<float>& vertices, float size, int hres, int vres);
	void generateWallvertices(Engine& engine, std::vector<float>& vertices);
	void generateGridVertices(std::vector<float>& vertices, vec3 spacing, vec3 diag1, vec3 diag2);
	
	void renderGrid(std::vector<float>& vertices, vec3 spacing, vec3 diag1, vec3 diag2);
	
	void generateAll(Engine& engine, std::vector<float>& vertices);

	void drawLight()
	{
		Shader lightShader = Shader("shader/lightsourceV.glsl", "shader/lightsourceF.glsl");
		lightShader.use();
		//defining model,view,projection matrices
		glm::mat4 model = mat4(1.0f);

		mat4 view = camera.GetViewMatrix();
		mat4 proj = perspective(radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
		lightShader.setMatrix4f("view", view);
		lightShader.setMatrix4f("projection", proj);
		model = translate(model, ourlight.pos);

		lightShader.setMatrix4f("model", model);
		lightShader.setVec3f("color", ourlight.color);

		glDrawArrays(GL_TRIANGLES, 0, SPHERE_VERT_COUNT / 6);
	}
	void createWindow(int WIDTH, int HEIGHT)
	{
		//intialize glfw and make a basic window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Sim3D", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

	};
};

void pushVec3(std::vector<float>& v, glm::vec3 vertex);
glm::vec3 vertexthetaphi(float size, float theta, float phi);

