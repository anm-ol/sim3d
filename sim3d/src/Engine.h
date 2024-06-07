#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

class Engine {
public:
	std::vector<particle> particles;
	float tconst;
	glm::vec3 globalAcc;
	float xmax, xmin, ymax, ymin, zmin, zmax;
	glm::vec3 walldiagonal1, walldiagonal2;
	int n;

	Engine(float xm, float ym, float zm);
	Engine();
	void setWall(glm::vec3 diag1, glm::vec3 diag2);
	void createParticles(int numParticles, float size, float maxVel);
	void updateall();
	void setAccelaration(glm::vec3);
};