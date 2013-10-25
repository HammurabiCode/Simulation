#include <stdio.h>
#include "Demon.h"
#include "vect.h"
#include "HashTable.h"
#include "Granular.h"

void InitDemonHT(Demon *dem_ptr)
{
  dem_ptr->sand_ht.content = NULL;
  dem_ptr->sand_ht.table_size = 0;
  vect origin_point;
  vectSetZero(origin_point);
  InitHashTable(&(dem_ptr->sand_ht),
	  dem_ptr->num*2, origin_point, dem_ptr->max_radius*2);
}

//-------------------------------------------------------------------------
void InitDemonBox(Demon *dem_ptr, const vect origin,
    unsigned x, unsigned y, unsigned z) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = x*y*z;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  //dem_ptr->max_radius= boxBigR + boxSmallR*2;
  dem_ptr->max_radius= ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  unsigned ip = 0;
  for(unsigned ix = 0; ix < x; ix ++)
    for(unsigned iy = 0; iy < y; iy ++)
      for(unsigned iz = 0; iz < z; iz ++) {
        vectSetValue(pos,
            origin[0] + ix*dem_ptr->max_radius*2,
            origin[1] + iy*dem_ptr->max_radius*2,
            origin[2] + iz*dem_ptr->max_radius*2);
        InitBoxGranular(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
				ip ++;
      }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonPush(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  //--------------------------------
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  dem_ptr->max_radius= ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  //--------------------------------
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	0, boxSmallR*2*2, boxSmallR*2);
	//vectSetValue(pos,	0, 0,	2.0f);
	//InitBoxGranular(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
	InitGranularSphere(ip, dem_ptr->sand+ip, pos, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
	vectSetValue(pos, 0, 0, 0);
	InitGranularHPlane(ip, dem_ptr->sand+ip, pos, 100, 5, boxSmallR, granDensity);
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonThrow(Demon *dem_ptr)
{
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  //--------------------------------
  vect pos;
  vectSetZero(pos);
  float radius = 0.2f;
  float granDensity= 2.0f;
  dem_ptr->max_radius= radius;
  //--------------------------------
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	0, radius*4,	radius*8);
	InitGranularSphere(ip, dem_ptr->sand+ip, pos, radius, granDensity); 
	vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
	vectSetValue(pos, 0, 0, 0);
	InitGranularHPlane(ip, dem_ptr->sand+ip, pos, 100, 5, radius, granDensity);
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonFall(Demon *dem_ptr)
{
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  //--------------------------------
  vect pos;
  vectSetZero(pos);
  float radius = 0.2f;
  float granDensity= 2.0f;
  dem_ptr->max_radius= radius;
  //--------------------------------
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	radius*2, radius*2,	radius*10.0f);
	//vectSetValue(pos,	0, 0,	2.0f);
	//InitBoxGranular(ip, dem_ptr->sand+ip, pos, boxBigR, radius, granDensity); 
	InitGranularSphere(ip, dem_ptr->sand+ip, pos, radius, granDensity); 
	ip++;
  //--------------------------------
	vectSetValue(pos, 0, -radius*5, 0);
	InitGranularHPlane(ip, dem_ptr->sand+ip, pos, 100, 10, radius, granDensity);
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemon2Gran(Demon *dem_ptr)
{
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  //dem_ptr->max_radius= boxBigR + boxSmallR*2;
  dem_ptr->max_radius= ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  for(unsigned ip = 0; ip < dem_ptr->num; ip ++)
  {
    vectSetValue(pos,
        1.0f,
        (ip+1)*dem_ptr->max_radius*2,
        1.0f);
    InitBoxGranular(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
  }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void TimeIntergration(Demon *dem_ptr)
{
  ClearHashTable(&(dem_ptr->sand_ht));
  vect temp;
  for(unsigned ip = 0; ip < dem_ptr->num; ip ++)
  {
    GranularTimeIntergration(dem_ptr->sand+ip, dem_ptr->time_step);
    //InsertHashTable(&(dem_ptr->sand_ht), dem_ptr->sand+ip);
  }
}
//-------------------------------------------------------------------------
void ComputeForce(Demon *dem_ptr)
{
  for(unsigned ig = 0; ig < dem_ptr->num; ig++) {
    for(unsigned jg = ig+1; jg < dem_ptr->num; jg++) {
      ComputeGranularForce(dem_ptr->sand + ig, dem_ptr->sand + jg);
    }
  }
	return;
	/*
	*/
  for(unsigned ip = 0; ip < dem_ptr->num; ip ++)
  {
    Granular *cur_g = dem_ptr->sand + ip;
    unsigned hv[27];
    unsigned num_hv = 0;
    vect nei_grid;
    for(int ix = -1; ix <= 1; ix ++)
    {
      for(int iy = -1; iy <= 1; iy ++)
      {
        for(int iz = -1; iz <= 1; iz ++)
        {
          vectSetValue(nei_grid,
						ix*dem_ptr->sand_ht.cell_length,
            iy*dem_ptr->sand_ht.cell_length,
            iz*dem_ptr->sand_ht.cell_length);
          vectAddTo(nei_grid, (cur_g->position), nei_grid);
          unsigned cur_hv = HASH_VALUE(nei_grid,
							dem_ptr->sand_ht.min_pos,
							dem_ptr->sand_ht.cell_length,
							dem_ptr->sand_ht.table_size);
          int ih = 0;
          for (; ih < num_hv; ih ++) {
            if(hv[ih] == cur_hv) {
              break;
            }
          }
          if(ih == num_hv) {
            hv[num_hv++] = cur_hv;
          }
        }
      }
    }
    for (unsigned ih = 0; ih < num_hv; ih ++) {
			HashGrid *curHG = &(dem_ptr->sand_ht.content[hv[ih]]);
			for(unsigned iIndex = 0; iIndex < curHG->num; iIndex ++) {
				if(ip < curHG->content[iIndex]) {
					//compute force between ip & curHG->content[iIndex]
					ComputeGranularForce(dem_ptr->sand + ip, dem_ptr->sand + curHG->content[iIndex]);
				}
			}
    }
  }
}
//-------------------------------------------------------------------------
void SaveDemon(const Demon *dem_ptr, FILE *fp)
{
  char obj_name[] = "sand";
  fprintf(fp, "#declare %s = union {\n\t", obj_name);
  for(unsigned i = 0; i < dem_ptr->num; i++) {
    for(unsigned j = 0; j < dem_ptr->sand[i].num; j++) {
      const Particle *p = &(dem_ptr->sand[i].component[j]);
      fprintf(fp, "sphere {\n\t<%f, %f, %f>, %f\n\ttexture {pigment{color Brown}}\n}\n",
          p->position[0], p->position[2], p->position[1], p->radius);
    }
  }
  fprintf(fp, "}\n");
  fprintf(fp, "object {\n\t%s\n\ttranslate <%f, %f, %f>\n\trotate <%f, %f, %f>\n}\n",
      obj_name, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}
//-------------------------------------------------------------------------
void FreeDemon(Demon *dem_ptr) {
	if(dem_ptr == NULL) return;
	FreeHashTable(&(dem_ptr->sand_ht));
	for(unsigned is = 0; is < dem_ptr->num; is ++) {
		FreeGranular(&(dem_ptr->sand[is]));
	}
	free(dem_ptr->sand);
}
