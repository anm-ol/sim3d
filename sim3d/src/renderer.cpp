#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int render() {

	int screen_height = 480, screen_width = 720;

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
	//take world coordinates (3D) and
	//  return perspective projection coordinates as seen by camera to be displayed on 2d screen

	//display a static sphere at window center 

	//NOTE: shader code will go in a different file and we will make a 
	// "shaderReader" class to convert code to string and compile it

	return 0;
}

//generate set of 3d vertices for sphere to be rendered with triangles
std::vector<glm::vec3> generateSphereVertices(glm::vec3 center, float size, int resolution)
{
	std::vector <glm::vec3> vertices;
	vertices.push_back( glm::vec3(0) ) ; //TODO
	return vertices;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}