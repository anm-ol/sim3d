#include "SpringHandler.h"
#include "Engine.h"

using namespace glm;

SpringHandler::SpringHandler() {

}


// width and height refers to num particles in x and y axes respectively
SpringHandler::SpringHandler(int x, int y, float s, float m)
	: num_x(x), num_y(y), size(s), mass(m) {
};


// adding particles as vertices
void SpringHandler::initVertices(Engine& engine) {
    unsigned int total_num = num_x * num_y;
    engine.particles.reserve(total_num);
	particleIDs.reserve(total_num);

	for (int y = 0; y < num_y; y++) {
		for (int x = 0; x < num_x; x++) {
			vec3 posn = vec3(x * (size + 5), y * (size + 5), 0.0f);
			engine.particles.emplace_back(posn, size, mass);
            particleIDs.emplace_back(engine.particles.size() - 1);
		}
	}
}

void SpringHandler::init(Engine& engine) {
    initVertices(engine);
    initSprings(engine);
}

void SpringHandler::initSprings(Engine& engine) {
    springs.reserve(num_x * num_y * 6);

    for (int y = 0; y < num_y; y++) {
        for (int x = 0; x < num_x; x++) {
            int index = y * num_x + x;
            if (x < num_x - 1) {
                // Horizontal structural 
                springs.emplace_back(engine.particles[particleIDs[index]], engine.particles[particleIDs[index + 1]], structCoeff);
            }
            if (y < num_y - 1) {
                // Vertical structural edge
                springs.emplace_back(engine.particles[particleIDs[index]], engine.particles[particleIDs[index + 1]], structCoeff);
            }
            if (x < num_x - 1 && y < num_y - 1) {
                // Shear edges
                springs.emplace_back(engine.particles[particleIDs[index]], engine.particles[particleIDs[index + num_x + 1]], shearCoeff);
                springs.emplace_back(engine.particles[particleIDs[index + 1]], engine.particles[particleIDs[index + num_x]], shearCoeff);
            }
            if (x < num_x - 2) {
                // Horizontal bending edge
                springs.emplace_back(engine.particles[particleIDs[index]], engine.particles[particleIDs[index + 2]], bendingCoeff);
            }
            if (y < num_y - 2) {
                // Vertical bending edge
                springs.emplace_back(engine.particles[particleIDs[index]], engine.particles[particleIDs[index + 2]], bendingCoeff);
            }
        }
    }
}

void SpringHandler::updateForce(Engine& engine)
{
    //reset force to zero 
    for (auto& index : particleIDs)
    {
        engine.particles[index].force = engine.globalAcc;
    }

    //re-calculate force with updated vertex positions
    for (spring& spr : springs)
    {
        spr.setForce();
    }
}