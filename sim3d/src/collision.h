#include "particle.h"
#include "Engine.h"

bool isCollision(particle& p1, particle& p2);
void resolveCollision(particle& p1, particle& p2, float elasticity);
void wallCollide(Engine& engine);
void particleCollide(Engine& engine);

void assignParticlesToCells(Engine& engine);