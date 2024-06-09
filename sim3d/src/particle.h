#pragma once
#include <glm/glm.hpp>
class particle {
public:
	glm::vec3 pos;
	glm::vec3 velocity;
	float size;
	float mass;

	particle(glm::vec3 posn, float size, float mass);
	void setVelocity(glm::vec3 v);
	glm::vec3 getPosition();
	void update(float time);

	bool operator==(const particle& other) const;
};