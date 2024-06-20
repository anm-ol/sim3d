#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"
#include "SpacePartition.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	
	//rendering after that
	vec3 min = vec3(-50, -30, -50);
	vec3 max = vec3(50,30, 50);
	Engine engine(min,max);

	Renderer renderer = Renderer(engine);

	pointLight light1 = pointLight();
	pointLight light2 = pointLight();
	pointLight light3 = pointLight();
	light1.attenuation = .0004f;
	light2.attenuation = .0004f;
	light3.attenuation = .0004f;
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


	// particle parameters
	const int numParticles = 1000;
	const float size = 2.0f;
	const float mass = size;
	const vec3 maxVel = vec3(.4f);

	engine.wallElasticity = 1.0f;
	engine.particleElasticity = 1.0f;
	engine.friction = 1.0f;
	engine.NumSteps = 20;
	engine.usePartition = true;

	engine.setWall(min, max);
	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.001f, 0));
	engine.box.createGrid(min, max);

	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;

}