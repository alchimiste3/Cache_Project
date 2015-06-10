#include "low_cache.h"


//! Recherche d'un bloc libre.
struct Cache_Block_Header *Get_Free_Block(struct Cache *pcache) {
	struct Cache_Block_Header *ret = pcache->pfree;
	if (ret != NULL)
		return ret;
	ret->flags |= VALID;
	pcache->pfree = (pcache->pfree)+1;
	return ret;
}