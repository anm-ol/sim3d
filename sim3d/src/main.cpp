#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"
#include "cloth.h"

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
	const int numParticles = 100;
	const float size = 5.0f;
	const float mass = size;
	vec3 maxVel = vec3(-1.0f);

	engine.wallElasticity = 0.6f;
	engine.particleElasticity = 0.0f;
	engine.NumSteps = 10;

	engine.createParticles(numParticles, size, mass, maxVel, false);
	engine.setAccelaration(vec3(0, -0.02f, 0));
	maxVel = vec3(1.0f);

	engine.wallElasticity = 0.0f;
	engine.particleElasticity = 0.0f;
	engine.friction = 0.0f;
	engine.NumSteps = 20;

	Cloth cloth = Cloth(15, 15, 1.0f);
	for (auto& p : cloth.vertices) {
		engine.particles.push_back(p);
	}
	//engine.createParticles(numParticles, size, mass, maxVel, false);
	engine.setAccelaration(vec3(0, 0, 0));

	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}