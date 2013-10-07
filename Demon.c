#include "demon.h"
#include "vect.h"
#include "HashTable.h"
#include "Granular.h"

void InitDemonHT(Demon *dem_ptr)
{
  vect origin_point;
  setVectZero(origin_point);
  InitHashTable(&(dem_ptr->sand_ht),
	  dem_ptr->num*2, origin_point, dem_ptr->max_radius*2);
}

void InitDemon(Demon *dem_ptr)
{
  dem_ptr->time_step = 0.001f;
  dem_ptr->num = 2;
  dem_ptr->sand = (Granular*)malloc(sizeof(Granular)*dem_ptr->num);
  vect pos;
  setVectZero(pos);
  float boxBigR = 0.4f;
  float boxSmallR = 0.2f;
  float granDensity= 2.0f;
  dem_ptr->max_radius= boxBigR + boxSmallR*2;
  for(unsigned ip = 0; ip < dem_ptr->num; ip ++)
  {
    setVectValue(pos,
        1.0f,
        (ip+1)*dem_ptr->max_radius,
        1.0f);
    InitBoxGranular(dem_ptr+ip, pos, boxBigR, boxSmallR, granDensity); 
  }
  InitDemonHT(dem_ptr);
}
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

void ComputeForce(Demon *dem_ptr)
{
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
          setVectValue(nei_grid,
						ix*dem_ptr->sand_ht.cell_length,
            iy*dem_ptr->sand_ht.cell_length,
            iz*dem_ptr->sand_ht.cell_length);
          addVectTo(nei_grid, (cur_g->position), nei_grid);
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
void SaveDemon(const Demon *dem_ptr, FILE *fp)
{
  char obj_name[] = "sand";
  fprintf(fp, "#declare %s = union {\n\t", obj_name);
  for(unsigned i = 0; i < dem_ptr->num; i++) {
    for(unsigned j = 0; j < dem_ptr->sand[i].num; j++) {
      const particle *p = &(dem_ptr->sand[i].component[j]);
      fprintf(fp, "sphere {\n\t<%f, %f, %f>, %f\n\ttexture {pigment{color Gray}}\n}\n",
          p->position[0], p->position[1], p->position[2], p->radius);
    }
  }
  fprintf(fp, "}\n");
  fprintf(fp, "object {\n\t%s\n\ttranslate <%f, %f, %f>\n\trotate <%f, %f, %f>\n}\n",
      obj_name, 0, 0, 0, 0, 0, 0);
}
