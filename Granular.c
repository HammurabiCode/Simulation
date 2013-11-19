#include "Granular.h"
#include "vect.h" 
#include "mat.h" 
#include "quat.h"
#include "common.h"

//----------------------------------------------------------------
void InitGranularBox(unsigned index, Granular *gran, const vect pos,
    float bigR, float smallR, float density)
{
	gran->index = index;
	gran->density = density;
	gran->mass = PI*4.0*(bigR*bigR*bigR + smallR*smallR*smallR*8)/3.0;
	gran->radius = bigR + 2*smallR;
  //--------------------------------
  //quatenion inertia
  vectCopy((gran->position), pos);
  vectSetZero((gran->velocity));
  vectSetZero((gran->acceleration));
  vectSetZero((gran->angularVelocity));
  vectSetZero((gran->angularMomentum));

  //--------------------------------
  FreeGranular(gran);

  gran->num = 9;
  gran->component = (Particle*)malloc(sizeof(Particle)*gran->num);
  gran->offset = (vect*)malloc(sizeof(vect)*gran->num);
  InitParticle(gran->component, pos, bigR);
	vectSetZero(gran->offset);
  float dist = (bigR + smallR)/sqrt(3.0);
  for(unsigned i = 1; i < 9; i++) {
    int x, y, z;
    x = (((i-1)&0x01) == 0) ? 1 : -1;
    y = (((i-1)&0x02) == 0) ? 1 : -1;
    z = (((i-1)&0x04) == 0) ? 1 : -1;
    vectSetValue((gran->offset+i), x*dist, y*dist, z*dist);
    vect offset;
    vectAddTo(offset, pos, gran->offset[i]);
    InitParticle(gran->component+i, offset, smallR);
  }
  //--------------------------------
	setQuatZero(&(gran->quaternion));
	gran->quaternion[0] = 1.0f;
	InitGranularInertia(gran);
  //--------------------------------
}

