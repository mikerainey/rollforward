#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "rollforward.h"

void try_to_initiate_rollforward(void** rip) {
  void* ra_src = *rip;
  void* ra_dst = NULL;
  // Binary search over the rollforward keys
  {
    int64_t i = 0, j = (int64_t)rollforward_table_size - 1;
    int64_t k;
    while (i <= j) {
      k = i + ((j - i) / 2);
      if ((uint64_t)rollforward_table[k].from == (uint64_t)ra_src) {
	ra_dst = rollforward_table[k].to;
	break;
      } else if ((uint64_t)rollforward_table[k].from < (uint64_t)ra_src) {
	i = k + 1;
      } else {
	j = k - 1;
      }
    }
  } 
  if (ra_dst != NULL) {
    *rip = ra_dst;
  }
}

void* try_to_initiate_rollbackward(void* ra_dst) {
  void* ra_src = NULL;
  // Binary search over the rollbackwards
  {
    int64_t i = 0, j = (int64_t)rollforward_table_size - 1;
    int64_t k;
    while (i <= j) {
      k = i + ((j - i) / 2);
      if ((uint64_t)rollback_table[k].from == (uint64_t)ra_dst) {
	ra_src = rollback_table[k].to;
	break;
      } else if ((uint64_t)rollback_table[k].from < (uint64_t)ra_dst) {
	i = k + 1;
      } else {
	j = k - 1;
      }
    }
  }
  return ra_src;
}

int rollforward_table_sorter(const void* v1,const void* v2) {
  return *((uint64_t*)v1)-*((uint64_t*)v2);
}

__attribute__((constructor)) // GCC syntax that makes __initialize run before main()
void __initialize(int argc, char **argv) {
  qsort(rollforward_table, rollforward_table_size, 16, rollforward_table_sorter);
  qsort(rollback_table, rollforward_table_size, 16, rollforward_table_sorter);
  { // sanity check of the rollforward/rollbackward tables
    if (rollforward_table_size == 0) {
      return;
    }
    uint64_t rff1 = (uint64_t)rollforward_table[0].from;
    for (uint64_t i = 1; i < rollforward_table_size; i++) {
      uint64_t rff2 = (uint64_t)rollforward_table[i].from;
      // check increasing order of 'from' keys in rollforwad table
      if (rff2 < rff1) {
	printf("bogus ordering in rollforward table rff2=%lx rff1=%lx\n",rff2,rff1);
	//      exit(1);
      }
      // check that rollback table is an inverse mapping of the
      // rollforward table
      uint64_t rft2 = (uint64_t)rollforward_table[i].to;
      uint64_t rbf2 = (uint64_t)rollback_table[i].from;
      uint64_t rbt2 = (uint64_t)rollback_table[i].to;
      if (rft2 != rbf2) {
	printf("bogus mapping rft2=%lx rbf2=%lx\n",rft2,rbf2);
	exit(1);
      }
      if (rff2 != rbt2) {
	printf("bogus mapping!\n");
	exit(1);
      }
      rff1 = rff2;
    }
  }
}
