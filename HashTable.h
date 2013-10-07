#ifndef HASH_TABLE_H_H
#define HASH_TABLE_H_H

#include "vect.h"
#include "Granular.h"
#include "common.h"
#define NULL (0)
#define P1 (73856093)
#define P2 (19349663)
#define P3 (83492791)
/*
#define HASH_VALUE(v, min_p, cell, ts) (((unsigned long)(((v).x-(min_p).x)/(cell)*P1))\
	^((unsigned long)(((v).y-(min_p).y)/(cell)*P2))\
	^((unsigned long)(((v).z-(min_p).z)/(cell)*P3)))%(ts)
  */
#define HASH_GRID_INIT_CAPACITY (10)
#define HASH_VALUE(v, min_p, cell, ts) (((unsigned long)(((v)[0]-(min_p)[0])/(cell)*P1))\
	^((unsigned long)(((v)[1]-(min_p)[1])/(cell)*P2))\
	^((unsigned long)(((v)[2]-(min_p)[2])/(cell)*P3)))%(ts)
#define HASH_GRID_INIT_CAPACITY (10)
#define HASH_GRID_INCR_CAPACITY (10)
//-------------------------------------
typedef struct  
{
  unsigned num;
  //unsigned hash_value;
  unsigned capacity;
  ParticleIndex *content;
}HashGrid;

void InitHashGrid(HashGrid *);
void InsertHashGrid(HashGrid *, ParticleIndex);
void ClearHashGrid(HashGrid *);
void FreeHashGrid(HashGrid *);

//-------------------------------------
typedef struct
{
  HashGrid *content;
  unsigned table_size;
  vect min_pos;
  float cell_length;
} HashTable;

//-------------------------------------
void InitHashTable(HashTable *ht, unsigned ts, const vect min_pos, float cl);
void InsertHashTable(HashTable *, const Granular *);
void ClearHashTable(HashTable *);//clear but not free
void FreeHashTable(HashTable *);//free

//-------------------------------------
#endif
