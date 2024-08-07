#include "renderer.h"
#include "SpringHandler.h"
#include "Engine.h"
#include "camera.h"
#include "GraphicObjects.h"
#include "shader.h"
#include "gui.h"
#include "ClothRenderer.h"
#include <filesystem>

using namespace glm;

void calcFrameRate();
bool firstMouse = true;
bool cursorVisible = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

double lastTime = glfwGetTime();
int numFrames = 0;

Renderer::Renderer(Engine& ourengine, int width, int height) : engineRef(ourengine), screen_width(width), screen_height(height)
{
	lastX = screen_width / 2.0f;
	lastY = screen_height / 2.0f;
	//Creates a GLFW window
	createWindow(screen_width, screen_height);
	glfwSetWindowUserPointer(window, this);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//loading GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	Renderer::camera = Camera(glm::vec3(0.0f, 0.0f, 100.0f));

	//initialising to load, compile and link shaders
	std::string shaderPath = std::filesystem::current_path().string() + "/shader/";
	particleShader = Shader(shaderPath + "LightingV.glsl", shaderPath + "LightingF.glsl");
	lightShader = Shader(shaderPath + "lightsourceV.glsl", shaderPath + "lightsourceF.glsl");
	SpringShader = Shader(shaderPath + "basicvshader.glsl", shaderPath + "basicfshader.glsl");
	ClothShader = Shader(shaderPath + "cloth_vert.glsl", shaderPath + "cloth_frag.glsl");

	model = mat4(1);
	view = mat4(1);
	proj = perspective(radians(45.0f), (float)screen_width / (float)screen_height, 1.0f, 1000.0f);

	SPRING_VERT_COUNT = 2 * engineRef.ourSpringHandler.springs.size();
}

int Renderer::render(Engine& engine)
{
	GUI debugGUI = GUI(engineRef, *this);

	//setup objects
	generateAll(engine, vertices);

	glEnable(GL_DEPTH_TEST);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create and bind buffers
	glGenBuffers(1, &VBO_main);
	glGenBuffers(1, &VBO_spring);
	glGenVertexArrays(1, &VAO_main);
	glGenVertexArrays(1, &VAO_spring);
	glBindVertexArray(VAO_main);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_main);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//binding spring buffers
	glBindVertexArray(VAO_spring);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_spring);
	glBufferData(GL_ARRAY_BUFFER, 3 * SPRING_VERT_COUNT * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//unbinding
	glBindVertexArray(VBO_main);

	//rendering loop
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		glBindVertexArray(VAO_main);

		//render point light sources
		if(showLights) drawLights(m_lights);

		view = camera.GetViewMatrix();

		if (showparticles)
			renderParticles(vertices);
		view = camera.GetViewMatrix();

		//render cloth
		if (cloth && showcloth && engine.ourSpringHandler.isInit)
		{
			cloth->setLights(m_lights);
			cloth->render(camera.Position, view, proj);
		}
		if(showsprings && engine.ourSpringHandler.isInit)
			renderSprings(engine.ourSpringHandler.springs);
		renderWalls();

		debugGUI.InitFrame();
		debugGUI.render();

		// display frame rate
		calcFrameRate();
		engine.updateall(deltaTime);
		glfwSwapBuffers(window);
	}
	debugGUI.shutdown();
	return 0;
}

//generate meshes for all scene objects
void Renderer::generateAll(Engine& engine, std::vector<float>& vertices)
{
	vertices.clear();

	generateSphereMesh(vertices, 1.0f, HRES, VRES);
	SPHERE_VERT_COUNT = vertices.size();
	generateWallvertices(engine, vertices);
	WALL_VERT_COUNT = vertices.size() - SPHERE_VERT_COUNT;

	generateSprings(springVerts, engineRef.ourSpringHandler.springs);
	if(engine.ourSpringHandler.isInit)
	cloth = std::make_unique<ClothRenderer>(engine.ourSpringHandler, ClothShader);
}

//generate set of 3d vertices for sphere to be rendered with triangles
//takes references of target vertex and index array as input
void Renderer::generateSphereMesh(std::vector<float>& vertices, float size, int hres, int vres)
{
	float hstep = radians(180.f / hres);
	float vstep = radians(360.0f / vres);
	vec3 v1, v2, v3, normal;
	for (int i = 1; i <= hres; i++)
	{
		for (int j = 0; j < vres; j++) {
			float theta = j * vstep;
			float phi = i * hstep;

			//first layer
			if (i == 1)
			{
				v1 = vertexthetaphi(size, 0.0f, 0.0f);
				v2 = vertexthetaphi(size, (j)*vstep, hstep);
				v3 = vertexthetaphi(size, (j + 1) * vstep, hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));


				pushVec3(vertices, v1); //north pole point
				pushVec3(vertices, v1);
				pushVec3(vertices, v2);
				pushVec3(vertices, v2);
				pushVec3(vertices, v3);
				pushVec3(vertices, v3);

				//std::cout << v1.x << v1.y << v1.z;
				//std::cout << v2.x, v2.y, v2.z;
				//std::cout << v3.x, v3.y, v3.z;
			}

			else if (i == hres) {
				v1 = vertexthetaphi(size, 0.0f, i * hstep);
				v2 = vertexthetaphi(size, (j)*vstep, (i - 1) * hstep);
				v3 = vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				pushVec3(vertices, v1); //south pole point
				pushVec3(vertices, v1);
				pushVec3(vertices, v2);
				pushVec3(vertices, v2);
				pushVec3(vertices, v3);
				pushVec3(vertices, v3);

			}
			else
			{
				//first triangle
				v1 = vertexthetaphi(size, (j)*vstep, (i - 1) * hstep);
				v2 = vertexthetaphi(size, (j)*vstep, (i)*hstep);
				v3 = vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				pushVec3(vertices, v1); // point
				pushVec3(vertices, v1);
				pushVec3(vertices, v2);
				pushVec3(vertices, v2);
				pushVec3(vertices, v3);
				pushVec3(vertices, v3);


				//second triangle
				v1 = vertexthetaphi(size, (j + 1) * vstep, (i - 1) * hstep);
				v2 = vertexthetaphi(size, (j)*vstep, (i)*hstep);
				v3 = vertexthetaphi(size, (j + 1) * vstep, (i)*hstep);
				normal = normalize(glm::cross(v2 - v1, v3 - v2));

				pushVec3(vertices, v1); // point
				pushVec3(vertices, v1);
				pushVec3(vertices, v2);
				pushVec3(vertices, v2);
				pushVec3(vertices, v3);
				pushVec3(vertices, v3);

			}
		}
	}
}

