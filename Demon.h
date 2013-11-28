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
  vect vDisplayOffset;
  vect vGravity;
} Demon;

void InitDemonHT(Demon*);

void InitDemonCube(Demon*);
void InitDemonBox(Demon*, const vect, unsigned, unsigned, unsigned);

void InitDemonPush(Demon *dem_ptr);
void InitDemonThrow(Demon *dem_ptr);
void InitDemonInterCubeGran(Demon *dem_ptr);
void InitDemonGranPull(Demon *dem_ptr);
void InitDemonGranPlane(Demon *dem_ptr);
void InitDemonGranOneShear(Demon *dem_ptr);
void InitDemonGranOneNor(Demon *dem_ptr);
void InitDemonGranPile(Demon *dem_ptr);
void InitDemonGran2Collide(Demon *dem_ptr);
void InitDemonSphereBox(Demon *dem_ptr);
void InitDemonGranularBox(Demon *dem_ptr);

void ComputeForce(Demon*);
void TimeIntergration(Demon *);
void SaveDemon(const Demon * , FILE *);
void FreeDemon(Demon *);

enum {
  INIT_CUBE,
  INIT_PUSH_SPHERE,
  INIT_INTER_CUBE_GRAN,
  INIT_THROW_SPHERE,
  INIT_GRAN_PULL,
  INIT_GRAN_PLANE,
  INIT_GRAN_PILE,
  INIT_GRAN_ONE_SHEAR,
  INIT_GRAN_ONE_NOR,
  INIT_GRAN_2_COLLIDE,
  INIT_SPHE_BOX,
  INIT_GRAN_BOX,
  INIT_FUNC_NUM
};
typedef void (*InitDemonFunc)(Demon *);
const static InitDemonFunc initList[INIT_FUNC_NUM] = {
  InitDemonCube,
  InitDemonPush,
  InitDemonInterCubeGran,
  InitDemonThrow,
  InitDemonGranPull,
  InitDemonGranPlane,
  InitDemonGranPile,
  InitDemonGranOneShear,
  InitDemonGranOneNor,
  InitDemonGran2Collide,
  InitDemonSphereBox,
  InitDemonGranularBox
};
const static char filenameList[INIT_FUNC_NUM][128] = {
  "demon-cube",
  "demon-push",
  "demon-inter-cube-gran",
  "demon-throw",
  "gran-pull",
  "gran-plane",
  "gran-pile",
  "gran-one-shear",
  "gran-one-nor",
  "gran-2-collide",
  "sphere-box",
  "granular-box"
};
#endif
