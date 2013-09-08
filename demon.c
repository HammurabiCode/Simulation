#include "demon.h"
#include "vect.h"
#include "HashTable.h"


void InitDemon2Particle(Demon *dem_ptr)
{
  dem_ptr->num = 2;
  dem_ptr->max_radius = 1.1f;
  dem_ptr->sand = (Particle *)malloc(sizeof(Particle)*dem_ptr->num);
  InitParticleA(dem_ptr->sand, dem_ptr->num);
}

void InitDemonHT(Demon *dem_ptr)
{
  vect origin_point;
  setValue(&origin_point, 0.0f, 0.0f, 0.0f);
  InitHashTable(&sand_ht, dem_ptr->num*2, &origin_point, max_radius*2);
}

void TimeIntergration(Demon *dem_ptr)
{
  vect temp;
  for(unsigned ip = 0; ip < PART_NUM; ip ++)
  {
    setValue(&(temp), 0.0f, 0.0f, 0.0f);
    scaleAdd(&temp, &(dem_ptr->sand[ip].acceleration), time_step);
    add2(&(dem_ptr->sand[ip].velocity), &temp);
    scaleAdd(&(dem_ptr->sand[ip].position), &(dem_ptr->sand[ip].velocity), time_step);
  }
}

void ComputeForce(Demo *dem_ptr)
{
  for(unsigned ip = 0; ip < dem_ptr->num; ip ++)
  {
    //compute force
  }
}
