#ifndef GRANULAR_H_H
#define GRANULAR_H_H

#include "particle.h"
#include "vect.h"
#define  GranularDensity (1.0f)
#define  BGSmallR (1.0f)
#define  BGBigR (1.0f)

typedef struct
{
  unsigned num;
  Particle *component;
  quaternion quat;
  mat3x3 inertiaTensorInv;
  vect position;
  vect acceleration;
  vect angularMomentum;
} Granular;

//-------------------------------------
void InitBoxGranuar(Granular *, float bigR, float smallR,
   float density, const vect *pos);
#endif
