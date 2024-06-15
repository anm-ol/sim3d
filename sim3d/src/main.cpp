#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	
	//rendering after that
	Engine engine = Engine();

	Renderer renderer = Renderer(engine);

	pointLight light1 = pointLight();
	pointLight light2 = pointLight();
	pointLight light3 = pointLight();
	light1.attenuation = .00004f;
	light2.attenuation = .004f;
	light3.attenuation = .004f;
	light1.pos = vec3(30,5,30);
	light2.pos = vec3(20,0,-40);
	light3.pos = vec3(-60, 20, 0);
	light1.color = vec3(0.8, 0.8, 0.9);
	light2.color = vec3(0.2, 1, 0.4);
	light3.color = vec3(1, 0.1, 0.1);
	light3.intensity = 1;
	renderer.m_lights.push_back(light1);
	renderer.m_lights.push_back(light2);
	renderer.m_lights.push_back(light3);

	engine.setWall(vec3(-100,-20,-100), vec3(100,40,100));

	// particle parameters
	const int numParticles = 400;
	const float size = 3.0f;
	const float mass = size;
	const vec3 maxVel = vec3(.4f);

	engine.wallElasticity = 1.0f;
	engine.particleElasticity = 1.0f;
	engine.friction = 1.0f;
	engine.NumSteps = 20;

	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.00f, 0));

	int numThreads = std::thread::hardware_concurrency();
	std::cout << "number of max threads" << numThreads << std::endl;

	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;

}