#define GLM_ENABLE_EXPERIMENTAL

#include "SpacePartition.h"
#include "collision.h"
#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

void partition::addParticle(particle* p)
{
    group.push_back(p);
    p->color = color;
}
spacePartition::spacePartition(Engine& engine, int divisions)
    : engine(engine), subDivs(divisions)
{}

void spacePartition::createGrid(vec3 min, vec3 max)
{
    worldmax = max; worldmin = min;
    partitions.reserve((int)pow(subDivs, 3));

    worldsize = worldmax - worldmin;
    boxsize = worldsize / (float)subDivs;
    vec3 boxmin(worldmin), boxmax = boxmin + boxsize;

    int index = 0;

    for (int i = 0; i < subDivs; i++)
    {
        boxmin.y = worldmin.y + i * boxsize.y;
        boxmax.y = boxmin.y + boxsize.y;
        for (int j = 0; j < subDivs; j++)
        {
            boxmin.z = worldmin.z + j * boxsize.z;
            boxmax.z = boxmin.z + boxsize.z;
            for (int k = 0; k < subDivs; k++)
            {
                boxmin.x = worldmin.x + k * boxsize.x;
                boxmax.x = boxmin.x + boxsize.x;

                partitions.emplace_back(boxmin, boxmax);
            }
        }
    }
}

void spacePartition::setParticles(std::vector<particle>& particles)
{
    for (auto& p : particles)
    {
        ivec3 boxindex = (p.pos - worldmin) / boxsize;
        ivec3 minboxindex = vec3(glm::max(0.0f, (p.pos.x - p.size - worldmin.x)), 
                                glm::max(0.0f, p.pos.y - p.size - worldmin.y),
                                 glm::max(0.0f, p.pos.z - p.size - worldmin.z)) / boxsize;
        vec3 maxboxfloat = (p.pos + p.size - worldmin) / boxsize;
        ivec3 maxboxindex = ivec3(glm::min((float)subDivs - 1, maxboxfloat.x),
                                  glm::min((float)subDivs - 1, maxboxfloat.y),
                                  glm::min((float)subDivs - 1, maxboxfloat.z));
        int index = boxindex.x * subDivs * subDivs + boxindex.y * subDivs + boxindex.z;
        int minindex = minboxindex.x * subDivs * subDivs + minboxindex.y * subDivs + minboxindex.z;
        int maxindex = maxboxindex.x * subDivs * subDivs + maxboxindex.y * subDivs + maxboxindex.z;
        partitions[index].addParticle(&p);
        if (minindex != index)
            partitions[minindex].addParticle(&p);
        else if (maxindex != index)
            partitions[maxindex].addParticle(&p);
    }
}

void spacePartition::partitionCollide()
{
    float elasticity = engine.particleElasticity;
    for (auto& part : partitions)
    {
        const std::vector<particle*>& group = part.group;
        int size = group.size();
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = i + 1; j < size; j++)
            {
                if (isCollision(*group[i], *group[j]))
                {
                    resolveCollision(*group[i], *group[j], engine.particleElasticity);
                }
            }

        }
    }
}

void spacePartition::reset()
{
    for (auto& part : partitions)
    {
        part.group.clear();
    }
    setParticles(engine.particles);
}