#include "Granular.h"
#include "vect.h" 
#include "mat.h" 
#include "quat.h"

//----------------------------------------------------------------
void InitBoxGranular(unsigned index, Granular *gran, const vect pos,
		float bigR, float smallR, float density)
{
	gran->index = index;
	gran->density = density;
	gran->mass = PI*4.0*(bigR*bigR*bigR + smallR*smallR*smallR*8)/3.0;
  //--------------------------------
  //quatenion inertia
  copyVect((gran->position), pos);
  setVectZero((gran->velocity));
  setVectZero((gran->acceleration));
  setVectZero((gran->angularVelocity));
  setVectZero((gran->angularMomentum));

  //--------------------------------
  if(gran->component != NULL)
  {
    free(gran->component);
  }
  gran->num = 9;
  gran->component = (Particle*)malloc(sizeof(Particle)*gran->num);
  gran->offset = (vect*)malloc(sizeof(vect)*gran->num);
  InitParticle(gran->component, pos, bigR);
	setVectZero(gran->offset);
  float dist = (bigR + smallR)/sqrt(3.0);
  for(unsigned i = 1; i < 9; i++) {
    int x, y, z;
    x = (((i-1)&0x01) == 0) ? 1 : -1;
    y = (((i-1)&0x02) == 0) ? 1 : -1;
    z = (((i-1)&0x04) == 0) ? 1 : -1;
    setVectValue((gran->offset+i), x*dist, y*dist, z*dist);
    vect offset;
    addVectTo(offset, pos, gran->offset[i]);
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

	copyVect(gran->position, pos);
	setVectZero(gran->velocity);
	setVectZero(gran->acceleration);
	setVectZero(gran->angularMomentum);
	setVectZero(gran->angularVelocity);

	gran->num = 1;
	gran->component = (Particle*) malloc(sizeof(Particle)*gran->num);
	gran->offset = (vect*) malloc(sizeof(vect)*gran->num);
  InitParticle(gran->component+0, pos, radius);
  setVectValue(gran->offset+0, 0, 0, 0);
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
	setVectValue(centerOffset, (l-1)*radius, (w-1)*radius, 0);
	addVectTo(gran->position, centerOffset, min_pos);
	setVectZero(gran->velocity);
	setVectZero(gran->acceleration);
	setVectZero(gran->angularMomentum);
	setVectZero(gran->angularVelocity);

	gran->num = l*w;
	gran->component = (Particle*) malloc(sizeof(Particle)*gran->num);
	gran->offset = (vect*) malloc(sizeof(vect)*gran->num);
	float d = radius*2;
	unsigned ip = 0;
  for (unsigned x = 0; x < l; x++) {
		for (unsigned y = 0; y < w; y++) {
			setVectValue(gran->offset+ip, x*d, y*d, 0);
			vect pos;
			addVectTo(pos, min_pos, gran->offset+ip);
			InitParticle(gran->component+ip, pos, radius);
			ip ++;
		}
	}
  //--------------------------------
	setQuatZero(&(gran->quaternion));
	gran->quaternion[0] = 1.0f;
	InitGranularInertia(gran);
}
//-------------------------------------------------------------------------
void ComputeGranularForce(Granular *iG, Granular *jG)
{
  unsigned i = 0;
  char name[4];
	for(unsigned ip = 0; ip < iG->num; ip++) {
		for(unsigned jp = 0; jp < jG->num; jp ++) {
      sprintf(name, "%02u", i++);
      print_vect(iG->component[ip].force, name); 
			ComputeParticleForce(iG->component + ip, jG->component + jp);
      printf("\n");
		}
	}
}
void GranularTimeIntergration(Granular *iG, float time_step)
{
	if(iG->mass <= 0.0f) return;
  vect dAngMom;
  setVectZero(dAngMom);
  vect sumForce;
  setVectZero(sumForce);
  for(unsigned ip = 0; ip < iG->num; ip++) {
    vect iAngMom;
    crossProductVect(iAngMom, iG->offset[ip], iG->component[ip].force);
    //print_vect(iG->component[ip].force, "");
    addVect(dAngMom, iAngMom);
    addVect(sumForce, iG->component[ip].force);
    setVectZero(iG->component[ip].force);
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
  //print_vect(sumForce, "");
	scaleAddVect(iG->acceleration, sumForce, 1.0/iG->mass);
	addVect(iG->acceleration, GRAVITY);
	scaleAddVect(iG->velocity, iG->acceleration, time_step);
  for(unsigned ip = 0; ip < iG->num; ip++) {
    copyVect(iG->component[ip].velocity, iG->velocity);
    addCrossProductVect(iG->component[ip].velocity,
        iG->angularVelocity, iG->offset[ip]);
    UpdateParticlePosition(iG->component+ip, time_step);
  }
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
	matInv(gran->inertiaInv, inertia);
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
