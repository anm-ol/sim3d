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
Engine::Engine()
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	tconst = 1.0f;
	globalAcc = glm::vec3(0);
}
void Engine::setWall(glm::vec3 diag1, glm::vec3 diag2)
{
	walldiagonal1 = diag1;
	walldiagonal2 = diag2;
	xmin = walldiagonal1.x; ymin = walldiagonal1.y; zmin = walldiagonal1.z;
	xmax = walldiagonal2.x; ymax = walldiagonal2.y; zmax = walldiagonal2.z;

}

void Engine::setAccelaration(glm::vec3 acc) {
	globalAcc = acc;
}

void Engine::updateall() //this is the main function that gets called in infinite loop
{
	particles.push_back(particle(glm::vec3(1), 1.0f));


	//call particle.update() for every element in array
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].update(tconst);
		particles[i].velocity += globalAcc;
	}

	//update velocity according to force/accelaration (if any)

	//call collision handling functions after updation


}