#include <glm/glm.hpp>
#include "particle.h"
#include <iostream>

using namespace glm;

bool isCollision(particle *p1, particle *p2) // should this function handle both particle/wall collision?
{
	return false;
	// need to write collision handling for two spherical particles here.
}

void resolveCollision(particle *p1, particle *p2)
{
	// if collision true then make the spheres "un-interesect"
	if(isCollision(p1, p2)){
		// TODO: make them "un-intersect"
	}
	// update position of p1 and p2
}

// also need function to handle collision with wall
static void wallCollide(particle p)
{

	vec3 pos = p.pos;	 // getting the current position of the particle
	float size = p.size; // getting the size of the particle

	float xmin = 0.0, xmax{}, ymin = 0.0, ymax{}, zmin = 0.0, zmax{}; // take values from engine

	// check is particle is outside bounds or intersecting
	//  TODO: will also have to look at the particle's radius to determine whether it is outside bounds
	if (pos.x >= xmax || pos.x <= xmin || pos.y >= ymax || pos.y <= ymin || pos.z >= zmax || pos.z <= zmin)
	{
		// the particle is colliding with the wall
		p.setVelocity(-p.velocity); // reverse the velocity of the particle. This might have to be changed later.
	}
}