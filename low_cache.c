#include "low_cache.h"


//! Recherche d'un bloc libre.
struct Cache_Block_Header *Get_Free_Block(struct Cache *pcache) {
	struct Cache_Block_Header *res;

	int i ;

	for(i = 0; i < pcache->nblocks ; i++){
		res =  &(pcache->headers[i]);

		if((res->flags & VALID) == 0) return res;
	}

	return NULL;

}