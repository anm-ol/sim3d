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
		particle& p = engine.particles[i];
		glm::vec3 pos = p.pos;	 // getting the current position of the particle
		float radius = p.size; // getting the radius of the particle

		/*
			check is particle is outside bounds or intersecting
			we also include the particle's radius in the eqn
			based on which co-ordinate of the particle is out of bounds,
			velocity will be reversed in that direction only
		*/
		glm::vec3 newVel = p.velocity;
		if (pos.x - radius <= engine.xmin)
		{
			//p.pos.x += radius - p.pos.x;
			newVel.x = -newVel.x;
			p.setVelocity(newVel);	// reverse vel in x axis
		}
		if ( pos.x + radius >= engine.xmax)
		{
			//p.pos.x += -radius - p.pos.x;
			newVel.x = -newVel.x;
			p.setVelocity(newVel);	// reverse vel in x axis
		}
		if (pos.y - radius <= engine.ymin)
		{
			//p.pos.y += radius - p.pos.y;
			newVel.y = -newVel.y;
			p.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.y + radius >= engine.ymax)
		{
			//p.pos.y += -radius - p.pos.y;
			newVel.y = -newVel.y;
			p.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.z - radius <= engine.zmin)
		{
			//p.pos.z += radius - p.pos.z;
			newVel.z = -newVel.z;
			p.setVelocity(newVel);	// reverse vel in z axis
		}
		if (pos.z + radius >= engine.zmax)
		{
			//p.pos.z += -radius - p.pos.z;
			newVel.z = -newVel.z;
			p.setVelocity(newVel);	// reverse vel in z axis
		}
	}
}


