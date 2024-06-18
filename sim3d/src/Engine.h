#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"
#include "SpacePartition.h"
#include "SpringHandler.h"

using namespace glm;

vec3 randomVec3(vec3 min, vec3 max);

class Engine {
public:
	std::vector<particle> particles;
	spacePartition box;
	SpringHandler handler;

	glm::vec3 globalAcc;
	glm::vec3 walldiagonal1 = vec3(0); glm::vec3 walldiagonal2 = vec3(0);
	
	bool pause;
	bool useThreading;
	bool usePartition;

	float particleElasticity, wallElasticity;
	float friction;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;

	int NumSteps;
	int m_NumThreads;

	Engine(const vec3& min,const vec3& max);
	void setWall(glm::vec3 diag1, glm::vec3 diag2);
	void updateall(float dt);
	void runSubsteps(int numstep, float dt);
	void setAccelaration(glm::vec3);


	void createParticles(int numParticles, float size, float mass, glm::vec3 maxVel, bool randVelocity);
	void createParticle(float size, float mass, glm::vec3 maxVel, bool randVelocity);
};
