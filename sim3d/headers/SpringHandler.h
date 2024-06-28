#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "particle.h"

class Engine;

using namespace glm;

struct spring {
	particle &p1, &p2;
	float naturalLength;
	const float coefficient, damping;

	spring(particle& v1, particle& v2, float coeff)
		: p1(v1), p2(v2), coefficient(coeff), damping(coeff/2)
	{
		naturalLength = distance(p1.pos, p2.pos);
	};

	//set force according to hooke's law
	//if there is an alternate to using glm::distance() , we should use it since sqrt is expensive
	void setForce()
	{
		vec3 dir = normalize(p1.pos - p2.pos);
		vec3 relativeVel = p1.velocity - p2.velocity;
		vec3 F_spring = coefficient * (distance(p1.pos, p2.pos) - naturalLength) * -dir;
		vec3 F_damping = damping * dot(relativeVel, dir) * -dir;
		vec3 F = F_spring + F_damping;
		p1.force += F;
		p2.force += -F;
	}
};

class SpringHandler
{
public:
	// number of particles in x and y axes
	int num_x, num_y;
	float size, mass, m_spacing;
	vec3 m_startPos;
	
	//spring co-efficients
	float structCoeff = 1.0f, shearCoeff = 1.0f, bendingCoeff = 1.0f;

	std::vector<particle> *targetVector;
	std::vector<unsigned int> particleIDs;
	std::vector<spring> springs;

	SpringHandler() : targetVector(nullptr){}

	SpringHandler(std::vector<particle> *particles, int w, int h, float s, float m);

	void init(Engine& engine);
	void initVertices(Engine& engine, vec3 StartPos, float spacing);
	void initSprings();
	void addSpring(particle& p1, particle& p2);
	void updateForce();
};

