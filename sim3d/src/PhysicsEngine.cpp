#include <glm/glm.hpp>
#include "particle.h"
#include "collision.h"

particle* particles; //this is array of all existing particles

void updateall() //this is the main function that gets called in infinite loop
{	
	//call particle.update() for every element in array
	
	//update velocity according to force/accelaration (if any)

	//call collision handling functions after updation
}