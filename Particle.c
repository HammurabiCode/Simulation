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
  const static float lowBound = 0;
  vect vBoundForce;
  vectSetZero(vBoundForce);
	float sKesi = radius + lowBound - ip->position[2]; 
	if(sKesi > 0.0f) {
    vect vUp;
    vectSetZero(vUp);
    vUp[2] = 1;
  }
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
