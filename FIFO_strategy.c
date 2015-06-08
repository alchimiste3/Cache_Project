/*!
 * \file FIFO_strategy.c
 *
 * \brief  Stratégie de remplacement au hasard..
 * 
 * \author Jean-Paul Rigault 
 *
 * $Id: FIFO_strategy.c,v 1.3 2008/03/04 16:52:49 jpr Exp $
 */

#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "cache_list.h"
#include "random.h"
#include "time.h"


struct Cache_List * list;

/*!
 * FIFO
 *
 * En fait, nous initialisons le germe
 * (seed) du générateur aléatoire à quelque chose d'éminemment variable, pour
 * éviter d'avoir la même séquence à chque exécution...
 */
void *Strategy_Create(struct Cache *pcache) 
{
    list = Cache_List_Create();

    struct Cache_Block_Header *h = pcache->headers;

    for(int i = 0 ; i < pcache->nblocks ; i++){
        Cache_List_Prepend(list,&h[i]);
    }

}

/*!
 * FIFO
 */
void Strategy_Close(struct Cache *pcache)
{
    Cache_List_Delete(list);
}

/*!
 * FIFO : Rien à faire ici.
 */
void Strategy_Invalidate(struct Cache *pcache)
{ 
}

/*! 
 * FIFO : On prend le premier bloc invalide. S'il n'y en a plus, on prend un bloc au hasard.
 */
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{
    int ib;
    struct Cache_Block_Header *pbh;

    /* On cherche d'abord un bloc invalide */
    if ((pbh = Get_Free_Block(pcache)) != NULL) return pbh;

    if(Cache_List_Is_Empty(list)){
        perror("pas possible");
        exit(1);
    }

    pbh = Cache_List_Remove_First(list);
    Cache_List_Append(list, pbh);

    return pbh;

}


/*!
 * FIFO : Rien à faire ici.
 */
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{
}  

/*!
 * FIFO : Rien à faire ici.
 */  
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
} 

char *Strategy_Name()
{
    return "FIFO";
}

