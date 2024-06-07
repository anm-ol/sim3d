#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "particle.h"
#include "Engine.h"
#include "collision.h"

using namespace glm;

vec3 randomVec3(vec3 min, vec3 max);

Engine::Engine(float xm, float ym, float zm)
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	xmax = xm;
	ymax = ym;
	zmax = zm;
	tconst = 1.0f;
	n = 0;
}
Engine::Engine()
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	tconst = 1.0f;
	globalAcc = vec3(0);
}
void Engine::setWall(vec3 diag1, vec3 diag2)
{
	walldiagonal1 = diag1;
	walldiagonal2 = diag2;
	xmin = walldiagonal1.x; ymin = walldiagonal1.y; zmin = walldiagonal1.z;
	xmax = walldiagonal2.x; ymax = walldiagonal2.y; zmax = walldiagonal2.z;
}

// create particles randomly from numParticles and maxSize
// TODO: particles overlap with wall and each other
void Engine::createParticles(int numParticles, float size, float maxVel)
{
	for (int i = 0; i < numParticles; i++)
	{
		// walldiagonal +- maxSize is to ensure particles don't intersect the wall
		particle p = particle(randomVec3(walldiagonal1 + size, walldiagonal2 - size), size);
		p.setVelocity(randomVec3(vec3(0), vec3(maxVel)));
		particles.push_back(p);
	}
}
	
void Engine::setAccelaration(vec3 acc) {
	globalAcc = acc;
}

void Engine::updateall() //this is the main function that gets called in infinite loop
{
	//particles.push_back(particle(vec3(1), 1.0f));

	//call particle.update() for every element in array
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].update(tconst);

		//update velocity according to force/accelaration (if any)
		particles[i].velocity += globalAcc;
	}

	//call collision handling functions after updation
	wallCollide(*this);

	// inter particle collision
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = i + 1; j < particles.size(); j++)
		{
			if (isCollision(particles[i], particles[j]))
			{
				n++;
				std::cout << n << std::endl;
				resolveCollision(particles[i], particles[j]);
			}
		}
	}



}

// Function to generate a random vec3 position within a range
vec3 randomVec3(vec3 min, vec3 max)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	vec3 posn(0.0);
	for (int i = 0; i < posn.length(); i++)
	{
		// Create a uniform distribution for floats within the range [min, max]
		std::uniform_real_distribution<float> dist(min[i], max[i]);
		posn[i] = dist(gen);
	}
	return posn;
}