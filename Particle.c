#include "Particle.h"
#include "coef.h"


void InitParticleA(Particle *par_list, unsigned num)
{
}

void InitParticle(Particle *p, const vect pos, float r)
{
	p->radius = r;
	vectCopy(p->position, pos);
	vectSetZero(p->velocity);
	vectSetZero(p->force);
	return;
}
void PartApplyBound(Particle * ip) {
  const static float xBound = 0;
  const static float yBound = 0;
  const static float zBound = 0;
	float sKesi = 0.0f;
  /*
	sKesi = ip->radius + xBound - ip->position[0]; 
	if(sKesi > 0.0f) {
    float sNorForce = 
      COEF_DAMPING*pow(sKesi, COEF_ALPHA)*ip->velocity[0]+COEF_RESTORATION*pow(sKesi, COEF_BELTA);
    vect vShearVelo;
    vectCopy(vShearVelo, ip->velocity);
    vShearVelo[0] = 0;
    float sShearCoulomb = sNorForce*COEF_COULOMB/vectGetLength(vShearVelo);
    float sShearForce = COEF_KT > sShearCoulomb ? sShearForce : COEF_KT;
    vect vContactForce;
    vectScaleTo(vContactForce, vShearVelo, sShearForce);
    vContactForce[0] += sNorForce;
    vectAdd(ip->force, vContactForce);
  }
	sKesi = ip->radius + yBound - ip->position[1]; 
	if(sKesi > 0.0f) {
    float sNorForce = 
      COEF_DAMPING*pow(sKesi, COEF_ALPHA)*ip->velocity[1]+COEF_RESTORATION*pow(sKesi, COEF_BELTA);
    vect vShearVelo;
    vectCopy(vShearVelo, ip->velocity);
    vShearVelo[1] = 0;
    float sShearCoulomb = sNorForce*COEF_COULOMB/vectGetLength(vShearVelo);
    float sShearForce = COEF_KT > sShearCoulomb ? sShearForce : COEF_KT;
    vect vContactForce;
    vectScaleTo(vContactForce, vShearVelo, sShearForce);
    vContactForce[1] += sNorForce;
    vectAdd(ip->force, vContactForce);
  }
  */
	sKesi = ip->radius + zBound - ip->position[2]; 
	if(sKesi > 0.0f) {
    float sNorForce = 
      COEF_DAMPING*pow(sKesi, COEF_ALPHA)*ip->velocity[2]+COEF_RESTORATION*pow(sKesi, COEF_BELTA);
    vect vShearVelo;
    vectCopy(vShearVelo, ip->velocity);
    vShearVelo[2] = 0;
    float sShearCoulomb = sNorForce*COEF_COULOMB/vectGetLength(vShearVelo);
    float sShearForce = COEF_KT > sShearCoulomb ? sShearForce : COEF_KT;
    vect vContactForce;
    vectScaleTo(vContactForce, vShearVelo, sShearForce);
    vContactForce[2] += sNorForce;
    vectAdd(ip->force, vContactForce);
  }
    /*
  const static float SPRING_COEF = 1;
  const static float DAMPING_COEF = 240;
  const static float KT_COEF = 0;
	sKesi = ip->radius + zBound - ip->position[2]; 
	if(sKesi > 0.0f) {
    vect vRelVelo;
    vectScaleTo(vRelVelo, ip->velocity, -1);
    vectScaleAdd(ip->force, vRelVelo, DAMPING_COEF);
    vect vShearVelo;
    vectCopy(vShearVelo, vRelVelo);
    vShearVelo[2] = 0;
    vectScaleAdd(ip->force, vShearVelo, KT_COEF);
    ip->force[2] += SPRING_COEF*sKesi;
  }
    */
}
void ComputeParticleForce(Particle *ip, Particle *jp) {
	vect vPosNorm;
	vectSubstractTo(vPosNorm, jp->position, ip->position);
	float sKesi = ip->radius + jp->radius - vectNormalize(vPosNorm);
	if(sKesi < 0.0f) return;
	 
	vect vRelVelo;
	vectSubstractTo(vRelVelo, ip->velocity, jp->velocity);
	//print_vect(vRelVelo, "vRelVelo:");
	float sDKesi = vectDotProduct(vRelVelo, vPosNorm);
	//printf("%f ", sDKesi);
	float sNorForce = 
		COEF_DAMPING*pow(sKesi, COEF_ALPHA)*sDKesi+COEF_RESTORATION*pow(sKesi, COEF_BELTA);
	vect vNormVelo, vShearVelo;
	vectScaleTo(vNormVelo, vPosNorm, sDKesi);
	vectSubstractTo(vShearVelo, vRelVelo, vNormVelo);
	float sShearCoulomb = sNorForce*COEF_COULOMB/vectGetLength(vShearVelo);
	float sShearForce = COEF_KT > sShearCoulomb ? sShearForce : COEF_KT;
	vect vContactForce;
	vectScaleTo(vContactForce, vPosNorm, sNorForce);
	vectScaleAdd(vContactForce, vShearVelo, sShearForce);
	vectScaleAdd((ip->force), vContactForce, -1);
	vectScaleAdd((jp->force), vContactForce, 1);
}
