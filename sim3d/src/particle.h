#pragma once
#include <glm/glm.hpp>
class particle {
	glm::vec3 pos;
	glm::vec3 velocity;
	float size;

	particle(glm::vec3 posn, float size);
	void setVelocity();
	void update(float time);
};