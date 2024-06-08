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
	const int numParticles = 200;
	const float size = 5.0f;
	const float mass = size;
	const vec3 maxVel = vec3(1.0f);

	engine.wallElasticity = 0.8f;
	engine.particleElasticity = 0.8f;
	engine.friction = 0.98f;
	engine.NumSteps = 10;

	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -.02f, 0));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}