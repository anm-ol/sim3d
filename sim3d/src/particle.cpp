#include <iostream>
#include <glm/glm.hpp>

using namespace glm;

class particle 
{
public:
	vec3 pos;
	vec3 velocity;
	float size;

	particle(vec3 posn,float s) {
		pos = posn;
		size = s; 
		velocity = vec3(0);
	}

	void setVelocity(vec3 v) {
		velocity = v;
	}

	vec3 getPosition() 
	{
		return pos;
	}

	void update(float time) 
	{
		pos += velocity * time;
	}
};