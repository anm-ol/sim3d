#include <glm/glm.hpp>
#include <iostream>
#include "particle.h"
#include "Engine.h"

using namespace glm;

bool isCollision(particle& p1, particle& p2) // should this function handle both particle/wall collision?
{
	return distance(p1.pos, p2.pos) <= p1.size + p2.size;	// if euclidian distance b/w two particles is less than their combined radius, means they are colliding
}

void resolveCollision(particle& p1, particle& p2, float elasticity)
{
	// if collision true then make the spheres "un-interesect"
	vec3 pos1 = p1.getPosition();
	vec3 pos2 = p2.getPosition();

	float m1 = p1.mass;
	float m2 = p2.mass;

	vec3 n = normalize(pos2 - pos1);
	float mEff = (m1 * m2) / (m1 + m2);
	float vImp = dot(n, p1.velocity - p2.velocity);

	float J = (1 + elasticity) * mEff * vImp;
	vec3 dv1 = -J / m1 * n;
	vec3 dv2 = J / m2 * n;

	vec3 newV1 = p1.velocity + dv1;
	vec3 newV2 = p2.velocity + dv2;

	// if the particles are overlaping, them displace them by some amount to make them un-intersect
	float overlap = (p1.size + p2.size) - distance(pos1, pos2);	
	if (overlap > 0.0f)
	{
		//std::cout << overlap << std::endl;
		vec3 displacement1 = n * (overlap * (p1.size / (p1.size + p2.size)));
		vec3 displacement2 = n * (overlap * (p2.size / (p1.size + p2.size)));
		p1.pos += -displacement1;
		p2.pos += displacement2;
	}

	p1.setVelocity(newV1);
	p2.setVelocity(newV2);
}

// handling inter-particle collision
void particleCollide(Engine& engine)
{
	// we must use a reference here, otherwise the changes will not refelect on the original vector<>
	std::vector<particle>& particles = engine.particles;
	int size = particles.size();
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (isCollision(particles[i], particles[j])) {
				// resolve collisions
				resolveCollision(particles[i], particles[j], engine.particleElasticity);
			}
		}
	}
}

// wall collision
void wallCollide(Engine& engine)
{
	//minimum velocity under which the particle.component will be rounded to zero
	float velocitymin = 0.1;

	// check is particle is outside bounds or intersecting
	for (auto& p : engine.particles)
	{
		vec3 &pos = p.pos;	 // reference to current position of the particle
		float radius = p.size; // getting the radius of the particle
		/*
			check if particle is outside bounds or intersecting
			we also include the particle's radius in the eqn
			based on which co-ordinate of the particle is out of bounds,
			velocity will be reversed in that direction only
		*/
		vec3 newVel = p.velocity;
		if (pos.x - radius < engine.xmin)
		{
			//shift particle to be inside left wall
			pos.x = engine.xmin + radius;
			newVel.x = -engine.wallElasticity * newVel.x; // reverse vel in x axis
			if (glm::abs(p.velocity.x) < velocitymin) 
			{
				//if velocity is lesser than threshold then round it off to zero
				newVel.x = 0;
			}
			p.setVelocity(newVel);	
		}

		//same but for right wall
		if (pos.x + radius > engine.xmax)
		{
			pos.x = engine.xmax - radius;
			newVel.x = -engine.wallElasticity * newVel.x;
			if (glm::abs(p.velocity.x) < velocitymin)
			{
				newVel.x = 0;
			}
			p.setVelocity(newVel);	// reverse vel in x axis
		}
		if (pos.y - radius < engine.ymin)
		{
			pos.y = engine.ymin + radius;
			newVel.y = -engine.wallElasticity * newVel.y;
			if (glm::abs(p.velocity.y) < velocitymin)
			{
				newVel.y = 0;
			}
			p.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.y + radius > engine.ymax)
		{
			pos.y = engine.ymax - radius;
			newVel.y = -engine.wallElasticity * newVel.y;
			if (glm::abs(p.velocity.y) < velocitymin)
			{
				newVel.y = 0;
			}
			p.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.z - radius < engine.zmin)
		{
			pos.z = engine.zmin + radius;
			newVel.z = -engine.wallElasticity * newVel.z;
			if (glm::abs(p.velocity.z) < velocitymin)
			{
				newVel.z = 0;
			}
			p.setVelocity(newVel);	// reverse vel in z axis
		}
		if (pos.z + radius > engine.zmax)
		{
			pos.z = engine.zmax - radius;
			newVel.z = -engine.wallElasticity * newVel.z;
			if (glm::abs(p.velocity.z) < velocitymin)
			{
				newVel.z = 0;
			}
			p.setVelocity(newVel);	// reverse vel in z axis
		}
	}
}


