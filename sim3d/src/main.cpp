#include <iostream>
#include <glm/glm.hpp>
#include "renderer.h"
#include "Engine.h"
#include "cloth.h"

using namespace glm;

int main() {
	//call updatefunction here idk
	
	//rendering after that
	Engine engine = Engine();
<<<<<<< Updated upstream
=======

	Renderer renderer = Renderer(engine);
	renderer.ourlight.color = vec3(1);
	renderer.ourlight.intensity = 1;
	renderer.ourlight.attenuation = .0003f;

>>>>>>> Stashed changes
	engine.setWall(vec3(-100), vec3(100));

	// particle parameters
	const int numParticles = 100;
	const float size = 5.0f;
	const float mass = size;
<<<<<<< Updated upstream
	const vec3 maxVel = vec3(-1.0f);

	engine.wallElasticity = 0.6f;
	engine.particleElasticity = 0.0f;
	engine.NumSteps = 10;

	engine.createParticles(numParticles, size, mass, maxVel, false);
	engine.setAccelaration(vec3(0, -0.02f, 0));
=======
	const vec3 maxVel = vec3(1.0f);

	engine.wallElasticity = 0.0f;
	engine.particleElasticity = 0.0f;
	engine.friction = 0.0f;
	engine.NumSteps = 20;

	Cloth cloth = Cloth(15, 15, 1.0f);
	for (auto& p : cloth.vertices) {
		engine.particles.push_back(p);
	}
	//engine.createParticles(numParticles, size, mass, maxVel, false);
	engine.setAccelaration(vec3(0, 0, 0));
>>>>>>> Stashed changes

	if(render(engine))
		std::cout << "Error" << std::endl;
	return 0;
}