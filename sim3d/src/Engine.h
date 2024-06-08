#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

using namespace glm;


vec3 randomVec3(vec3 min, vec3 max);

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
	void updateall(float dt);
	void setAccelaration(glm::vec3);

	void createParticles(int numParticles, float size, float mass, glm::vec3 maxVel);
	void createParticle(float size, float mass, glm::vec3 maxVel);
};
