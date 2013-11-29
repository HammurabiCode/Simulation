#include <stdio.h>
#include "Demon.h"
#include "vect.h"
#include "HashTable.h"
#include "Granular.h"

const static vect GRAVITY = {0, 0, -9.8};

void InitDemonHT(Demon *dem_ptr)
{
  dem_ptr->htDRigid.content = NULL;
  dem_ptr->htDRigid.table_size = 0;
  vect origin_point;
  vectSetZero(origin_point);
  InitHashTable(&(dem_ptr->htDRigid),
	  dem_ptr->numDRigid*2, origin_point, dem_ptr->max_radius*2);
  for (GranularIndex ig = 0; ig < dem_ptr->numDRigid; ig ++ ) {
    InsertHashTable(&(dem_ptr->htDRigid), dem_ptr->listDRigid+ig);
  }
  //HashTablePrint(&(dem_ptr->htDRigid));
}

//-------------------------------------------------------------------------
void InitDemonSphereBox(Demon *dem_ptr) {
  unsigned xNum = 10;
  unsigned yNum = 10;
  unsigned zNum = 10;
  dem_ptr->time_step  = 0.00001f;
  dem_ptr->numDRigid        = xNum*yNum*zNum;
  dem_ptr->listDRigid       = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
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
        InitGranularSphere(ip, dem_ptr->listDRigid + ip, pos, radius, granDensity);
				ip ++;
      }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranularBox(Demon *dem_ptr) {
  unsigned xNum = 10;
  unsigned yNum = 10;
  unsigned zNum = 10;
  float smallRadius   = 0.08f;
  float bigRadius     = 0.2f;
  float granDensity   = 10.0f;
  float dist          = ((smallRadius + bigRadius)/sqrt(3.0)+smallRadius)*2;

  dem_ptr->time_step  = 0.00001f;
  dem_ptr->numDRigid        = xNum*yNum*zNum;
  dem_ptr->listDRigid       = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  dem_ptr->max_radius = 2*smallRadius + bigRadius;
  vectCopy(dem_ptr->vGravity, GRAVITY);
  vectSetValue(dem_ptr->vDisplayOffset, -dist/2.0f*xNum, 0, 0);
  
  vect vMinPos;
  vectSetValue(vMinPos, dist/2.0f, dist/2.0f, dist/2.0f);
  //--------------------------------
  for (unsigned ip = 0; ip < dem_ptr->numDRigid; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)), ip/xNum%yNum, ip/(xNum*yNum) + 2);
    if (ip/(xNum*yNum)%2 == 1) {
      vOffset[0] += 0.25;
    } else {
      vOffset[0] -= 0.25;
    }
    vectScale(vOffset, dist);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, vOffset, bigRadius, smallRadius, granDensity); 
  }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranOneShear(Demon *dem_ptr) {
  unsigned xNum = 1;
  unsigned yNum = 1;
  unsigned zNum = 1;
  dem_ptr->time_step = 0.0001f;
  dem_ptr->numDRigid = xNum*yNum*zNum;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  
  float boxBigR = 0.4f;
  float boxSmallR = 0.15f;
  float granDensity= 10.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);

  vectCopy(dem_ptr->vGravity, GRAVITY);

  vectSetValue(dem_ptr->vDisplayOffset, -granEdge*(xNum*2), 0, 0);
  vect vMinPos;
  vectSetValue(vMinPos, granEdge, granEdge, granEdge);
  //--------------------------------
  for (unsigned ip = 0; ip < dem_ptr->numDRigid; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)) + 6, ip/xNum%yNum, ip/(xNum*yNum));
    vectScale(vOffset, granEdge*2);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, vOffset, boxBigR, boxSmallR, granDensity); 
    vectSetValue(dem_ptr->listDRigid[ip].velocity, -10.0, 0, 0);
  }
  //--------------------------------
  InitDemonHT(dem_ptr);
}

