#include "particle.h"

void InitParticleA(Particle *par_list, unsigned num)
{
  float;
  for(unsigned ip = 0; ip < num; ip ++)
  {
    par_list[ip].mass = 1.0f;
    par_list[ip].radius = 1.0f;
    par_list[ip].density = 1.0f;
    setValue(&(par_list[ip].acceleration), 0.0f, 0.0f, 0.0f);
    par_list[ip].index = ip;
  }
  setValue(&(par_list[0].position), 0.0f, 0.0f, 0.0f);
  setValue(&(par_list[0].velocity), 1.0f, 0.0f, 0.0f);
  setValue(&(par_list[1].position), 2.2f, 0.0f, 0.0f);
  setValue(&(par_list[1].velocity), -1.0f, 0.0f, 0.0f);
}
