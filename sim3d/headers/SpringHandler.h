#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "particle.h"

class Engine;

using namespace glm;

struct spring {
	particle& p1, p2;
	float naturalLength;
	float coefficient;

	spring(particle& v1, particle& v2, float coeff)
		: p1(v1), p2(v2), coefficient(coeff)
	{
		naturalLength = distance(p1.pos, p2.pos);
	};

	//set force according to hooke's law
	//if there is an alternate to using glm::distance() , we should use it since sqrt is expensive
	void setForce()
	{
		vec3 dir = normalize(p1.pos - p2.pos);
		vec3 F = coefficient * (distance(p1.pos, p2.pos) - naturalLength) * -dir;
		p1.force += F;
		p2.force += -F;
	}
};

class SpringHandler
{
public:
	// number of particles in x and y axes
	int num_x, num_y;
	float size, mass;
	
	//spring co-efficients
	float structCoeff = 1.0f, shearCoeff = 1.0f, bendingCoeff = 1.0f;

	std::vector<particle> *targetVector;
	std::vector<unsigned int> particleIDs;
	std::vector<spring> springs;

	SpringHandler() : targetVector(nullptr){}

	SpringHandler(std::vector<particle> *particles, int w, int h, float s, float m);

	void initVertices();
	void initSprings();
	void addSpring(particle& p1, particle& p2);
	void updateForce();
};

