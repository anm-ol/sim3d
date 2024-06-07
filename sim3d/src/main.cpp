#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	// 
	//rendering after that
	Engine engine = Engine();
	engine.setWall(vec3(-100), vec3(100));
	engine.particles.push_back(particle(vec3(5, -20, 2), 6.0f));
	engine.particles.push_back(particle(vec3(2, 3, -1), 10.0f));
	engine.particles.push_back(particle(vec3(-10, 0, -6), 2.0f));
	//engine.particles.push_back(particle(vec3(engine.walldiagonal1), 5.0f));

	engine.particles[0].setVelocity(vec3(0.2f,1.0f,-0.3f));
	engine.particles[1].setVelocity(vec3(0.2f));
	engine.particles[2].setVelocity(vec3(-0.1,0.0f,0.5f));

	engine.setAccelaration(vec3(0, -0.02, 0));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}