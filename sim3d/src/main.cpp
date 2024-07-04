#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"
#include "SpacePartition.h"

using namespace glm;

int main() {

	//Setting up min and max wall boundaries
	vec3 min = vec3(-50.0f);
	vec3 max = vec3(50.0f);
	//initialising engine
	Engine engine(min,max);
	engine.setSpringHandler(20, 20, .5, 5);
	//init renderer
	Renderer renderer = Renderer(engine, 1500, 1000);
	renderer.showcloth = true;
	renderer.showsprings = false;
	renderer.useSelect = true;
	if (!engine.ourSpringHandler.isInit) {
		renderer.showcloth = false;
		renderer.showsprings = false;
		renderer.showparticles = true;
	}
	//Setting point lights and their parameters
	pointLight light1 = pointLight();
	pointLight light2 = pointLight();
	pointLight light3 = pointLight();
	light1.attenuation = .0004f;
	light2.attenuation = .0004f;
	light3.attenuation = .0004f;
	light1.pos = vec3(30,5,30);
	light2.pos = vec3(20,0,-40);
	light3.pos = vec3(-25, 20, 0);
	light1.color = vec3(0.8, 0.8, 0.9);
	light2.color = vec3(0.2, 1, 0.4);
	light3.color = vec3(1, 0.1, 0.1);
	light3.intensity = 1;
	renderer.m_lights.push_back(light1);
	renderer.m_lights.push_back(light2);
	renderer.m_lights.push_back(light3);


	// particle parameters
	const int numParticles = 500;
	const float size = 3.0f;
	const float mass = size;
	const vec3 maxVel = vec3(0.0f);
	//engine.createParticles(numParticles, size, mass, maxVel, true);

	//setting engine parameters
	engine.wallElasticity = 0.2f;
	engine.particleElasticity = 0.2f;
	engine.friction = 0.0f;
	engine.NumSteps = 2;
	engine.usePartition = true;
	engine.pause = true;

	engine.setWall(min, max);
	engine.setAccelaration(vec3(0.00, -0.00f, 0.00));
	engine.box.createGrid(min, max);

	//rendering 
	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;

}