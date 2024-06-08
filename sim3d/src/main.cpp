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

	// particle parameters
	const int numParticles = 30;
	const float size = 10.0f;
	const float mass = size;
	const vec3 maxVel = vec3(1.0f);

	engine.createParticles(numParticles, size, mass, maxVel);
	engine.setAccelaration(vec3(0, -2, 0));
	engine.Elasticity = 0.8f;
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}