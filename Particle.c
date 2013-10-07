#include "Particle.h"
#include "coef.h"


void InitParticleA(Particle *par_list, unsigned num)
{
}

void InitParticle(Particle *p, const vect *pos, float r)
{
	p->radius = r;
	copyVect((p->position), pos);
	setVectZero((p->velocity));
	setVectZero((p->force));
	return;
}
void ComputeParticleForce(Particle *ip, Particle *jp)
{
	vect vPosNorm;
	substractVectTo(vPosNorm, jp->position, ip->position);
	float sKesi = ip->radius + jp->radius - normalizeVect(vPosNorm);
	if(sKesi < 0.0f) return;
	 
	vect vRelVelo;
	substractVectTo(vRelVelo, ip->velocity, jp->velocity);
	//print_vect(vRelVelo, "vRelVelo:");
	float sDKesi = dotProductVect(vRelVelo, vPosNorm);
	//printf("%f ", sDKesi);
	float sNorForce = 
		COEF_DAMPING*pow(sKesi, COEF_ALPHA)*sDKesi+COEF_RESTORATION*pow(sKesi, COEF_BELTA);
	vect vNormVelo, vShearVelo;
	scaleVectTo(vNormVelo, vPosNorm, sDKesi);
	substractVectTo(vShearVelo, vRelVelo, vNormVelo);
	float sShearCoulomb = sNorForce*COEF_COULOMB/getVectLength(vShearVelo);
	float sShearForce = COEF_KT > sShearCoulomb ? sShearForce : COEF_KT;
	vect vContactForce;
	scaleVectTo(vContactForce, vPosNorm, sNorForce);
	scaleAddVect(vContactForce, vShearVelo, sShearForce);
	scaleAddVect((ip->force), vContactForce, -1);
	scaleAddVect((jp->force), vContactForce, 1);
}
