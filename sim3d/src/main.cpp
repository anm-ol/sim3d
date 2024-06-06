#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	// 
	//rendering after that
	Engine engine = Engine(100.0f,100.0f,100.0f);
	engine.particles.push_back(particle(vec3(5, -2, 20), 1.0f));
	engine.particles.push_back(particle(vec3(2, 3, -1), 3.0f));
	engine.particles.push_back(particle(vec3(-10, 10, -6), 2.0f));

	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}