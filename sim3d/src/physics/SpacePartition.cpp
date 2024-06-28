#define GLM_ENABLE_EXPERIMENTAL

#include "SpacePartition.h"
#include "collision.h"
#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

void partition::addParticle(particle* p)
{
    group.emplace_back(p);
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
    ivec4 min, max;
    for (auto& p : particles)
    {
        int index = getCellIndex(p.pos);
        
        min.x = getCellIndex(p.pos - vec3(p.size, 0, 0));
        min.y = getCellIndex(p.pos - vec3(0, p.size, 0));
        min.z = getCellIndex(p.pos - vec3(0, 0, p.size));
        min.w = getCellIndex(p.pos - p.size);
        
        max.x = getCellIndex(p.pos + vec3(p.size, 0, 0));
        max.y = getCellIndex(p.pos + vec3(0, p.size, 0));
        max.z = getCellIndex(p.pos + vec3(0, 0, p.size));
        max.w = getCellIndex(p.pos + p.size);

        partitions[index].addParticle(&p);
        if (min.x != index)
            partitions[min.x].addParticle(&p);
        if(min.y != index)
            partitions[min.y].addParticle(&p);
        if(min.z != index)
            partitions[min.z].addParticle(&p);
        if (min.w != index)
            partitions[min.w].addParticle(&p);

        if (max.x != index)
            partitions[max.x].addParticle(&p);
        if (max.y != index)
            partitions[max.y].addParticle(&p);
        if (max.z != index)
            partitions[max.z].addParticle(&p);
        if (max.w != index)
            partitions[max.w].addParticle(&p);
    }
}

int spacePartition::getCellIndex(vec3 pos)
{   
    // Calculate box indices in each dimension
    ivec3 boxindex = (pos - worldmin) / boxsize;

    // Clamp box indices to valid range [0, subDivs - 1]
    boxindex.x = glm::clamp(boxindex.x, 0, subDivs - 1);
    boxindex.y = glm::clamp(boxindex.y, 0, subDivs - 1);
    boxindex.z = glm::clamp(boxindex.z, 0, subDivs - 1);

    // Calculate linear index
    int index = boxindex.x * subDivs * subDivs + boxindex.y * subDivs + boxindex.z;

    return index;
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