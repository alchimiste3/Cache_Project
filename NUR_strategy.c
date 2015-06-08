/*!
 * \file NUR_strategy.c
 *
 * \brief  Stratégie de remplacement au hasard..
 * 
 * \author Jean-Paul Rigault 
 *
 * $Id: NUR_strategy.c,v 1.3 2008/03/04 16:52:49 jpr Exp $
 */

#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "random.h"
#include "time.h"




int nderef;


/*!
 * NUR
 */
void *Strategy_Create(struct Cache *pcache) 
{
    nderef = 0;
}

/*!
 * NUR
 */
void Strategy_Close(struct Cache *pcache)
{
}

/*!
 * NUR : Rien à faire ici.
 */
void Strategy_Invalidate(struct Cache *pcache)
{ 
}

/*! 
 * NUR :
 */
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{

    struct Cache_Block_Header *pbh;

    /* On cherche d'abord un bloc invalide */
    if ((pbh = Get_Free_Block(pcache)) != NULL) return pbh;

    int i = 0;
    struct Cache_Block_Header *tmp = NULL;
    int addMin = 8;
    int add = 0;

    while(i < pcache->nblocks){
        pbh = &pcache->headers[i++];

        // add = 2 x R + M
        add = ((pbh->flags & 4) >> 2)*2 + ((pbh->flags & 2) >> 1);

        switch(add){
            case 0 : 
                return pbh;
            break;
            case 1 : case 2 : case 3 :
                if(addMin > add){
                    addMin = add;
                    tmp = pbh;
                }
            break;
        }
    }

    return tmp;

}


/*!
 * NUR 
 */
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{

    nderef++;

    //R = 1
    pbh->flags |=  (1 << 2);

    if(nderef >= pcache->nderef){

        struct Cache_Block_Header *h = pcache->headers;
        for(int e = 0 ; e < pcache->nblocks ; e++){
            h[e].flags -= 4;
        }

        nderef = 0;

    }
}  

/*!
 * NUR 
 */  
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
    nderef++;

    //R = 1
    pbh->flags |=  (1 << 2);

    if(nderef >= pcache->nderef){

        struct Cache_Block_Header *h = pcache->headers;
        for(int e = 0 ; e < pcache->nblocks ; e++){
            h[e].flags -= 4;
        }

        nderef = 0;

    }
} 

char *Strategy_Name()
{
    return "NUR";
}

