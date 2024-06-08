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
	const int numParticles = 300;
	const float maxSize = 1;
	const float maxVel = 1.0f;

	engine.createParticles(numParticles, maxSize, maxVel, true);
	engine.setAccelaration(vec3(0, -0.008, 0));
	engine.Elasticity = 0.8f;
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}