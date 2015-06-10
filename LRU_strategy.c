/*!
 * \file RAND_strategy.c
 *
 * \brief  Stratégie de remplacement au hasard..
 *
 * \author Jean-Paul Rigault
 *
 * $Id: RAND_strategy.c,v 1.3 2008/03/04 16:52:49 jpr Exp $
 */

#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "cache_list.h"
#include "random.h"
#include "time.h"


struct Cache_List *pcl;

/*!
 * RAND : pas grand chose à faire ici.
 *
 * En fait, nous initialisons le germe
 * (seed) du générateur aléatoire à quelque chose d'éminemment variable, pour
 * éviter d'avoir la même séquence à chque exécution...
 */
void *Strategy_Create(struct Cache *pcache)
{
	pcl = Cache_List_Create();

    struct Cache_Block_Header *h = pcache->headers;

    int i;
    for(i = 0 ; i < pcache->nblocks ; i++){
        Cache_List_Prepend(pcl,&h[i]);
    }

	return (void *) pcl;
}

/*!
 * RAND : Rien à faire ici.
 */
void Strategy_Close(struct Cache *pcache)
{
	Cache_List_Delete(pcl);
}

/*!
 * RAND : Rien à faire ici.
 */
void Strategy_Invalidate(struct Cache *pcache)
{
}

/*!
 * LRU : On prend le premier bloc invalide. S'il n'y en a plus, on prend le bloc le moins récemment utilisé.
 */
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache)
{
    struct Cache_Block_Header *pbh;

    /* On cherche d'abord un bloc invalide */
    if ((pbh = Get_Free_Block(pcache)) != NULL) return pbh;

    if (Cache_List_Is_Empty(pcl)) {
    	perror("vide : pas possible");
    	exit(1);
    }

    pbh = Cache_List_Remove_First(pcl);
    Cache_List_Append(pcl, pbh);

    return pbh;
}


/*!
 * LRU :
 */
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
	Cache_List_Move_To_End(pcl,pbh);
}

/*!
 * LRU :
 */
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
	Cache_List_Move_To_End(pcl,pbh);
}

char *Strategy_Name()
{
    return "LRU";
}
