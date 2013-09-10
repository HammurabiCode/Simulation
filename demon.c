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
  InitHashTable(&(dem_ptr->sand_ht), dem_ptr->num*2, &origin_point, dem_ptr->max_radius*2);
}

void TimeIntergration(Demon *dem_ptr)
{
  ClearHashTable(&(dem_ptr->sand_ht));
  vect temp;
  for(unsigned ip = 0; ip < PART_NUM; ip ++)
  {
    setValue(&(temp), 0.0f, 0.0f, 0.0f);
    scaleAdd(&temp, &(dem_ptr->sand[ip].acceleration), time_step);
    add2(&(dem_ptr->sand[ip].velocity), &temp);
    scaleAdd(&(dem_ptr->sand[ip].position), &(dem_ptr->sand[ip].velocity), time_step);
    InsertHashTable(&(dem_ptr->sand_ht), dem_ptr->sand+ip);
  }
}

void ComputeForce(Demo *dem_ptr)
{
  for(unsigned ip = 0; ip < dem_ptr->num; ip ++)
  {
    Particle *cur_p = dem_ptr->sand + ip;
    unsigned hv[27];
    unsigned num_hv = 0;
    vect nei_grid;
    for(int ix = -1; ix <= 1; ix ++)
    {
      for(int iy = -1; iy <= 1; iy ++)
      {
        for(int iz = -1; iz <= 1; iz ++)
        {
          setValue(&nei_grid, ix*dem_ptr->sand_ht->cell_length,
            iy*dem_ptr->sand_ht->cell_length,
            iz*dem_ptr->sand_ht->cell_length)
          substractTo(&nei_grid, &(cur_p->position), &nei_grid);
          unsigned cur_hv = HASH_VALUE(nei_grid, dem_ptr->sand_ht->min_pos, dem_ptr->sand_ht->cell_length, dem_ptr->sand_ht->table_size);
          for (int ih = 0; ih < num_hv; ih ++)
          {
            /////////todo :
          }
        }
      }
    }
    //contact
    //compute force
    //
    //
    
    Particle *a, *b;
    {
      float nor_force = 0.0f;
      float t_force = 0.0f;
      vect nor;
      substract(&nor, &(b->position), &(a->position));
      float dist = normalize(&nor);
       
      vect rel_velo;
      substract(&rel_velo, &(a->velocity), &(b->velocity));
      //print_vect(&rel_velo, "rel_velo:");
      float kesi = a->radius + b->radius - dist;
      float d_kesi = dotProduct(&rel_velo, &nor);
      //printf("%f ", d_kesi);
      if(kesi < 0.0f)
      {
        nor_force = 0.0f;
        t_force = 0.0f;
      }
      else
      {
        nor_force = COEF_DAMPING*pow(kesi, COEF_ALPHA)*d_kesi+COEF_RESTORATION*pow(kesi, COEF_BELTA);
        float t_coulomb = nor_force*COEF_COULOMB;
        vect nor_velo, t_velo;
        copyVect(&nor_force, &nor);
        scale(&nor_force, d_kesi);
        substract(&t_velo, &rel_velo, &nor_velo);
        float t_velo_f = COEF_KT*normalize(&t_velo);
        t_force = min(t_velo_f, t_coulomb);
        scale(&t_velo, t_force);
        scale(&nor, -nor_force);
        add(&(a->acceleration), &nor, &t_velo);
        add(&(b->acceleration), &t_velo, &nor);
      }
    }
  }
}
