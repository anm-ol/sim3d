#pragma once
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Engine.h"
#include "renderer.h"

class GUI
{
public:
	Engine& engine;
	GLFWwindow* window;

	GUI(Engine& engine, GLFWwindow* ourwindow);
	void InitFrame();
	void render();
	void shutdown();

	
};