#include "Engine.h"
#include "shader.h"
#include "renderer.h"
#include "collision.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "camera.h"

using namespace std;
using namespace glm;

int screen_height = 800, screen_width = 1200;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, -30.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const int VRES = 80;
const int HRES = 60;

unsigned int SPHERE_VERT_COUNT, WALL_VERT_COUNT;

int render(Engine& engine) {


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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//temporary cube coords
	std::vector<float> vertices;
	
	generateAll(engine, vertices);

	glEnable(GL_DEPTH_TEST);
	//initialising shader.h to load, compile and link shaders
	Shader sh = Shader("shader/lightvshader.glsl", "shader/lightfshader.glsl");
	
	//create and bind buffers
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//display a static sphere at window center 
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		glBindVertexArray(VAO);
		sh.use();
		//defining model,view,projection matrices
		glm::mat4 model = mat4(1.0f);
		glm::mat4 view = mat4(1.0f);
		glm::mat4 proj = mat4(1.0f);

		vec3 center = vec3(0);
		view = camera.GetViewMatrix();
		sh.setMatrix4f("view", view);

		proj = glm::perspective(radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
		sh.setMatrix4f("projection", proj);

		center = view * vec4(center, 1.0);
		center = vec3(center.x, center.y, center.z);
		sh.setVec3f("center", center);
		
		for (int i = 0; i < engine.particles.size(); i++) 
		{
			float ModelColor = 0.9;
			sh.setFloat("light", ModelColor);
			model = mat4(1.0f);

			particle particlei = engine.particles[i];
			model = translate(model, particlei.pos);
			model = glm::scale(model, vec3(particlei.size));
			
			sh.setMatrix4f("model", model);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glDrawArrays(GL_TRIANGLES, 0, SPHERE_VERT_COUNT/6);
		}
		float ModelColor = 1.0f;
		sh.setFloat("light", ModelColor);
		model = glm::mat4(1.0f);
		model = translate(model, (engine.walldiagonal1 + engine.walldiagonal2) / 2.0f);
		model = scale(model, engine.walldiagonal2 - engine.walldiagonal1);
		//model = glm::translate(model, vec3(-engine.xmax / 2, -engine.ymax / 2, -engine.zmax / 2));
		sh.setMatrix4f("model", model);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		glDrawArrays(GL_TRIANGLES, SPHERE_VERT_COUNT/6, WALL_VERT_COUNT/6);
		//set mvp matrix as uniform

		wallCollide(engine);
		engine.updateall();
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	return 0;
}

//generate set of 3d vertices for sphere to be rendered with triangles
//takes references of target vertex and index array as input
void generateSphereMesh(std::vector<float>& vertices, vec3 center, float size, int hres, int vres)
{
	float hstep = radians(180.f / hres);
	float vstep = radians(360.0f / vres);
	vec3 v1, v2, v3, normal;
	for (int i = 1; i <= hres; i++)
	{
		for (int j = 0; j <vres; j++) {
			float theta = j*vstep;
			float phi= i*hstep;

			//first layer
			if (i == 1)
			{
				v1 = center + vertexthetaphi(size, 0.0f, 0.0f);
				v2 = center + vertexthetaphi(size, (j)*vstep, hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); //north pole point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);

				//std::cout << v1.x << v1.y << v1.z;
				//std::cout << v2.x, v2.y, v2.z;
				//std::cout << v3.x, v3.y, v3.z;
			}

			else if (i == hres) {
				v1 = center + vertexthetaphi(size, 0.0f, i * hstep);
				v2 = center + vertexthetaphi(size, (j)*vstep, (i - 1) * hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); //south pole point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);

			}
			else
			{
				//first triangle
				v1 = center + vertexthetaphi(size, (j)*vstep, (i - 1) * hstep);
				v2 = center + vertexthetaphi(size, (j)*vstep, (i)*hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); // point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);


				//second triangle
				v1 = center + vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				v2 = center + vertexthetaphi(size, (j)*vstep, (i)*hstep);
				v3 = center + vertexthetaphi(size, (j + 1) * vstep, (i)*hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z); // point
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
				vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
				vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);

			}
		}
	}
}

vec3 vertexthetaphi(float size, float theta, float phi) {
	return vec3(size * sin(phi) * cos(theta), size * sin(phi) * sin(theta), size * cos(phi));
}

//Adds the vertices and normal of a unit cube which will be offset and resized by the model matrix later 
void generateWallvertices(Engine& engine, std::vector<float>& vertices)
{
	float vert[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f
	};
	
	for (int i = 0; i < sizeof(vert) / sizeof(float); i++) 
	{
		vertices.push_back(vert[i]);
	}

}

void generateGridVertices(std::vector<float> &vertices, vec3 spacing, vec3 diag1, vec3 diag2)
{
	vec3 count = (diag1 - diag2) / spacing;
	float xcount = count.x, ycount = count.y, zcount = count.z;
	for (int i = 0; i <= xcount; i++) 
	{
		pushVertex(vertices, vec3(diag1.x+ i * spacing.x, 0, 0));
		pushVertex(vertices, vec3(0));
		pushVertex(vertices, vec3(diag1.x + i * spacing.x, diag2.y, 0));
		pushVertex(vertices, vec3(0));
	}
	for (int j = 0; j <= ycount; j++)
	{
		pushVertex(vertices, vec3(0, diag1.y + j * spacing.y, 0));
		pushVertex(vertices, vec3(0));
		pushVertex(vertices, vec3(diag2.x, diag1.y + j * spacing.y, 0));
		pushVertex(vertices, vec3(0));
	}
}

void renderGrid(std::vector<float>& vertices, vec3 spacing, vec3 diag1, vec3 diag2)
{
	mat4 model = mat4(1);
	float zcount = ((diag2 - diag1) / spacing).z;
	int size_before = vertices.size();
	generateGridVertices(vertices, spacing, diag1, diag2);
	int XY_PLANE_COUNT = vertices.size() - size_before;
	for (int i = 0; i <= zcount; i++)
	{
		model = translate(model, vec3(0, 0,spacing.z));
		//

	}
}
void pushVertex(std::vector<float>& vertices, vec3 vertex) {
	vertices.push_back(vertex.x);
	vertices.push_back(vertex.y);
	vertices.push_back(vertex.z);
}

//generate meshes for all particles and walls
void generateAll(Engine& engine, std::vector<float>& vertices)
{	
	vertices.clear();
	particle p = engine.particles[0];
	generateSphereMesh(vertices, vec3(0), 1.0f, HRES, VRES);
	SPHERE_VERT_COUNT = vertices.size();
	generateWallvertices(engine, vertices);
	WALL_VERT_COUNT = vertices.size() - SPHERE_VERT_COUNT;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}