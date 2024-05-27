#include <glm/glm.hpp>
#include "particle.h"
#include "collision.h"
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
};