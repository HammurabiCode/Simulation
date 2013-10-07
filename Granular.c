#include "Granular.h"
#include "vect.h" 
#include "mat.h" 
#include "quat.h"

//-------------------------------------------------------------------------
void InitBoxGranular(Granular *gran, const vect pos,
		float bigR, float smallR, float density)
{
  //--------------------------------
  //quatenion inertia
  copyVect((gran->position), pos);
  setVectZero((gran->acceleration));
  setVectZero((gran->angularMomentum));

  //--------------------------------
  if(gran->component != NULL)
  {
    free(gran->component);
  }
  gran->num = 9;
  gran->component = (Particle*)malloc(sizeof(Particle)*gran->num);
  gran->offset = (vect*)malloc(sizeof(vect)*gran->num);
  //--------------------------------
  InitParticle(gran->component, pos, bigR);
	setVectZero((gran->offset));
  float dist = bigR + smallR;
  for(unsigned i = 1; i < 9; i++)
  {
    int x, y, z;
    x = ((i&0x01) == 0) ? 1 : -1;
    y = ((i&0x02) == 0) ? 1 : -1;
    z = ((i&0x04) == 0) ? 1 : -1;
    setVectValue((gran->offset+i), x*dist, y*dist, z*dist);
    vect offset;
    addVectTo(offset, pos, gran->offset[i]);
    InitParticle(gran->component+i+1, offset, smallR);
  }
}

void ComputeGranularForce(Granular *iG, Granular *jG)
{
	for(unsigned ip = 0; ip < iG->num; ip++) {
		for(unsigned jp = 0; jp < jG->num; jp ++) {
			ComputeParticleForce(iG->component + ip, jG->component + jp);
		}
	}
}
//-------------------------------------------------------------------------
void GranularTimeIntergration(Granular *iG, float time_step)
{
  vect dAngMom;
  setVectZero(dAngMom);
  vect sumForce;
  setVectZero(sumForce);
  for(unsigned ip = 0; ip < iG->num; ip++) {
    vect iAngMom;
    crossProductVect(iAngMom, iG->offset[ip], iG->component[ip].force);
    addVect(dAngMom, iAngMom);
    addVect(sumForce, iG->component[ip].force);
  }
  scaleAddVect(iG->angularMomentum, dAngMom, time_step);
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
  for(unsigned ip = 0; ip < iG->num; ip++) {
    scaleAddVect(iG->velocity, sumForce, time_step/iG->mass);
    copyVect(iG->velocity, iG->component[ip].velocity);
    addCrossProductVect(iG->component[ip].velocity,
        iG->angularVelocity, iG->offset[ip]);
    UpdateParticlePosition(iG->component+ip, time_step);
  }
	return;
}
//-------------------------------------------------------------------------
void FreeGranular(Granular *gran)
{
	if(gran->num > 0 && gran->component != NULL)
	{
		free(gran->component);
		free(gran->offset);
		gran->num = 0;
	}
}
