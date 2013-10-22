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
  float mass;
  float density;

  unsigned num; //num of particle
  Particle *component; //list of particle
  vect *offset;

  vect position;
  vect velocity;
  vect acceleration;
  vect angularVelocity;
  vect angularMomentum;

  mat3x3 inertiaInv;
  quat quaternion;
} Granular;

//-------------------------------------
void InitBoxGranular(unsigned index, Granular *gran, const vect pos,
		float bigR, float smallR, float density);
void InitGranularHPlane(unsigned index, Granular *gran, const vect min_pos,
		unsigned l, unsigned w, float radius, float density);
void InitGranularSphere(unsigned index, Granular *gran, const vect pos,
    float radius, float density);
void ComputeGranularForce(Granular *iG, Granular *jG);
void GranularTimeIntergration(Granular *, float time_step);
void InitGranularInertia(Granular *);
void FreeGranular(Granular *);
#endif
