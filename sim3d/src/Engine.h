#include <glm/glm.hpp>
#include <vector>
#ifndef PARTICLE_H
#define PARTICLE_H
#include "particle.h"
#endif


class Engine {
public:
	std::vector<particle> particles;
	float tconst;
	float xmax, xmin, ymax, ymin, zmin, zmax;

	Engine(float xm, float ym, float zm);

	void updateall();
};