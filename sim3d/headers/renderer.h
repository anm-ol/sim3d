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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void calcFrameRate();

class Renderer
{
public:
	Camera camera;
	Engine& engineRef;
	GLFWwindow* window;
	
	Shader particleShader;

	bool cursorVisible = false;
	bool select = false;

	std::vector<pointLight> m_lights;
	unsigned int VBO_main, VAO_main, VAO_spring, VBO_spring;
	unsigned int SPHERE_VERT_COUNT, WALL_VERT_COUNT, SPRING_VERT_COUNT;

	glm::mat4 model, view, proj;

	int VRES = 25;
	int HRES = 20;

	int screen_height, screen_width;
	float lastX, lastY;

	int render(Engine& engine);
	
	Renderer(Engine& engine, int widht, int height);
	void processInput(GLFWwindow* window);
	void setCursorVisible(bool b)
	{
		cursorVisible = b;
		camera.isMovementLocked = b;
		if (b)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void generateSphereMesh(std::vector<float>& vertices, float size, int hres, int vres);
	void generateWallvertices(Engine& engine, std::vector<float>& vertices);
	void generateGridVertices(std::vector<float>& vertices, vec3 spacing, vec3 diag1, vec3 diag2);
	void generateSprings(std::vector<float>& vertices, std::vector<spring> &springs);

	void renderSprings(std::vector<spring>& springs);
	void renderGrid(std::vector<float>& vertices, vec3 spacing, vec3 diag1, vec3 diag2);
	
	void generateAll(Engine& engine, std::vector<float>& vertices);

	void drawLights(std::vector<pointLight>& ourlights)
	{
		Shader lightShader = Shader("shader/lightsourceV.glsl", "shader/lightsourceF.glsl");
		lightShader.use();
		//defining model,view,projection matrices

		view = camera.GetViewMatrix();
		lightShader.setMatrix4f("view", view);
		lightShader.setMatrix4f("projection", proj);
		for (auto& ourlight : ourlights)
		{
			model = mat4(1);
			model = translate(model, ourlight.pos);
			model = scale(model, vec3(3) * glm::pow(ourlight.intensity, 0.33f));

			lightShader.setMatrix4f("model", model);
			lightShader.setVec3f("color", ourlight.color);

			glDrawArrays(GL_TRIANGLES, 0, SPHERE_VERT_COUNT / 6);
		}
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
		glfwSetKeyCallback(window, key_callback);

	};
};

void pushVec3(std::vector<float>& v, glm::vec3 vertex);
glm::vec3 vertexthetaphi(float size, float theta, float phi);

