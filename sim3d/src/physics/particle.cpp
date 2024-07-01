#include <iostream>
#include <glm/glm.hpp>
#include "particle.h"

using namespace glm;

particle::particle(glm::vec3 posn, float s, float m)
    : pos(posn), size(s), mass(m), velocity(0), force(0), oldPos(0)
{
    color = vec3(0.2, 0.1, 0.8);
}

void particle::setVelocity(glm::vec3 v) 
{
    velocity = v;
   // oldPos = pos - velocity * 1.0f;
}

//position verlet function
void particle::update(float time, glm::vec3 acc) 
{
    if (isPivot) return;
    if (velocity != (pos - oldPos) / time)
    {
        oldPos = pos - velocity * time;
    }
    glm::vec3 temp = pos;
    pos += (pos - oldPos) + (acc + force/mass) * (time * time);
    oldPos = temp;

    velocity = (pos - oldPos) / time;
}

//copy constructor to detect when copies are created
particle::particle(const particle& other) 
    : pos(other.pos), oldPos(other.oldPos), size(other.size), mass(other.mass), color(other.color), velocity(other.velocity), force(other.force)
{}
