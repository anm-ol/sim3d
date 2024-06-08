#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	
	//rendering after that
	Engine engine = Engine();
	engine.setWall(vec3(-200,-30,-200), vec3(100));

	// number of particles
	const unsigned int numParticles = 500;
	const float maxSize = 5;

	engine.createParticles(numParticles, maxSize, true);
	engine.setAccelaration(vec3(0, -0.004, 0));
	engine.Elasticity = 0.8f;
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}