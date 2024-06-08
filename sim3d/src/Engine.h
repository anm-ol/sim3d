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
	int n;
	
	float Elasticity;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;

	Engine(float xm, float ym, float zm);
	Engine();
	void setWall(glm::vec3 diag1, glm::vec3 diag2);
	void createParticles(int numParticles, float maxSize, float maxVel,bool random_velocity);
	void updateall();
	void setAccelaration(glm::vec3);
};