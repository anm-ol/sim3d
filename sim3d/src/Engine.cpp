#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "particle.h"
#include "Engine.h"
#include "collision.h"

Engine::Engine(float xm, float ym, float zm)
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	xmax = xm;
	ymax = ym;
	zmax = zm;
	tconst = 1.0f;
}

void Engine::updateall() //this is the main function that gets called in infinite loop
{
	particles.push_back(particle(glm::vec3(1), 1.0f));
	//call particle.update() for every element in array
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].update(tconst);
	}

	//update velocity according to force/accelaration (if any)

	//call collision handling functions after updation


}