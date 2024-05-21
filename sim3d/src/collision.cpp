#include <glm/glm.hpp>
#include "particle.h"
#include <iostream>

bool isCollision(particle* p1, particle* p2) //should this function handle both particle/wall collision?
{
	return false;
	//need to write collision handling for two spherical particles here.
}

void resolveCollision() {
	//if collision true then make the spheres "un-interesect"
	//update position of p1 and p2
}

//also need function to handle collision with wall
void wallCollide() {

	float xmin, xmax, ymin, ymax, zmin, zmax; //take values from engine 

	//check is particle is outside bounds or intersecting

}