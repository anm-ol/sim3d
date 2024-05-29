#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "particle.h"

class Engine {
public:
	std::vector<particle> particles; //this is group of all existing particles
	//NOTE: using vectors for now for easy dynamically adding new particles, might change later if
	//not memory efficient
	float t_const = 1.0f;
	float xmin, xmax, ymin, ymax, zmin, zmax;

	//constructor to define wall diagonal, sets origin as other diagonal
	//might need to change later
	Engine(float xm,float ym,float zm) 
	{
		xmin = 0;
		ymin = 0;
		zmin = 0;
		xmax = xm;
		ymax = ym;
		zmax = zm;
	}

	void updateall() //this is the main function that gets called in infinite loop
	{
		particles.push_back(particle(glm::vec3(1), 1.0f));
		//call particle.update() for every element in array
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i].update(t_const);
		}

		//update velocity according to force/accelaration (if any)

		//call collision handling functions after updation

      
	}
};

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
		float radius = p.size / 2; // getting the radius of the particle

		/*
			check is particle is outside bounds or intersecting
			we also include the particle's radius in the eqn
			based on which co-ordinate of the particle is out of bounds,
			velocity will be reversed in that direction only
		*/
		glm::vec3 newVel = p.velocity;
		if (pos.x + radius >= engine.xmax || pos.x - radius <= engine.xmin)
		{
			newVel.x = -newVel.x;
			p.setVelocity(newVel);	// reverse vel in x axis
		}
		if (pos.y + radius >= engine.ymax || pos.y - radius <= engine.ymin)
		{
			newVel.y = -newVel.y;
			p.setVelocity(newVel);	// reverse vel in y axis
		}
		if (pos.z + radius >= engine.zmax || pos.z - radius <= engine.zmin)
		{
			newVel.z = -newVel.z;
			p.setVelocity(newVel);	// reverse vel in z axis
		}
	}
}