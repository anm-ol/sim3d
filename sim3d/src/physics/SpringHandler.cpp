#include "SpringHandler.h"

using namespace glm;

SpringHandler::SpringHandler() {

}


// width and height refers to num particles in x and y axes respectively
SpringHandler::SpringHandler(int w, int h, float s, float m)
	: width(w), height(h), size(s), mass(m) {

	initVertices();
	initSprings();
};


// adding particles as vertices
void SpringHandler::initVertices() {
	particles.reserve(width * height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			vec3 posn = vec3(x * size, y * size, 0.0f);
			particles.emplace_back(posn, size, mass);
		}
	}
}

void SpringHandler::initSprings() {
    springs.reserve(width * height * 6);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;

            if (x < width - 1) {
                // Horizontal structural edge
                springs.emplace_back(particles[index], particles[index + 1], structCoeff);
            }
            if (y < height - 1) {
                // Vertical structural edge
                springs.emplace_back(particles[index], particles[index + width], structCoeff);
            }
            if (x < width - 1 && y < height - 1) {
                // Shear edges
                springs.emplace_back(particles[index], particles[index + width + 1], shearCoeff);
                springs.emplace_back(particles[index + 1], particles[index + width], shearCoeff);
            }
            if (x < width - 2) {
                // Horizontal bending edge
                springs.emplace_back(particles[index], particles[index + 2], bendingCoeff);
            }
            if (y < height - 2) {
                // Vertical bending edge
                springs.emplace_back(particles[index], particles[index + 2], bendingCoeff);
            }
        }
    }
}

void SpringHandler::updateForce()
{
    //reset force to zero 
    for (particle& p : particles)
    {
        p.force = vec3(0);
    }
    //re-calculate force with updated vertex positions
    for (spring& spr : springs)
    {
        spr.setForce();
    }
}