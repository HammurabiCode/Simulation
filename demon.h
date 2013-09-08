#ifndef DEMON_H_H
#define DEMON_H_H
#include "particle.h"
#include "HashTable.h"

typedef struct
{
  Particle *sand;
  unsigned num;
  HashTable sand_ht;
  float max_radius;
  float time_step;
} Demon;

void InitDemon2Particle(Demon *);
void InitDemonHT(Demon*);
void ComputeForce(Demon*);
void TimeIntergration(Demon *);

#endif
