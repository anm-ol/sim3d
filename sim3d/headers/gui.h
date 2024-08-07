#pragma once
#include "Engine.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector>
#include <ImGuizmo.h>

#include <iostream>
#include <filesystem>

class Renderer;
struct pointLight;

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
	ImVec2 getWindowSize();
	void showLightMenu(pointLight& selectedLight);

	std::vector<std::string> textureNames;
	bool openpopup = false;
	bool randVel;
	bool rotationMode = false;

	int selectedObjectType;
	float size;
	vec3 maxvel;
	std::string selectedTexture;


	enum 
	{
		PARTICLE,
		LIGHT,
		CLOTH
	};
};