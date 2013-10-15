#ifndef DEMON_H_H
#define DEMON_H_H
#include "HashTable.h"
#include "Granular.h"

typedef struct
{
  Granular *sand;
  unsigned num;
  HashTable sand_ht;
  float max_radius;
  float time_step;
} Demon;

void InitDemonHT(Demon*);
void InitDemon2Gran(Demon*);
void InitDemonBox(Demon*, const vect, unsigned, unsigned, unsigned);
void ComputeForce(Demon*);
void TimeIntergration(Demon *);
void SaveDemon(const Demon * , FILE *);
void FreeDemon(Demon *);

#endif
