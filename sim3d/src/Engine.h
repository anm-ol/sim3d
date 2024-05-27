#pragma once
#include <glm/glm.hpp>
#include "particle.h"
#include "collision.h"
#include <vector>

class Engine {
public:
	std::vector<particle> particles;
	float tconst;
	float xmax, xmin, ymax, ymin, ymax, zmin, zmax;

	Engine(float xm, float ym, float zm);
	void updateall();
};