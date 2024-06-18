#pragma once
#include <glm/glm.hpp>
#include <iostream>


class particle {
public:
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 color;
	float size;
	float mass;

	particle(glm::vec3 posn, float size, float mass);
	particle(const particle& other);

	void setVelocity(glm::vec3 v);
	glm::vec3 getPosition();
	void update(float time);

	bool operator==(const particle& other) const
	{
		return this->pos == other.pos;
	}
	void update(float time, glm::vec3 accln);
};