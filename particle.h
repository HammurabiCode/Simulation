#ifndef PARTICLE_H_H
#define PARTICLE_H_H
#include "vect.h"
#include "common.h"
typedef struct{
  ParticleIndex index;
  float mass;
  float radius;
  float density;
  float young;
  float poission;
  vect position;
  vect velocity;
  vect acceleration;
} Particle;

void InitParticleA(Particle *p_list, unsigned num);

#endif
