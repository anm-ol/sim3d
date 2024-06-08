#include <glm/glm.hpp>
#include <iostream>
#include "particle.h"
#include "Engine.h"

bool isCollision(particle& p1, particle& p2) // should this function handle both particle/wall collision?
{
	float distance = glm::distance(p1.pos, p2.pos);
	return distance <= p1.size;	// if euclidian distance b/w two particles is less than their combined radius, means they are colliding
}

void resolveCollision(particle& p1, particle& p2)
{
	// if collision true then make the spheres "un-interesect"
	if (isCollision(p1, p2)) {
		// TODO: make them "un-intersect"
	}
	// update position of p1 and p2
}

// takes engine reference as input
void wallCollide(Engine& engine)
{
	// check is particle is outside bounds or intersecting
	//  TODO: will also have to look at the particle's radius to determine whether it is outside bounds
	for (int i = 0; i < engine.particles.size(); i++)
	{
		particle& particlei = engine.particles[i];
		glm::vec3 pos = particlei.pos;	 // getting the current position of the particle
		float radius = particlei.size; // getting the radius of the particle

		/*
			check is particle is outside bounds or intersecting
			we also include the particle's radius in the eqn
			based on which co-ordinate of the particle is out of bounds,
			velocity will be reversed in that direction only
		*/
		glm::vec3 newVel = particlei.velocity;
		if (pos.x - radius <= engine.xmin)
		{
			//particlei.pos.x += radius - particlei.pos.x;
			newVel.x = -newVel.x;
			particlei.setVelocity(newVel);	// reverse vel in x axis
		}
		if ( pos.x + radius >= engine.xmax)
		{
			//particlei.pos.x += -radius - particlei.pos.x;
			newVel.x = -newVel.x;
			particlei.setVelocity(newVel);	// reverse vel in x axis
		}
		if (pos.y - radius <= engine.ymin)
		{
			//particlei.pos.y += radius - particlei.pos.y;
			newVel.y = -newVel.y;
			particlei.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.y + radius >= engine.ymax)
		{
			//particlei.pos.y += -radius - particlei.pos.y;
			newVel.y = -newVel.y;
			particlei.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.z - radius <= engine.zmin)
		{
			//particlei.pos.z += radius - particlei.pos.z;
			newVel.z = -newVel.z;
			particlei.setVelocity(newVel);	// reverse vel in z axis
		}
		if (pos.z + radius >= engine.zmax)
		{
			//particlei.pos.z += -radius - particlei.pos.z;
			newVel.z = -newVel.z;
			particlei.setVelocity(newVel);	// reverse vel in z axis
		}
	}
}


