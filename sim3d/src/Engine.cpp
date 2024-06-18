#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>

#include "particle.h"
#include "Engine.h"
#include "collision.h"
#include "SpacePartition.h"

using namespace glm;



Engine::Engine(const vec3& diag1, const vec3& diag2) : walldiagonal1(diag1), walldiagonal2(diag2), box(*this, 20)
{	
	tconst = 1.0f;
	wallElasticity = 1.0f;
	particleElasticity = 1.0f;
	friction = 1;
	globalAcc = vec3(0);
	NumSteps = 1;


	m_NumThreads = std::thread::hardware_concurrency(); // Get the number of supported hardware threads
	pause = false;
	useThreading = false;
	usePartition = false;
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
	if(!pause)
	runSubsteps(NumSteps, dt); //more numsteps --> more accuracy
}

//Betweem each frame we update position/velocity, handle collision multiple times
void Engine::runSubsteps(int numstep, float dt) 
{
	for (int i = 1; i <= numstep; i++)
	{
		//one "sub-step"
		for (auto& p : particles)
		{
			//updates position for each particle
			// T_const may be replaced with dt in the future
			p.update(tconst / numstep);
			//update velocity according to gravity
			p.velocity += (globalAcc * tconst) / (float)numstep;
		}

		//call collision handling functions after updation
		// handling wall collisions
		wallCollide(*this);
		
		// handling inter-particle collisions
		if (useThreading)
			collisionParralel(*this);
		else if (usePartition)
		{
			box.reset();
			box.partitionCollide();
		}
		else
		particleCollide(*this, 0, particles.size());
	}
}

// create particles randomly from numParticles, size and maxVel
// for some reasom the application crashes abruptly at 1185 particles when vector.reserve is not being used
// crasher at lesser number of particles when reserve() is used
// something weird is happening with std::vector
void Engine::createParticles(int numParticles, float size, float mass, vec3 maxVel, bool randVelocity)
{
	particles.reserve(numParticles);
	for (int i = 0; i < numParticles; i++)
	{
		createParticle(size, mass, maxVel, randVelocity);
	}
}

// both mass and size are fixed. may make them random later!
// need to optimize this part
void Engine::createParticle(float size, float mass, vec3 maxVel, bool randVelocity)
{
	// walldiagonal +- size is to ensure particles don't intersect the wall
	particle p(randomVec3(walldiagonal1 + size, walldiagonal2 - size), size, mass);
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
		particles.emplace_back(p);
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