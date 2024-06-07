#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

class Engine {
public:
	std::vector<particle> particles;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;
	glm::vec3 walldiagonal1, walldiagonal2;

	Engine(float xm, float ym, float zm);
	Engine();
	void setWall(glm::vec3 diag1, glm::vec3 diag2);
	void updateall();
};