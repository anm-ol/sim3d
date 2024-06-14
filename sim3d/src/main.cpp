#include <iostream>
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
	light1.attenuation = .001f;
	light2.attenuation = .0001f;
	light3.attenuation = .001f;
	light1.pos = vec3(30,5,30);
	light2.pos = vec3(20,0,-40);
	light3.pos = vec3(-60, 20, 0);
	light1.color = vec3(0.2, 0.0, 0.9);
	light2.color = vec3(0.2, 1, 0.4);
	light3.color = vec3(1, 0.1, 0.1);
	light3.intensity = 1;
	renderer.m_lights.push_back(light1);
	renderer.m_lights.push_back(light2);
	renderer.m_lights.push_back(light3);

	engine.setWall(vec3(-200,-20,-200), vec3(200,40,200));

	// particle parameters
	const int numParticles = 200;
	const float size = 5.0f;
	const float mass = size;
	const vec3 maxVel = vec3(1.0f);

	engine.wallElasticity = 0.98f;
	engine.particleElasticity = 0.98f;
	engine.friction = 0.99f;
	engine.NumSteps = 15;

	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.4f, 0));

	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}