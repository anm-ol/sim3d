#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Engine.h"

int render(Engine &engine);

glm::vec3 vertexthetaphi(float size, float theta, float phi);

std::vector<glm::vec3> generateNormal(std::vector<unsigned int>& indices, std::vector<glm::vec3>& vertices);
void generateSphereMesh(std::vector<float>& vertices, glm::vec3 center, float size, int hres, int vres);
void generateAll(Engine& engine, std::vector<float>& vertices);
void generateWallvertices(Engine& engine, std::vector<glm::vec3>& vertices);

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);