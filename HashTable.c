#include "HashTable.h"
#include "vect.h"

//-------------------------------------
void InitHashGrid(HashGrid* hg)
{
  hg->num = 0;
  hg->capacity = HASH_GRID_INIT_CAPACITY;
  if(hg->content != NULL)
  {
    free(hg->content);
    hg->content = (ParticleIndex*) malloc(sizeof(ParticleIndex)*hg->capacity);
  }
}
//-------------------------------------
void InsertHashGrid(HashGrid *hg, ParticleIndex e)
{
  if(hg->num == hg->capacity)
  {
    hg->capacity += HASH_GRID_INCR_CAPACITY;
    ParticleIndex *ptr = (ParticleIndex*)malloc(sizeof(ParticleIndex)*hg->capacity);
    memcpy(ptr, hg->content, hg->num*sizeof(ParticleIndex));
    free(hg->content);
    hg->content = ptr;
  }
  hg->content[hg->num++] = e;
}
//-------------------------------------
void ClearHashGrid(HashGrid *hg)
{
  hg->num = 0;
}
//-------------------------------------
void FreeHashGrid(HashGrid *hg)
{
  hg->num = hg->capacity = 0;
  if(hg->content != NULL)
  {
    free(hg->content);
    hg->content = NULL;
  }
}

//-------------------------------------------------------------------------
void InitHashTable(HashTable *ht, unsigned ts, const vect* min_p, float cl)
{
  ht->table_size = ts;
  ht->cell_length = cl;
  copyVect(&(ht->min_pos), min_p);
  if(ht->content != NULL)
  {
    free(ht->content);
  }
  ht->content = (HashGrid*)malloc(sizeof(HashGrid)*ht->table_size);
}
//-------------------------------------
void InsertHashTable(HashTable *ht, const Particle *p)
{
  unsigned hv = HASH_VALUE(p->position, ht->min_pos, ht->cell_length, ht->table_size);
  InsertHashGrid(ht->content+hv, p->index);
}
//-------------------------------------
void ClearHashTable(HashTable *ht)
{
  if(ht->content == NULL)
    return;
  for(unsigned ig = 0; ig < ht->table_size; ig ++)
  {
    ClearHashGrid(ht->content+ig);
  }
}
//-------------------------------------
void FreeHashTable(HashTable *ht)
{
  if(ht->content == NULL)
    return;
  for(unsigned ig = 0; ig < ht->table_size; ig ++)
  {
    FreeHashGrid(ht->content+ig);
  }
  free(ht->content);
  ht->content = NULL;
}
//-------------------------------------
//-------------------------------------
