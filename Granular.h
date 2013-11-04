#ifndef GRANULAR_H_H
#define GRANULAR_H_H

#include "Particle.h"
#include "common.h"
#include "vect.h"
#include "mat.h"
#include "quat.h"
#define  GranularDensity (1.0f)
#define  BGSmallR (1.0f)
#define  BGBigR (1.0f)

#define NUM_PART_GRAN_BOX (9)
#define NUM_PART_GRAN_SPHERE (1)
#define NUM_PART_GRAN_PLANE(w, l) ((w)*(l))

typedef struct
{
	unsigned index;
	ParticleIndex iStart;
  float mass;
  float density;

  vect position;
  vect velocity;
  vect acceleration;
  vect angularVelocity;
  vect angularMomentum;

  unsigned num; //num of particle
  Particle *component; //list of particle
  vect *offset;

  mat3x3 inertiaInv;
  quat quaternion;
} Granular;

//-------------------------------------
void InitBoxGranular(unsigned index, Granular *gran, const vect pos,
    ParticleIndex startIndex, Particle *startPart, vect* startOffset,
		float bigR, float smallR, float density);
void InitGranularHPlane(unsigned index, Granular *gran, const vect min_pos,
    ParticleIndex startIndex, Particle *startPart, vect* startOffset,
		unsigned l, unsigned w, float radius, float density);
void InitGranularSphere(unsigned index, Granular *gran, const vect pos,
    ParticleIndex startIndex, Particle *startPart, vect* startOffset,
    float radius, float density);
void ComputeGranularForce(Granular *iG, Granular *jG);
void GranApplyBound(Granular *iG);
void GranularTimeIntergration(Granular *, float time_step);
void InitGranularInertia(Granular *);
void GranularPrint(const Granular *gran);
void FreeGranular(Granular *);
#endif
