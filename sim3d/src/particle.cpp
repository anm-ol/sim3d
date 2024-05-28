#include <iostream>
#include <glm/glm.hpp>

class particle 
{
public:
	glm::vec3 pos;
	glm::vec3 velocity;
	float size;

	particle(glm::vec3 posn,float s) {
		pos = posn;
		size = s; 
		velocity = glm::vec3(0);
	}

	void setVelocity(glm::vec3 v) {
		velocity = v;
	}

	glm::vec3 getPosition() 
	{
		return pos;
	}

	void update(float time) 
	{
		pos += velocity * time;
	}
};