vec3 vertexthetaphi(float size, float theta, float phi) {
	return vec3(size * sin(phi) * cos(theta), size * sin(phi) * sin(theta), size * cos(phi));
}

//Adds the vertices and normal of a unit cube which will be offset and resized by the model matrix later 
void Renderer::generateWallvertices(Engine& engine, std::vector<float>& vertices)
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

//TODO: change to dynamic rendering
void Renderer::renderParticles(std::vector<float>& vertices)
{
	glBindVertexArray(VAO_main);

	particleShader.use();
	//defining model,view,projection matrices
	model = mat4(1.0f);
	view = camera.GetViewMatrix();
	particleShader.setMatrix4f("view", view);
	particleShader.setMatrix4f("projection", proj);

	particleShader.setVec3f("cameraPos", camera.Position);
	particleShader.setLights("ourlights", m_lights);

	//this is where earlier the particle was being copied instead of being used as reference
		//turning into ref improved fps from 40fps to 120fps at 100 particles and 20 timesteps
	for (auto& particlei : engineRef.particles)
	{
		float ModelColor = 0.9;
		particleShader.setFloat("light", ModelColor);
		model = mat4(1.0f);

		model = translate(model, particlei.pos);
		model = glm::scale(model, vec3(particlei.size));

		particleShader.setVec3f("objectColor", particlei.color);
		particleShader.setMatrix4f("model", model);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawArrays(GL_TRIANGLES, 0, SPHERE_VERT_COUNT/6);
	}
	glBindVertexArray(0);
}

void Renderer::renderWalls()
{
	glBindVertexArray(VAO_main);
	float ModelColor = 1.0f;
	particleShader.use();
	vec3 wallcolor(1);
	particleShader.setVec3f("objectColor", wallcolor);
	particleShader.setFloat("light", ModelColor);
	particleShader.setLights("ourlights", m_lights);

	view = mat4(1.0f);
	view = camera.GetViewMatrix();
	particleShader.setMatrix4f("view", view);
	particleShader.setMatrix4f("projection", proj);
	model = glm::mat4(1.0f);
	model = translate(model, (engineRef.walldiagonal1 + engineRef.walldiagonal2) / 2.0f);
	model = scale(model, engineRef.walldiagonal2 - engineRef.walldiagonal1);
	particleShader.setMatrix4f("model", model);

	//Draw call
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, SPHERE_VERT_COUNT / 6, WALL_VERT_COUNT / 6);
	glBindVertexArray(0);
}

//add the spring vertex data to buffer
void Renderer::generateSprings(std::vector<float>& vertices, std::vector<spring>& springs)
{
	SPRING_VERT_COUNT = springs.size() * 2;
	vertices.reserve(SPRING_VERT_COUNT);

	for (spring& s : springs)
	{
		pushVec3(vertices, s.p1.pos);
		pushVec3(vertices, s.p2.pos);
	}
}

//Batch rendering springs dynamically
void Renderer::renderSprings(std::vector<spring>& springs)
{
	std::vector<float> vertices;
	generateSprings(vertices, springs);

	// Bind the VBO and update its data
	glBindBuffer(GL_ARRAY_BUFFER, VBO_spring);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), &vertices[0]);

	// Bind the VAO
	glBindVertexArray(VAO_spring);

	// Compile and use the shader program
	SpringShader.use();

	// set uniforms
	view = camera.GetViewMatrix();
	SpringShader.setMatrix4f("view", view);
	SpringShader.setMatrix4f("projection", proj);
	SpringShader.setInt("ObjectID", SPRING);

	// Draw the line segments
	glLineWidth(3);
	glDrawArrays(GL_LINES, 0, SPRING_VERT_COUNT);

	// Unbind the VAO for good practice
	glBindVertexArray(0);
}

void pushVec3(std::vector<float>& vertices, vec3 vertex) {
	vertices.push_back(vertex.x);
	vertices.push_back(vertex.y);
	vertices.push_back(vertex.z);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Renderer::processInput(GLFWwindow* window)
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

void calcFrameRate()
{
	// measure speed
	double currentTime = glfwGetTime();
	numFrames++;

	if (currentTime - lastTime >= 1.0)
	{
		//cout << numFrames << endl;
		numFrames = 0;
		lastTime += 1.0;
	}
}
