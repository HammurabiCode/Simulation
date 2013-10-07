#ifndef GRANULAR_H_H
#define GRANULAR_H_H

#include "Particle.h"
#include "vect.h"
#include "mat.h"
#include "quat.h"
#define  GranularDensity (1.0f)
#define  BGSmallR (1.0f)
#define  BGBigR (1.0f)

typedef struct
{
	unsigned index;
  unsigned num; //num of particle
  float mass;
  Particle *component; //list of particle
  vect *offset;
  vect position;
  vect velocity;
  vect acceleration;
  vect angularVelocity;
  mat3x3 inertiaInv;
  vect angularMomentum;
  quat quaternion;
} Granular;

//-------------------------------------
void InitBoxGranular(Granular *, const vect pos, float bigR, float smallR,
   float density);
void ComputeGranularForce(Granular *iG, Granular *jG);
void GranularTimeIntergration(Granular *, float time_step);
void FreeGranular(Granular *);
#endif
