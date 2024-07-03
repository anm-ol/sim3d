#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Engine.h"
#include "camera.h"
#include "GraphicObjects.h"
#include "shader.h"
#include "ClothRenderer.h"

using namespace glm;

//class ClothRenderer;
class Renderer
{
public:
	Camera camera;
	Engine& engineRef;
	GLFWwindow* window;

	Shader particleShader, lightShader, SpringShader, ClothShader;

	std::vector<float> vertices;
	std::vector<float> springVerts;
	std::unique_ptr<ClothRenderer> cloth;
	bool cursorVisible = false;
	bool useSelect = false;
	bool showparticles = false;
	bool showsprings = false;
	bool showcloth = false;
	bool showLights = true;

	std::vector<pointLight> m_lights;
	int selectedObject;
	unsigned int VBO_main, VAO_main, VAO_spring, VBO_spring;
	unsigned int SPHERE_VERT_COUNT, WALL_VERT_COUNT, SPRING_VERT_COUNT;

	glm::mat4 model, view, proj;

	int VRES = 15;
	int HRES = 15;

	int screen_height, screen_width;
	float lastX, lastY;

	enum {
		PARTICLE,
		WALL,
		POINT_LIGHT,
		SPRING
	};
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
	void generateSprings(std::vector<float>& vertices, std::vector<spring>& springs);

	void renderParticles(std::vector<float>& vertices);
	void renderWalls();
	void renderSprings(std::vector<spring>& springs);
	void renderGrid(std::vector<float>& vertices, vec3 spacing, vec3 diag1, vec3 diag2);

	void generateAll(Engine& engine, std::vector<float>& vertices);

	void drawLights(std::vector<pointLight>& ourlights)
	{
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
		// Initialize GLFW and create a basic window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Sim3D", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        // Set the Renderer pointer as the window user pointer

        // Set framebuffer size callback
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
            renderer->screen_height = height;
            renderer->screen_width = width;
            renderer->proj = perspective(radians(45.0f), (float)width / (float)height, 10.0f, 1000.0f);
            glViewport(0, 0, width, height);
			});

        // Set cursor position callback
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn) {
            static bool firstMouse = true;
            Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

            float xpos = static_cast<float>(xposIn);
            float ypos = static_cast<float>(yposIn);
            float& lastX = renderer->lastX;
            float& lastY = renderer->lastY;
            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;
            renderer->camera.ProcessMouseMovement(xoffset, yoffset);
            });

        // Set scroll callback
        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
            Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
            renderer->camera.ProcessMouseScroll(static_cast<float>(yoffset));
            });

        // Set key callback
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
                renderer->engineRef.pause = !renderer->engineRef.pause;
            if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
                renderer->setCursorVisible(!renderer->cursorVisible);
			if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
				renderer->selectedObject--;
			if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
				renderer->selectedObject++;
			if (key == GLFW_KEY_UP && action == GLFW_PRESS && renderer->engineRef.ourSpringHandler.isInit)
				renderer->selectedObject += renderer->cloth->num_x;
			if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && renderer->engineRef.ourSpringHandler.isInit)
				renderer->selectedObject -= renderer->cloth->num_x;
            });
    }
};

void pushVec3(std::vector<float>& v, glm::vec3 vertex);
glm::vec3 vertexthetaphi(float size, float theta, float phi);