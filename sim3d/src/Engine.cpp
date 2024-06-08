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
	wallElasticity = 1.0f;
	particleElasticity = 1.0f;
}
Engine::Engine()
{
	xmin = 0;
	ymin = 0;
	zmin = 0;
	tconst = 1.0f;
	wallElasticity = 1.0f;
	particleElasticity = 1.0f;
	globalAcc = vec3(0);
}
void Engine::setWall(vec3 diag1, vec3 diag2)
{
	walldiagonal1 = diag1;
	walldiagonal2 = diag2;
	xmin = walldiagonal1.x; ymin = walldiagonal1.y; zmin = walldiagonal1.z;
	xmax = walldiagonal2.x; ymax = walldiagonal2.y; zmax = walldiagonal2.z;
}
	
void Engine::setAccelaration(vec3 acc) {
	globalAcc = acc;
}

void Engine::updateall(float dt) //this is the main function that gets called in infinite loop
{
	for (auto& p : particles)
	{
		//call particle.update() for every element in array
		p.update(tconst);
		p.velocity += globalAcc;
	}

	//call collision handling functions after updation
	// handling wall collisions
	wallCollide(*this);
	// handling inter-particle collisions
	particleCollide(*this);
}

// create particles randomly from numParticles, size and maxVel
void Engine::createParticles(int numParticles, float size, float mass, vec3 maxVel, bool randVelocity)
{
	for (int i = 0; i < numParticles; i++)
	{
		createParticle(size, mass, maxVel, randVelocity);
	}
}

// both mass and size are fixed. may make them random later!
void Engine::createParticle(float size, float mass, vec3 maxVel, bool randVelocity)
{
	// walldiagonal +- size is to ensure particles don't intersect the wall
	particle p = particle(randomVec3(walldiagonal1 + size, walldiagonal2 - size), size, mass);
	bool intersects = false;

	for (auto& other : particles)
	{
		if (isCollision(p, other))
		{
			intersects = true;
			break;
		}
	}
	if (intersects) {
		createParticle(size, mass, maxVel, randVelocity);
	}
	else {
		p.setVelocity(randVelocity ? randomVec3(-maxVel, maxVel) : maxVel);
		particles.push_back(p);
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