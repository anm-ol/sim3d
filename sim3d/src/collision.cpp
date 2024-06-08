#include <glm/glm.hpp>
#include <iostream>
#include "particle.h"
#include "Engine.h"

using namespace glm;

bool isCollision(particle& p1, particle& p2) // should this function handle both particle/wall collision?
{
	float distance = glm::distance(p1.pos, p2.pos);
	return distance <= p1.size + p2.size;	// if euclidian distance b/w two particles is less than their combined radius, means they are colliding
}

void resolveCollision(particle& p1, particle& p2)
{
	// if collision true then make the spheres "un-interesect"
	vec3 pos1 = p1.getPosition();
	vec3 pos2 = p2.getPosition();

	float m1 = p1.mass;
	float m2 = p2.mass;

	vec3 n = normalize(pos2 - pos1);
	float mEff = (m1 * m2) / (m1 + m2);
	float vImp = dot(n, p1.velocity - p2.velocity);

	float J = 1 * mEff * vImp;
	vec3 dv1 = -J / m1 * n;
	vec3 dv2 = J / m2 * n;

	vec3 newV1 = p1.velocity + dv1;
	vec3 newV2 = p2.velocity + dv2;

	p1.setVelocity(newV1);
	p2.setVelocity(newV2);
}

// takes engine reference as input
void wallCollide(Engine& engine)
{
	// check is particle is outside bounds or intersecting
	for (auto& particlei : engine.particles)
	{
		vec3 pos = particlei.pos;	 // getting the current position of the particle
		float radius = particlei.size; // getting the radius of the particle

		/*
			check if particle is outside bounds or intersecting
			we also include the particle's radius in the eqn
			based on which co-ordinate of the particle is out of bounds,
			velocity will be reversed in that direction only
		*/
		vec3 newVel = particlei.velocity;
		if (pos.x - radius <= engine.xmin)
		{
			particlei.pos.x += engine.xmin + radius - particlei.pos.x;
			newVel.x = - engine.Elasticity * newVel.x;
			particlei.setVelocity(newVel);	// reverse vel in x axis
		}
		if ( pos.x + radius >= engine.xmax)
		{
			particlei.pos.x += engine.xmax-radius - particlei.pos.x;
			newVel.x = - engine.Elasticity * newVel.x;
			particlei.setVelocity(newVel);	// reverse vel in x axis
		}
		if (pos.y - radius <= engine.ymin)
		{
			particlei.pos.y += engine.ymin + radius - particlei.pos.y;
			newVel.y = - engine.Elasticity * newVel.y;
			particlei.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.y + radius >= engine.ymax)
		{
			particlei.pos.y += engine.ymax -radius - particlei.pos.y;
			newVel.y = - engine.Elasticity * newVel.y;
			particlei.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.z - radius <= engine.zmin)
		{
			particlei.pos.z += engine.zmin + radius - particlei.pos.z;
			newVel.z = - engine.Elasticity * newVel.z;
			particlei.setVelocity(newVel);	// reverse vel in z axis
		}
		if (pos.z + radius >= engine.zmax)
		{
			particlei.pos.z += engine.zmax - radius - particlei.pos.z;
			newVel.z = - engine.Elasticity * newVel.z;
			particlei.setVelocity(newVel);	// reverse vel in z axis
		}
	}
}


