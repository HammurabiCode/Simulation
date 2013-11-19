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
  for (GranularIndex ig = 0; ig < dem_ptr->num; ig ++ ) {
    InsertHashTable(&(dem_ptr->sand_ht), dem_ptr->sand+ig);
  }
  //HashTablePrint(&(dem_ptr->sand_ht));
}

//-------------------------------------------------------------------------
void InitDemonSphereBox(Demon *dem_ptr) {
  unsigned xNum = 10;
  unsigned yNum = 10;
  unsigned zNum = 10;
  dem_ptr->time_step  = 0.001f;
  dem_ptr->num        = xNum*yNum*zNum;
  dem_ptr->sand       = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  vect pos;
  vectSetZero(pos);
  float radius        = 0.4f;
  float granDensity   = 2.0f;
  dem_ptr->max_radius = radius;
  GranularIndex ip    = 0;
  vect origin;
  vectSetValue(origin, radius, radius, radius);
  for(unsigned ix = 0; ix < xNum; ix ++)
    for(unsigned iy = 0; iy < yNum; iy ++)
      for(unsigned iz = 0; iz < zNum; iz ++) {
        vectSetValue(pos,
            origin[0] + ix*dem_ptr->max_radius*2,
            origin[1] + iy*dem_ptr->max_radius*2,
            origin[2] + iz*dem_ptr->max_radius*2);
        InitGranularSphere(ip, dem_ptr->sand + ip, pos, radius, granDensity);
				ip ++;
      }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranularBox(Demon *dem_ptr) {
  unsigned xNum = 4;
  unsigned yNum = 4;
  unsigned zNum = 4;
  dem_ptr->time_step  = 0.001f;
  dem_ptr->num        = xNum*yNum*zNum;
  dem_ptr->sand       = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  vect pos;
  vectSetZero(pos);
  float smallRadius   = 0.1f;
  float bigRadius     = 0.2f;
  float granDensity   = 2.0f;
  dem_ptr->max_radius = 2*smallRadius + bigRadius;
  float dist          = ((smallRadius + bigRadius)/sqrt(3.0)+smallRadius)*2;
  GranularIndex ip    = 0;
  vect origin;
  vectSetValue(origin, dem_ptr->max_radius, dem_ptr->max_radius, dem_ptr->max_radius);
  vectSetValue(dem_ptr->vDisplayOffset, -dist/2*xNum, 0, 0);
  for(unsigned ix = 0; ix < xNum; ix ++)
    for(unsigned iy = 0; iy < yNum; iy ++)
      for(unsigned iz = 0; iz < zNum; iz ++) {
        vectSetValue(pos,
            origin[0] + ix*dist,
            origin[1] + iy*dist,
            origin[2] + iz*dist);
        InitGranularBox(ip, dem_ptr->sand + ip, pos, bigRadius, smallRadius, granDensity);
				ip ++;
      }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranOneShear(Demon *dem_ptr) {
  unsigned xNum = 1;
  unsigned yNum = 1;
  unsigned zNum = 1;
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = xNum*yNum*zNum;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  vectSetValue(dem_ptr->vDisplayOffset, -granEdge*(xNum*2), 0, 0);
  vect vMinPos;
  vectSetValue(vMinPos, granEdge, granEdge, granEdge);
  //--------------------------------
  for (unsigned ip = 0; ip < dem_ptr->num; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum))-3, ip/xNum%yNum, ip/(xNum*yNum) + 0);
    vectScale(vOffset, granEdge*2);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->sand+ip, vOffset, boxBigR, boxSmallR, granDensity); 
    vectSetValue(dem_ptr->sand[ip].velocity, 3.18, 0, 0);
  }
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
void InitDemonGranOneNor(Demon *dem_ptr) {
  unsigned xNum = 1;
  unsigned yNum = 1;
  unsigned zNum = 1;
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = xNum*yNum*zNum;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 10.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  vectSetValue(dem_ptr->vDisplayOffset, -granEdge*(xNum*2), 0, 0);
  vect vMinPos;
  vectSetValue(vMinPos, granEdge, granEdge, granEdge);
  //--------------------------------
  for (unsigned ip = 0; ip < dem_ptr->num; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)), ip/xNum%yNum, ip/(xNum*yNum) + 2);
    vectScale(vOffset, granEdge*2);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->sand+ip, vOffset, boxBigR, boxSmallR, granDensity); 
    //vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
  }
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranPile(Demon *dem_ptr) {
  unsigned xNum = 4;
  unsigned yNum = 4;
  unsigned zNum = 4;
  dem_ptr->time_step = 0.0001f;
  dem_ptr->num = xNum*yNum*zNum + 0;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  vectSetValue(dem_ptr->vDisplayOffset, -granEdge*(xNum*2), 0, 0);
  vect vMinPos;
  vectSetValue(vMinPos, granEdge, granEdge, granEdge*2);
  //--------------------------------
  unsigned ip = 0;
  for (; ip < dem_ptr->num - 0; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)), ip/xNum%yNum, ip/(xNum*yNum));
    vectScale(vOffset, granEdge*2);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->sand+ip, vOffset, boxBigR, boxSmallR, granDensity); 
    //vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
  }
  //--------------------------------
  /*
  vect vOffset;
  vectSetValue(vOffset, 1.5, 1.5, 3);
  vectScale(vOffset, granEdge*2);
  vectAdd(vOffset, vMinPos);
  InitGranularBox(ip, dem_ptr->sand+ip, vOffset, boxBigR, boxSmallR, granDensity); 
  ip ++;
  */
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGran2Collide(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  dem_ptr->max_radius = boxBigR;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  vectSetZero(dem_ptr->vDisplayOffset);
  //--------------------------------
  unsigned ip = 0;
  vect pos;
  vectSetZero(pos);
  {
    vectSetValue(pos, -1.0f, 0.0f, 2.0f);
    InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
    vectSetValue(dem_ptr->sand[ip].velocity, 3.14, 0, 0);
    ip++;
  }
  {
    vectSetValue(pos, 1.0f, 0.0f, 2.2f);
    InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
    vectSetValue(dem_ptr->sand[ip].velocity, -3.14, 0, 0);
    ip++;
  }
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranPlane(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 1;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	boxSmallR*2*10, boxSmallR*2*2, granEdge+boxSmallR*10);
	InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranPull(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	boxSmallR*2*10, boxSmallR*2*2, granEdge+boxSmallR*10);
	InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
  unsigned xSize = 100;
  unsigned ySize = 20;
	vectSetValue(pos, 0, -ySize*boxSmallR, 0);
	InitGranularHPlane(ip, dem_ptr->sand+ip, pos, xSize, ySize, boxSmallR, granDensity);
  //--------------------------------
  //InitDemonHT(dem_ptr);
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
	//InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
	InitGranularSphere(ip, dem_ptr->sand+ip, pos, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->sand[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
  unsigned xSize = 100;
  unsigned ySize = 20;
	vectSetValue(pos, 0, -ySize*boxSmallR, 0);
	InitGranularHPlane(ip, dem_ptr->sand+ip, pos, xSize, ySize, boxSmallR, granDensity);
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
	//InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, radius, granDensity); 
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
    InitGranularBox(ip, dem_ptr->sand+ip, pos, boxBigR, boxSmallR, granDensity); 
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
    InsertHashTable(&(dem_ptr->sand_ht), dem_ptr->sand+ip);
  }
}
//-------------------------------------------------------------------------
void ComputeForce(Demon *dem_ptr)
{
  /// For denmon 'gran pull'.
	/*
  for(unsigned ig = 0; ig < dem_ptr->num; ig++) {
    for(unsigned jg = ig+1; jg < dem_ptr->num; jg++) {
      ComputeGranularForce(dem_ptr->sand + ig, dem_ptr->sand + jg);
    }
    GranApplyBound(dem_ptr->sand+ig);
  }
	*/
  const static unsigned MAX_NEI_NUM = 1000;
  GranularIndex *listNei = (GranularIndex*)malloc(sizeof(GranularIndex)*MAX_NEI_NUM);
  unsigned hv[27];
  for(unsigned ig = 0; ig < dem_ptr->num; ig ++) {
    Granular *iGran = dem_ptr->sand + ig;
    unsigned numNei = 0;
    //printf("%03u:", ig);
    for (unsigned ip = 0; ip < iGran->num; ip ++) {
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
            vectAdd(nei_grid, iGran->component[ip].position);
            unsigned cur_hv = HASH_VALUE(nei_grid,
                dem_ptr->sand_ht.min_pos,
                dem_ptr->sand_ht.cell_length,
                dem_ptr->sand_ht.table_size);
            int ih = 0;
            while (hv[ih] != cur_hv && ih < num_hv) {
              ih ++;
            }
            if (ih == num_hv) {
              hv[num_hv++] = cur_hv;
              HashGrid *curHG = &(dem_ptr->sand_ht.content[cur_hv]);
              for(unsigned iIndex = 0; iIndex < curHG->num; iIndex ++) {
                Granular *jGran = dem_ptr->sand + curHG->content[iIndex];
                if (iGran->index >= jGran->index) continue;
                unsigned iNei = 0;
                while (listNei[iNei] != jGran->index && iNei < numNei) {
                  iNei ++;
                }
                if (iNei == numNei) {
                  //compute force between ip & curHG->content[iIndex]
                  //if (vectGetDistance(iGran->position, jGran->position) < 0.20004 + 0.3*2.0/sqrt(3.0)) {
                  if (vectGetDistance(iGran->position, jGran->position) < iGran->radius + jGran->radius) {
                    //printf("***************\n");
                    //printf("%03u, ", jGran->index);
                    ComputeGranularForce(iGran, jGran);
                    listNei[numNei++] = curHG->content[iIndex];
                  }
                }
              }
            }
          }
        }
      }
      GranApplyBound(iGran);
    }
    //printf("\n");
  }
  free(listNei);
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
      obj_name, dem_ptr->vDisplayOffset[0],
      dem_ptr->vDisplayOffset[2],
      dem_ptr->vDisplayOffset[1], 0.0, 0.0, 0.0);
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
