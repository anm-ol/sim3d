#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

class Engine {
public:
	std::vector<particle> particles;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;

	Engine(float xm, float ym, float zm);

	void updateall();
};