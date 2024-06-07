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
	const int numParticles = 300;
	const float size = 5.0f;
	const float maxVel = 1.0f;

	engine.createParticles(numParticles, size, maxVel);
	engine.setAccelaration(vec3(0, -0.0002, 0));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}