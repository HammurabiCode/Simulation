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
    hg->content = (HashContent*) malloc(sizeof(HashContent)*hg->capacity);
  }
}
//-------------------------------------
void InsertHashGrid(HashGrid *hg, HashContent e)
{
  if(hg->num == hg->capacity)
  {
    hg->capacity += HASH_GRID_INCR_CAPACITY;
    HashContent *ptr = (HashContent*)malloc(sizeof(HashContent)*hg->capacity);
    memcpy(ptr, hg->content, hg->num*sizeof(HashContent));
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
void InitHashTable(HashTable *ht, unsigned ts, const vect min_p, float cl)
{
  ht->table_size = ts;
  ht->cell_length = cl;
  vectCopy(ht->min_pos, min_p);
  if(ht->content != NULL)
  {
    free(ht->content);
  }
  ht->content = (HashGrid*)malloc(sizeof(HashGrid)*ht->table_size);
  memset(ht->content, 0, sizeof(HashGrid)*ht->table_size);
}
//-------------------------------------
void InsertHashTable(HashTable *ht, const Granular *g)
{
  HashContent hc;
  for (ParticleIndex ip = 0; ip < g->num; ip ++) {
    unsigned hv = HASH_VALUE(g->component[ip].position,
        ht->min_pos, ht->cell_length, ht->table_size);
    hc.iGran = g->index;
    hc.iPart = ip;
    InsertHashGrid(ht->content+hv, hc);
  }
}
//-------------------------------------
void HashTablePrint(const HashTable *ht) {
  for (unsigned iGrid = 0; iGrid < ht->table_size; iGrid ++) {
    if (ht->content[iGrid].num == 0) continue;
    printf("%03u:(%03u) ", iGrid, ht->content[iGrid].num);
    for (unsigned iEle = 0; iEle < ht->content[iGrid].num; iEle ++) {
      printf("%03u, ", ht->content[iGrid].content[iEle]);
    }
    printf("\n");
  }
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
