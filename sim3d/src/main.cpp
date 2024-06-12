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
	renderer.ourlight.color = vec3(1);
	
	engine.setWall(vec3(-200,-20,-200), vec3(200,40,200));

	// particle parameters
	const int numParticles = 200;
	const float size = 5.0f;
	const float mass = size;
	const vec3 maxVel = vec3(3.0f);

	engine.wallElasticity = 0.9f;
	engine.particleElasticity = 0.9f;
	engine.friction = 0.99f;
	engine.NumSteps = 20;

	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.4f, 0));

	if(renderer.render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}