#pragma once
#include <glm/glm.hpp>
class particle {
public:
	glm::vec3 pos;
	glm::vec3 velocity;
	float size;

	particle(glm::vec3 posn, float size);
	void setVelocity(glm::vec3 v);
    void update(float time);
};