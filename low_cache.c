#include "low_cache.h"


//! Recherche d'un bloc libre.
struct Cache_Block_Header *Get_Free_Block(struct Cache *pcache) {
	struct Cache_Block_Header *ret = pcache->pfree;
	if (ret == NULL)
		return ret;
	ret->flags |= VALID;
	if(((pcache->pfree)+1)->ibcache < pcache->nblocks){
		(pcache->pfree)++;
	}
	else{
		pcache->pfree = NULL;
	}

	return ret;

}