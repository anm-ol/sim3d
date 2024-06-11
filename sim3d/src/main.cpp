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
	
	engine.setWall(vec3(-100,-70,-100), vec3(100,40,100));

	// particle parameters
	const int numParticles = 50;
	const float size = 15.0f;
	const float mass = size;
	const vec3 maxVel = vec3(10.0f);

	engine.wallElasticity = 0.8f;
	engine.particleElasticity = 0.95f;
	engine.friction = 0.99f;
	engine.NumSteps = 10;

	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.08f, 0));

	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}