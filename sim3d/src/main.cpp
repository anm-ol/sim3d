#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"
#include "SpacePartition.h"

using namespace glm;

int main() {

	//Setting up min and max wall boundaries
	vec3 min = vec3(-30.0f);
	vec3 max = vec3(30.0f);
	//initialising engine
	Engine engine(min,max);
	engine.setSpringHandler(15, 15, 0.5, 5);
	//init rendere
	Renderer renderer = Renderer(engine, 1920/1.5, 1080/1.5);
	renderer.camera.Position = vec3(0, 0, 50);
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
	
	light1.attenuation = .000001f;
	light2.attenuation = .002f;
	light3.attenuation = .0001f;
	
	light1.pos = vec3(10,-5,10);
	light2.pos = vec3(20,-10,-10);
	light3.pos = vec3(-15, -10, 0);
	
	light1.color = vec3(0.62, 0.12, 0.94);
	light2.color = vec3(0.1, 0.8, 0.1);
	light3.color = vec3(1, 0.1, 0.1);
	light3.intensity = 1;
	renderer.m_lights.push_back(light1);
	renderer.m_lights.push_back(light2);
	renderer.m_lights.push_back(light3);


	// particle parameters
	const int numParticles = 1000;
	const float size = 1.2f;
	const float mass = size;
	const vec3 maxVel = vec3(0.0f);
	//engine.createParticles(numParticles, size, mass, maxVel, true);

	//setting engine parameters
	engine.wallElasticity = 0.98f;
	engine.particleElasticity = 0.98f;
	engine.friction = 0.995f;
	engine.NumSteps = 1;
	engine.usePartition = true;
	engine.pause = true;

	engine.setWall(min, max);
	engine.setAccelaration(vec3(0.00, -0.0f, 0.00));
	engine.box.createGrid(min, max);

	//rendering 
	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;

}