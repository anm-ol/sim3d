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
	engine.particles.push_back(particle(vec3(20, 3, 10), 20.0f));
	engine.particles.push_back(particle(vec3(2, 30, -10), 10.0f));
	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}