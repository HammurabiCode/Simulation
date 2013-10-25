#ifndef DEMON_H_H
#define DEMON_H_H
#include "HashTable.h"
#include "Granular.h"

typedef struct
{
  float time_step;
  unsigned num;
  Granular *sand;
  float max_radius;
  HashTable sand_ht;
} Demon;

void InitDemonHT(Demon*);

void InitDemon2Gran(Demon*);
void InitDemonBox(Demon*, const vect, unsigned, unsigned, unsigned);

void InitDemonPush(Demon *dem_ptr);
void InitDemonThrow(Demon *dem_ptr);
void InitDemonFall(Demon *dem_ptr);

void ComputeForce(Demon*);
void TimeIntergration(Demon *);
void SaveDemon(const Demon * , FILE *);
void FreeDemon(Demon *);

#endif
