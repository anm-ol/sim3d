#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	
	//rendering after that
	Engine engine = Engine();
	engine.setWall(vec3(-100,-30,-100), vec3(100,40,100));

	// particle parameters
	const int numParticles = 1000;
	const float size = 5.0f;
	const float mass = size;
	const vec3 maxVel = vec3(1.0f);

	engine.wallElasticity = 0.9f;
	engine.particleElasticity = 0.95f;
	engine.NumSteps = 30;

	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.00f, 0));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}