//----------------------------------------------------------------
void InitGranularSphere(unsigned index, Granular *gran, const vect pos,
    float radius, float density) {
	if (gran->num != 0) {
		FreeGranular(gran);
	}

	gran->index = index;
	gran->mass = density*PI*4.0*radius*radius*radius/3.0;
	gran->density	= density;
	gran->radius = radius;

	vectCopy(gran->position, pos);
	vectSetZero(gran->velocity);
	vectSetZero(gran->acceleration);
	vectSetZero(gran->angularMomentum);
	vectSetZero(gran->angularVelocity);

	gran->num = 1;
	gran->component = (Particle*) malloc(sizeof(Particle)*gran->num);
	gran->offset = (vect*) malloc(sizeof(vect)*gran->num);
  InitParticle(gran->component+0, pos, radius);
  vectSetValue(gran->offset+0, 0, 0, 0);
  //--------------------------------
	setQuatZero(&(gran->quaternion));
	gran->quaternion[0] = 1.0f;
	InitGranularInertia(gran);
}
//----------------------------------------------------------------
void InitGranularHPlane(unsigned index, Granular *gran, const vect min_pos,
		unsigned l, unsigned w, float radius, float density) {
	if (gran->num != 0) {
		FreeGranular(gran);
	}

	gran->index = index;
	gran->mass = -1.0f;
	gran->density	= density;

	vect centerOffset;
	vectSetValue(centerOffset, (l-1)*radius, (w-1)*radius, 0);
	vectAddTo(gran->position, centerOffset, min_pos);
	vectSetZero(gran->velocity);
	vectSetZero(gran->acceleration);
	vectSetZero(gran->angularMomentum);
	vectSetZero(gran->angularVelocity);

	gran->num = l*w;
	gran->component = (Particle*) malloc(sizeof(Particle)*gran->num);
	gran->offset = (vect*) malloc(sizeof(vect)*gran->num);
	float d = radius*2*0.618;
  for (unsigned ip = 0; ip < gran->num; ip++) {
    vectSetValue(gran->offset+ip, (ip%l)*d, (ip/l)*d, 0);
    vect pos;
    vectAddTo(pos, min_pos, gran->offset+ip);
    InitParticle(gran->component+ip, pos, radius);
    vectSubstractTo(gran->offset[ip], pos, gran->position);
	}
  //--------------------------------
  /*
	setQuatZero(&(gran->quaternion));
	gran->quaternion[0] = 1.0f;
	InitGranularInertia(gran);
  */
}
//-------------------------------------------------------------------------
void ComputeGranularForce(Granular *iG, Granular *jG)
{
	for(unsigned ip = 0; ip < iG->num; ip++) {
    for(unsigned jp = 0; jp < jG->num; jp++) {
      ComputeParticleForce(iG->component+ip, jG->component+jp);
    }
	}
}
void GranApplyBound(Granular *iG)
{
	for(unsigned ip = 0; ip < iG->num; ip++) {
    PartApplyBound(iG->component+ip);
	}
}
void GranularTimeIntergration(Granular *iG, float time_step)
{
	if (iG->mass <= 0.0f) return;
  if (iG->num == 1) {
    vectScaleTo(iG->acceleration, iG->component[0].force, 1.0/iG->mass);
#ifndef ZERO_GRAVITY
    vectAdd(iG->acceleration, GRAVITY);
#endif
    vectScaleAdd(iG->velocity, iG->acceleration, time_step);
    vectScaleAdd(iG->position, iG->velocity, time_step);
    vectCopy(iG->component[0].velocity, iG->velocity);
    UpdateParticlePosition(iG->component+0, time_step);
    vectSetZero(iG->acceleration);
    vectSetZero(iG->component[0].force);
    return;
  }
  vect dAngMom;
  vectSetZero(dAngMom);
  vect sumForce;
  vectSetZero(sumForce);
  for(unsigned ip = 0; ip < iG->num; ip++) {
    vect iAngMom;
    vect iOffset;
    vectSubstractTo(iOffset, iG->component[ip].position, iG->position);
    vectCrossProduct(iAngMom, iOffset, iG->component[ip].force);
    //print_vect(iG->component[ip].force, "");
    vectAdd(dAngMom, iAngMom);
    vectAdd(sumForce, iG->component[ip].force);
    vectSetZero(iG->component[ip].force);
  }
  vectCheckZero(dAngMom);
  vectScaleAdd(iG->angularMomentum, dAngMom, time_step);
  quat dQuat;
  getDeltaQuat(dQuat, iG->angularVelocity, time_step);
  updateQuat(iG->quaternion, dQuat);
  mat3x3 matRot, matRotTran;
  matFromQuat(matRot, iG->quaternion);
  matTranspose(matRotTran, matRot);
  mat3x3 matTemp;
  matMulMat(matTemp, matRot, iG->inertiaInv);
  matMulMat(iG->inertiaInv, matTemp, matRotTran);
  matMulVect(iG->angularVelocity, iG->inertiaInv, iG->angularMomentum);
	vectScaleTo(iG->acceleration, sumForce, 1.0/iG->mass);
#ifndef ZERO_GRAVITY
	vectAdd(iG->acceleration, GRAVITY);
#endif
	vectScaleAdd(iG->velocity, iG->acceleration, time_step);
	vectScaleAdd(iG->position, iG->velocity, time_step);
  for (unsigned ip = 0; ip < iG->num; ip++) {
    vect iOffset;

    //1----
    /*
    matMulVect(iOffset, matRot, iG->offset[ip]);
    vectAddTo(iG->component[ip].position, iG->position, iOffset);

    vectCopy(iG->component[ip].velocity, iG->velocity);
    vectAddCrossProduct(iG->component[ip].velocity,
        iG->angularVelocity, iOffset);
    */
    //2----
    vectCopy(iOffset, iG->offset[ip]);
    vectCopy(iG->component[ip].velocity, iG->velocity);
    vectAddCrossProduct(iG->component[ip].velocity,
        iG->angularVelocity, iOffset);
    UpdateParticlePosition(iG->component+ip, time_step);
    vectSubstractTo(iG->offset[ip], iG->component[ip].position, iG->position);
  }
  /*
  float dd = fabs(iG->offset[2][0]) - fabs(iG->offset[1][0]);
  printf("%e, %e\n", dd, ZERO);
  if (fabs(dd) > ZERO) {
    for (unsigned ip = 0; ip < iG->num; ip++) {
      print_vect(iG->offset[ip], "");
      printf("\n");
    }
  }
  */
	return;
}
void InitGranularInertia(Granular *gran) {
	mat3x3 inertia;
	matSetZero(inertia);
	for(unsigned ip = 0; ip < gran->num; ip ++) {
		const Particle *p = gran->component+ip;
		float m = p->radius*p->radius*p->radius;
		inertia[0][0] += m*(gran->offset[ip][2]*gran->offset[ip][2]
				+ gran->offset[ip][1]*gran->offset[ip][1]);
		inertia[0][1] += -m*gran->offset[ip][0]*gran->offset[ip][1];
		inertia[0][2] += -m*gran->offset[ip][0]*gran->offset[ip][2];

		inertia[1][0] += -m*gran->offset[ip][0]*gran->offset[ip][1];
		inertia[1][1] += m*(gran->offset[ip][0]*gran->offset[ip][0]
				+ gran->offset[ip][2]*gran->offset[ip][2]);
		inertia[1][2] += -m*gran->offset[ip][1]*gran->offset[ip][2];

		inertia[2][0] += -m*gran->offset[ip][0]*gran->offset[ip][2];
		inertia[2][1] += -m*gran->offset[ip][1]*gran->offset[ip][2];
		inertia[2][2] += m*(gran->offset[ip][0]*gran->offset[ip][0]
				+ gran->offset[ip][1]*gran->offset[ip][1]);
	}
	matScale(inertia, 4.0*PI/3.0*gran->density);
	matSetZero(gran->inertiaInv);
	matInv(gran->inertiaInv, inertia);
}
//-------------------------------------------------------------------------
void GranularPrint(const Granular *gran)
{
  //if (vectGetLength(gran->acceleration) < 10.0) return;
  printf("%02u:", gran->index);
  print_vect(gran->position, "Position");
  printf("\t");
  print_vect(gran->velocity, "Velocity");
  printf("\t");
  print_vect(gran->acceleration, "Acce");
  printf("\t");
  print_vect(gran->angularVelocity, "Angular");
  printf("\t");
  print_vect(gran->angularMomentum, "AngularMom");
  printf("\n");
  for (unsigned ip = 0; ip < gran->num; ip++) {
    printf("%02u:", ip);
    ParticlePrint(gran->component+ip);
  }
}
void FreeGranular(Granular *gran)
{
	if(gran->num > 0 && gran->component != NULL)
	{
		free(gran->component);
		free(gran->offset);
		gran->num = 0;
	}
}
