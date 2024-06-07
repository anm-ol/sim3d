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

	// number of particles
	const unsigned int numParticles = 300;
	const float maxSize = 10;

	engine.createParticles(numParticles, maxSize);
	engine.setAccelaration(vec3(0, -0.02, 0));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}