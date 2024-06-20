#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "particle.h"

using namespace glm;

struct spring {
	particle& vertex1, vertex2;
	float naturalLength = distance(vertex1.pos, vertex2.pos);
	float coefficient;

	spring(particle& v1, particle& v2, float coeff)
		: vertex1(v1), vertex2(v2), coefficient(coeff)
	{};
};

class SpringHandler
{
public:
	int width, height;
	float size, mass;
	
	//spring co-efficients
	float structCoeff = 1.0f, shearCoeff = 1.0f, bendingCoeff = 1.0f;

	std::vector<particle> particles;
	std::vector<spring> springs;

	SpringHandler();
	SpringHandler(int w, int h, float s, float m);

	void initVertices();
	void initSprings();
};

