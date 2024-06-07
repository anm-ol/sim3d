#include "particle.h"
#include "Engine.h"

bool isCollision(particle& p1, particle& p2);
void resolveCollision(particle& p1, particle& p2);
void wallCollide(Engine& engine);