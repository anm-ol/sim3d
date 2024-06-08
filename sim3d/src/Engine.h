#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

using namespace glm;
class Engine {
public:
	std::vector<particle> particles;
	glm::vec3 globalAcc;
	glm::vec3 walldiagonal1, walldiagonal2;
	
	float particleElasticity, wallElasticity;
	float friction;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;

	int NumSteps;

	Engine(float xm, float ym, float zm);
	Engine();
	void setWall(glm::vec3 diag1, glm::vec3 diag2);
	void updateall(float dt);
	void runSubsteps(int numstep, float dt);
	void setAccelaration(glm::vec3);


	void createParticles(int numParticles, float size, float mass, glm::vec3 maxVel, bool randVelocity);
	void createParticle(float size, float mass, glm::vec3 maxVel, bool randVelocity);
};