#ifndef PARTICLE_H_H
#define PARTICLE_H_H
#include "vect.h"
typedef struct{
  float radius;
  vect position;
  vect velocity;
  vect force;
  //ParticleIndex index;
	/*
  float mass;
  float density;
  float young;
  float poission;
	*/
} Particle;

void InitParticleA(Particle *p_list, unsigned num);
void InitParticle(Particle *, const vect, float);
void ComputeParticleForce(Particle *, Particle *);
static inline
void UpdateParticlePosition(Particle *ip, float ts)
{
  vectScaleAdd(ip->position, ip->velocity, ts);
}

#endif
