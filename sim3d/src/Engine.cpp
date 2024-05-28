#include <glm/glm.hpp>
#include "particle.h"
#include <iostream>
#include <vector>

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

	bool isCollision(particle& p1, particle& p2) // should this function handle both particle/wall collision?
	{
		return false;
		// need to write collision handling for two spherical particles here.
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
			float size = p.size; // getting the size of the particle

			if (pos.x >= engine.xmax || pos.x <= engine.xmin || pos.y >= engine.ymax || pos.y <= engine.ymin || pos.z >= engine.zmax || pos.z <= engine.zmin)
			{
				// the particle is colliding with the wall
				p.setVelocity(-p.velocity); // reverse the velocity of the particle. This might have to be changed later.
			}
		}
	}
};