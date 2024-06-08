#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

class Engine {
public:
	std::vector<particle> particles;
	glm::vec3 globalAcc;
	glm::vec3 walldiagonal1, walldiagonal2;
	
	float Elasticity;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;

	Engine(float xm, float ym, float zm);
	Engine();
	void setWall(glm::vec3 diag1, glm::vec3 diag2);
	void createParticles(unsigned int numParticles, float maxSize,bool random_velocity);
	void updateall();
	void setAccelaration(glm::vec3);
};