#pragma once
#include <glm/glm.hpp>
#include <iostream>


class particle {
public:
	glm::vec3 oldPos, pos;
	glm::vec3 velocity;
	glm::vec3 force;
	glm::vec3 color;
	float size, mass;
	bool isPivot = false;

	particle(glm::vec3 posn, float size, float mass);
	particle(const particle& other);

	void setVelocity(glm::vec3 v);
	void update(float time, glm::vec3 accln);

	bool operator==(const particle& other) const
	{
		return this->pos == other.pos;
	}
};