//-------------------------------------------------------------------------
void InitDemonGranOneNor(Demon *dem_ptr) {
  unsigned xNum = 1;
  unsigned yNum = 1;
  unsigned zNum = 1;
  dem_ptr->time_step = 0.0001f;
  dem_ptr->numDRigid = xNum*yNum*zNum;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  
  vectCopy(dem_ptr->vGravity, GRAVITY);

  float boxBigR = 0.4f;
  float boxSmallR = 0.15f;
  float granDensity= 10.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  vectSetValue(dem_ptr->vDisplayOffset, -granEdge*(xNum*2), 0, 0);
  vect vMinPos;
  vectSetValue(vMinPos, granEdge, granEdge, granEdge);
  //--------------------------------
  for (unsigned ip = 0; ip < dem_ptr->numDRigid; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)), ip/xNum%yNum, ip/(xNum*yNum) + 2);
    vectScale(vOffset, granEdge*2);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, vOffset, boxBigR, boxSmallR, granDensity); 
    //vectSetValue(dem_ptr->listDRigid[ip].velocity, 6.18, 0, 0);
  }
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranPile(Demon *dem_ptr) {
  unsigned xNum = 10;
  unsigned yNum = 10;
  unsigned zNum = 10;
  float smallRadius   = 0.05f;
  float bigRadius     = 0.2f;
  float granDensity   = 5.0f;
  float dist          = ((smallRadius + bigRadius)/sqrt(3.0)+smallRadius)*2;

  dem_ptr->time_step  = 0.00001f;
  dem_ptr->numDRigid        = xNum*yNum*zNum;
  dem_ptr->listDRigid       = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  dem_ptr->max_radius = bigRadius;
  vectCopy(dem_ptr->vGravity, GRAVITY);
  vectSetValue(dem_ptr->vDisplayOffset, -dist/2.0f*xNum, 0, 0);
  
  vect vMinPos;
  vectSetValue(vMinPos, dist/2.0f, dist/2.0f, dist/2.0f);
  //--------------------------------
  for (unsigned ip = 0; ip < dem_ptr->numDRigid; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)), ip/xNum%yNum, ip/(xNum*yNum) + 2);
    vectScale(vOffset, dist);
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, vOffset, bigRadius, smallRadius, granDensity); 
  }
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGran2Collide(Demon *dem_ptr) {
  float smallRadius   = 0.15f;
  float bigRadius     = 0.4f;
  float granDensity   = 5.0f;
  float granEdge = ((bigRadius + smallRadius)/sqrt(3.0)+smallRadius);
  
  dem_ptr->time_step = 0.0001f;
  dem_ptr->max_radius = bigRadius;
  dem_ptr->numDRigid = 2;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  vectSetZero(dem_ptr->vGravity);
  vectSetValue(dem_ptr->vDisplayOffset, -2.0f, 0.0f, 0.0f);
  //--------------------------------
  unsigned ip = 0;
  vect pos;
  vectSetZero(pos);
  {
    vectSetValue(pos, 1.0f, 0.0f, 2.0f);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, pos, bigRadius, smallRadius, granDensity); 
    vectSetValue(dem_ptr->listDRigid[ip].velocity, 8.14, 0, 0);
    ip++;
  }
  {
    vectSetValue(pos, 5.0f, 0.0f, 2.0f - granEdge);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, pos, bigRadius, smallRadius, granDensity); 
    vectSetValue(dem_ptr->listDRigid[ip].velocity, -8.14, 0, 0);
    ip++;
  }
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranPlane(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->numDRigid = 1;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	boxSmallR*2*10, boxSmallR*2*2, granEdge+boxSmallR*10);
	InitGranularBox(ip, dem_ptr->listDRigid+ip, pos, boxBigR, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->listDRigid[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonGranPull(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->numDRigid = 2;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  
  vect pos;
  vectSetZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  float granEdge = ((boxBigR + boxSmallR)/sqrt(3.0)+boxSmallR);
  unsigned ip = 0;
  //--------------------------------
	vectSetValue(pos,	boxSmallR*2*10, boxSmallR*2*2, granEdge+boxSmallR*10);
	InitGranularBox(ip, dem_ptr->listDRigid+ip, pos, boxBigR, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->listDRigid[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
  unsigned xSize = 100;
  unsigned ySize = 20;
	vectSetValue(pos, 0, -ySize*boxSmallR, 0);
	InitGranularHPlane(ip, dem_ptr->listDRigid+ip, pos, xSize, ySize, boxSmallR, granDensity);
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonPush(Demon *dem_ptr) {
  dem_ptr->time_step = 0.001f;
  dem_ptr->numDRigid = 2;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
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
	//InitGranularBox(ip, dem_ptr->listDRigid+ip, pos, boxBigR, boxSmallR, granDensity); 
	InitGranularSphere(ip, dem_ptr->listDRigid+ip, pos, boxSmallR, granDensity); 
	vectSetValue(dem_ptr->listDRigid[ip].velocity, 6.18, 0, 0);
	ip++;
  //--------------------------------
  unsigned xSize = 100;
  unsigned ySize = 20;
	vectSetValue(pos, 0, -ySize*boxSmallR, 0);
	InitGranularHPlane(ip, dem_ptr->listDRigid+ip, pos, xSize, ySize, boxSmallR, granDensity);
  //--------------------------------
  //InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonCubeGran(Demon *dem_ptr)
{
  float smallRadius   = 0.06f;
  float bigRadius     = 0.2f;
  float cubeRadius    = 1.0f;

  dem_ptr->time_step  = 0.0001f;
  dem_ptr->numDRigid  = 1;
  dem_ptr->listDRigid = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  dem_ptr->max_radius = 2*smallRadius + bigRadius;
  vectCopy(dem_ptr->vGravity, GRAVITY);
  vectSetValue(dem_ptr->vDisplayOffset, 
      0.0f, 0.0f, 0.0f);
  
  //--------------------------------
  unsigned ip = 0;
  vect pos;
  vectSetValue(pos, 0.0f, 0.0f, 2.0f);
  InitCubeGranular(ip, dem_ptr->listDRigid+ip, pos, cubeRadius, 5.0f);
  ip++;
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
/** Interact between cube & granular
 * 
 *
 */
void InitDemonInterCubeGran(Demon *dem_ptr)
{
  /*
  unsigned xNum = 10;
  unsigned yNum = 10;
  unsigned zNum = 10;
  */
  unsigned xNum = 8;
  unsigned yNum = 8;
  unsigned zNum = 8;
  unsigned xCube  = 8;
  unsigned yCube  = 8;
  unsigned zCube  = 5;
  float smallRadius   = 0.06f;
  float bigRadius     = 0.2f;
  float cubeRadius    = 0.2f;
  float granDensity   = 5.0f;
  float cubeDensity   = 4.0f;
  float diameter      = ((smallRadius + bigRadius)/sqrt(3.0)+smallRadius)*2;

  dem_ptr->time_step  = 0.0001f;
  dem_ptr->numDRigid        = xNum*yNum*zNum + 2;
  dem_ptr->listDRigid       = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  dem_ptr->max_radius = 2*smallRadius + bigRadius;
  vectCopy(dem_ptr->vGravity, GRAVITY);
  vectSetValue(dem_ptr->vDisplayOffset, 
      -diameter/2.0f*xNum - cubeRadius*xCube, 0, 0);
  
  vect vMinPos;
  vectSetValue(vMinPos, diameter, diameter/2.0f, diameter/2.0f);
  //--------------------------------
  unsigned ip = 0;
  for (; ip < xNum*yNum*zNum; ip ++) {
    vect vOffset;
    vectSetValue(vOffset, ((int)(ip%xNum)), ip/xNum%yNum, ip/(xNum*yNum));
    vectScale(vOffset, diameter);
    vOffset[2] = vOffset[2] + cubeRadius*5;
    vectAdd(vOffset, vMinPos);
    InitGranularBox(ip, dem_ptr->listDRigid+ip, 
        vOffset, bigRadius, smallRadius, granDensity); 
  }
  vectSetValue(vMinPos,
      cubeRadius*(xCube+2) + diameter*(xNum + 2), 
      cubeRadius*(yCube+2),
      cubeRadius*(zCube+1));
  InitCubeRigid(ip, dem_ptr->listDRigid+ip, vMinPos, cubeRadius, cubeDensity,
      xCube, yCube, zCube); 
  ip++;
  vectSetValue(vMinPos,
      cubeRadius*(0) + diameter*(xNum/2 + 0.5f), 
      cubeRadius*(0) + diameter*(yNum/2 + 0.5f),
      cubeRadius*(2));
  InitCubeRigid(ip, dem_ptr->listDRigid+ip, vMinPos, cubeRadius, cubeDensity,
      2, 2, 2); 
  ip++;
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void InitDemonCube(Demon *dem_ptr)
{
  unsigned xNum = 4;
  unsigned yNum = 4;
  unsigned zNum = 4;
  float radius  = 0.2f;
  float granDensity   = 10.0f;

  dem_ptr->time_step  = 0.00001f;
  dem_ptr->numDRigid        = 1;
  dem_ptr->listDRigid       = (Granular*)malloc(sizeof(Granular)*dem_ptr->numDRigid);
  dem_ptr->max_radius = radius;
  vectCopy(dem_ptr->vGravity, GRAVITY);
  vectSetValue(dem_ptr->vDisplayOffset, 0.0f, 0, 0);
  
  vect vMinPos;
  vectSetValue(vMinPos, 2.0f, 2.0f, 2.0f);
  //--------------------------------
  unsigned ip = 0;
  InitCubeRigid(ip, dem_ptr->listDRigid+ip, vMinPos, radius, granDensity,
      xNum, yNum, zNum); 
  ip++;
  
  //--------------------------------
  InitDemonHT(dem_ptr);
}
//-------------------------------------------------------------------------
void TimeIntergration(Demon *dem_ptr)
{
  ClearHashTable(&(dem_ptr->htDRigid));
  vect temp;
  for(unsigned ip = 0; ip < dem_ptr->numDRigid; ip ++)
  {
    GranularTimeIntergration(dem_ptr->listDRigid+ip, dem_ptr->time_step, dem_ptr->vGravity);
    InsertHashTable(&(dem_ptr->htDRigid), dem_ptr->listDRigid+ip);
  }
}
//-------------------------------------------------------------------------
void ComputeForce(Demon *dem_ptr)
{
  unsigned hv[27];
  for(unsigned ig = 0; ig < dem_ptr->numDRigid; ig ++) {
    Granular *iGran = dem_ptr->listDRigid + ig;
    for (unsigned ip = 0; ip < iGran->num; ip ++) {
      Particle* iPart = iGran->component + ip;
      unsigned num_hv = 0;
      vect nei_grid;
      for(int ix = -1; ix <= 1; ix ++)
      {
        for(int iy = -1; iy <= 1; iy ++)
        {
          for(int iz = -1; iz <= 1; iz ++)
          {
            vectSetValue(nei_grid,
              ix*dem_ptr->htDRigid.cell_length,
              iy*dem_ptr->htDRigid.cell_length,
              iz*dem_ptr->htDRigid.cell_length);
            vectAdd(nei_grid, iGran->component[ip].position);
            unsigned cur_hv = HASH_VALUE(nei_grid,
                dem_ptr->htDRigid.min_pos,
                dem_ptr->htDRigid.cell_length,
                dem_ptr->htDRigid.table_size);
            int ih = 0;
            while (hv[ih] != cur_hv && ih < num_hv) {
              ih ++;
            }
            if (ih == num_hv) {
              hv[num_hv++] = cur_hv;
              HashGrid *curHG = &(dem_ptr->htDRigid.content[cur_hv]);
              for(unsigned iIndex = 0; iIndex < curHG->num; iIndex ++) {
                Granular *jGran = dem_ptr->listDRigid + curHG->content[iIndex].iGran;
                if (iGran->index >= jGran->index) continue;
                Particle* jPart = jGran->component + curHG->content[iIndex].iPart;
                ComputeParticleForce(iPart, jPart);
              }
            }
          }
        }
      }
      GranApplyBound(iGran);
    }
  }
}
//-------------------------------------------------------------------------
void SaveDemon(const Demon *dem_ptr, FILE *fp)
{
  char obj_name[] = "listDRigid";
  fprintf(fp, "#declare %s = union {\n\t", obj_name);
  for(unsigned i = 0; i < dem_ptr->numDRigid; i++) {
    for(unsigned j = 0; j < dem_ptr->listDRigid[i].num; j++) {
      const Particle *p = &(dem_ptr->listDRigid[i].component[j]);
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
	FreeHashTable(&(dem_ptr->htDRigid));
	for(unsigned is = 0; is < dem_ptr->numDRigid; is ++) {
		FreeGranular(&(dem_ptr->listDRigid[is]));
	}
	free(dem_ptr->listDRigid);
}
