#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
int render();
std::vector<glm::vec3> generateSphereVertices(glm::vec3 center, float size, int resolution);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
