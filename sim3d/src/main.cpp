#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	
	//rendering after that
	Engine engine = Engine();
	engine.setWall(vec3(-100), vec3(100));

	// particle parameters
	const int numParticles = 100;
	const float size = 5.0f;
	const float mass = size;
	const vec3 maxVel = vec3(1.0f);

	engine.wallElasticity = 0.0f;
	engine.particleElasticity = 1.0f;
	engine.createParticles(numParticles, size, mass, maxVel, true);
	engine.setAccelaration(vec3(0, -2.0f, 0));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}