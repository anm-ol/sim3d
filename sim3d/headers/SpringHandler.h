#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "particle.h"

class Engine;

struct spring {
	particle &p1, &p2;
	float naturalLength;
	float coefficient, damping;

	spring(particle& v1, particle& v2, float coeff)
		: p1(v1), p2(v2), coefficient(coeff), damping(coeff * 0.5)
	{
		naturalLength = glm::distance(p1.pos, p2.pos);
	};

	//set force according to hooke's law
	//if there is an alternate to using glm::distance(), we should use it since sqrt is expensive
	void setForce()
	{
		glm::vec3 dir = normalize(p1.pos - p2.pos);
		glm::vec3 relativeVel = p1.velocity - p2.velocity;
		glm::vec3 F_spring = coefficient * (distance(p1.pos, p2.pos) - naturalLength) * -dir;
		glm::vec3 F_damping = damping * dot(relativeVel, dir) * -dir;
		glm::vec3 F = F_spring + F_damping;
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
	glm::vec3 m_startPos;
	bool isInit = false;

	//spring co-efficients
	float structCoeff = 0.5f, shearCoeff = 0.5f, bendingCoeff = 1.0f;

	std::vector<particle> *targetVector;
	std::vector<glm::vec3> particlePositions;
	std::vector<int> particleIDs;
	std::vector<spring> springs;
	int center;

	SpringHandler() : targetVector(nullptr){}

	SpringHandler(std::vector<particle> *particles, int w, int h, float s, float m);

	void init(Engine& engine);
	void initVertices(Engine& engine, glm::vec3 StartPos, float spacing);
	void initSprings();
	void addSpring(particle& p1, particle& p2);
	void updateForce();

	void translate(glm::vec3 translate);
	void rotatecloth(glm::vec3 rotation);
};

