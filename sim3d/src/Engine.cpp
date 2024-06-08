#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "particle.h"
#include "Engine.h"
#include "collision.h"

using namespace glm;

vec3 randomPos(vec3 min, vec3 max);
float randomSize(float maxSize);

Engine::Engine(float xm, float ym, float zm)
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	xmax = xm;
	ymax = ym;
	zmax = zm;
	tconst = 1.0f;
	Elasticity = 1.0f;
}
Engine::Engine()
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	tconst = 1.0f;
	Elasticity = 1.0f;
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
void Engine::createParticles(unsigned int numParticles, float maxSize, bool Rand_velocity)
{
	for (int i = 0; i < numParticles; i++)
	{
		particle p = particle(randomPos(walldiagonal1, walldiagonal2), randomSize(maxSize));
		if (Rand_velocity)
			p.setVelocity(randomPos(-vec3(-.5), vec3(0.5)));
		else
			p.setVelocity(vec3(0));
		particles.push_back(p);
	}
}

void Engine::setAccelaration(vec3 acc) {
	globalAcc = acc;
}

void Engine::updateall() //this is the main function that gets called in infinite loop
{
	//call particle.update() for every element in array
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].update(tconst);
		particles[i].velocity += globalAcc;
	}

	//update velocity according to force/accelaration (if any)

	//call collision handling functions after updation


}

// Function to generate a random vec3 position within a range
vec3 randomPos(vec3 min, vec3 max)
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

float randomSize(float maxSize)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, maxSize);
	
	return dist(gen);
}