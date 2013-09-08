#include <math.h>
#include <stdio.h>
#include "vect.h"
#include "coef.h"
#include "particle.h"
/*
float GetKr(const Particle *p1, const Particle *p2)
{
  float result = 0.0f;
  result = sqrt(1.0/p1->radius + 1.0/p2->radius);
  result *= (1-p1->poission*p1->poission)/p1->young
    + (1-p2->poission*p2->poission)/p2->young;
  return 0.75f/ result;
}
*/

float compute_normal_force(const Particle *a, const Particle *b)
{
  float result = 0.0f;
  vect nor;
  substract(&nor, &(b->position), &(a->position));
  float dist = normalize(&nor);
   
  //print_vect(&nor, "nor:");
  vect delta_v;
  substract(&delta_v, &(a->velocity), &(b->velocity));
  print_vect(&delta_v, "delta_v:");
  float kesi = a->radius + b->radius - dist;
  float d_kesi = dotProduct(&delta_v, &nor);
  printf("%f ", d_kesi);
  if(kesi < 0.0f) return 0.0f;
  result = COEF_DAMPING*pow(kesi, COEF_ALPHA)*d_kesi+COEF_RESTORATION*pow(kesi, COEF_BELTA);
  return result;
}

const static unsigned PART_NUM = 2;

void init_particle(Particle *par_list)
{
  for(unsigned ip = 0; ip < PART_NUM; ip ++)
  {
    par_list[ip].mass = 1.0f;
    par_list[ip].radius = 1.0f;
    par_list[ip].density = 1.0f;
    setValue(&(par_list[ip].acceleration), 0.0f, 0.0f, 0.0f);
  }
  setValue(&(par_list[0].position), 0.0f, 0.0f, 0.0f);
  setValue(&(par_list[0].velocity), 1.0f, 0.0f, 0.0f);
  setValue(&(par_list[1].position), 2.2f, 0.0f, 0.0f);
  setValue(&(par_list[1].velocity), -1.0f, 0.0f, 0.0f);
}

void time_intergration(Particle *par_list, float time_step)
{
  vect temp, temp2;
  for(unsigned ip = 0; ip < PART_NUM; ip ++)
  {
    setValue(&(temp), 0.0f, 0.0f, 0.0f);
    scaleAdd(&temp, &(par_list[ip].acceleration), time_step);
    //print_vect(&(temp), "");
    add2(&(par_list[ip].velocity), &temp);
    scaleAdd(&(par_list[ip].position), &(par_list[ip].velocity), time_step);
  }
}

int main(int argc, char *argv[])
{
  Particle *par_list = (Particle*) malloc(sizeof(Particle)*PART_NUM);
  init_particle(par_list);
  for(unsigned iframe = 0; iframe < 20; iframe ++)
  {
    float dist = getDistance(&(par_list[0].position), &(par_list[1].position));
    printf("%u:%f ", iframe, dist);
    if(dist <= par_list[0].radius + par_list[1].radius)
        {
          float nor_f = compute_normal_force(&(par_list[0]), &(par_list[1]));
          printf("%f\n", nor_f);
          setValue(&(par_list[1].acceleration), nor_f, 0.0f, 0.0f);
          setValue(&(par_list[0].acceleration), -nor_f, 0.0f, 0.0f);
        }
    time_intergration(par_list, 0.01f);
    for(unsigned ip = 0; ip < PART_NUM; ip ++)
    {
      print_vect(&(par_list[ip].position), "");
    }
    printf("\n");
  }
  
  if(par_list != NULL)
  {
    free(par_list);
    par_list = NULL;
  }
  return 0;
}
