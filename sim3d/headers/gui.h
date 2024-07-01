#pragma once
#include "Engine.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class Renderer;

class GUI
{
public:
	Engine& engine;
	Renderer& renderer;
	GLFWwindow* window;
	ImGuiIO* ptrio;

	float width, height;
	GUI(Engine& engine, Renderer& renderer);
	void InitFrame();
	void render();
	void shutdown();

	bool randVel;
	float size;
	vec3 maxvel;
	
};