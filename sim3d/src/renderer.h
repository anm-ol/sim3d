#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
int render();
void generateSphereVertices(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, float size, int hres, int vres);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::vector<glm::vec3> generateNormal(std::vector<unsigned int>& indices, std::vector<glm::vec3>& vertices);
glm::vec3 vertexthetaphi(float size, float theta, float phi);