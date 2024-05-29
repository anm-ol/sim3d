#include <glm/glm.hpp>
#include "particle.h"
#include <iostream>

using namespace glm;

bool isCollision(particle p1, particle p2) // should this function handle both particle/wall collision?
{
	float distance = glm::distance(p1.pos, p2.pos);
	return distance <= p1.size;	// if euclidian distance b/w two particles is less than their combined radius, means they are colliding
}

void resolveCollision(particle p1, particle p2)
{
	// if collision true then make the spheres "un-interesect"
	if (isCollision(p1, p2))
	{
		// TODO: make them "un-intersect"

	}
	// update position of p1 and p2
}

// also need function to handle collision with wall
void wallCollide(particle p)
{

	vec3 pos = p.pos;	 // getting the current position of the particle
	float radius = p.size / 2; // getting the radius of the particle

	float xmin, xmax, ymin, ymax, zmin, zmax; // take values from engine
	/*
		check is particle is outside bounds or intersecting
		we also include the particle's radius in the eqn
		based on which co-ordinate of the particle is out of bounds, 
		velocity will be reversed in that direction only
	*/
	vec3 newVel = p.velocity;
	if (pos.x + radius >= xmax || pos.x - radius <= xmin)
	{
		newVel.x = -newVel.x;
		p.setVelocity(newVel);	// reverse vel in x axis
	}
	if (pos.y + radius >= ymax || pos.y -radius <= ymin)
	{
		newVel.y = -newVel.y;
		p.setVelocity(newVel);	// reverse vel in y axis
	}
	if (pos.z + radius >= zmax || pos.z - radius <= zmin)
	{
		newVel.z = -newVel.z;
		p.setVelocity(newVel);	// reverse vel in z axis
	}
}