#ifndef GRANULAR_H_H
#define GRANULAR_H_H

#include "Particle.h"
#include "common.h"
#include "vect.h"
#include "mat.h"
#include "quat.h"

typedef struct
{
	unsigned index;
	//ParticleIndex iStart;
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
void InitGranularBox(unsigned index, Granular *gran, const vect pos,
		float bigR, float smallR, float density);
void InitCubeGranular(unsigned index, Granular *gran, const vect pos,
		float r, float density);
void InitCubeRigid(unsigned index, Granular *gran, const vect pos,
		float r, float density, unsigned xNum, unsigned yNum, unsigned zNum);
void InitGranularHPlane(unsigned index, Granular *gran, const vect min_pos,
		unsigned l, unsigned w, float radius, float density);
void InitGranularSphere(unsigned index, Granular *gran, const vect pos,
    float radius, float density);
void ComputeGranularForce(Granular *iG, Granular *jG);
void GranApplyBound(Granular *iG);
void GranularTimeIntergration(Granular *, float time_step, const vect g);
void InitGranularInertia(Granular *);
void GranularPrint(const Granular *gran);
void FreeGranular(Granular *);
#endif
