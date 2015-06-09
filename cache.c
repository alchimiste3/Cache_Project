#include "cache.h"
#include "low_cache.h"
#include <stddef.h> 
#include <stdio.h>
#include <unistd.h>

extern int fileno(FILE *stream);

//! Création du cache.
struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,
                           size_t recordsz, unsigned nderef){
    
    struct Cache *pcache = malloc(sizeof(struct Cache));
    pcache->file = fic;
    pcache->fp = fopen(fic, "r+");
    pcache->nblocks = nblocks;
    pcache->nrecords = nrecords;
    pcache->recordsz = recordsz;
    pcache->blocksz = recordsz*recordsz;
    pcache->nderef = nderef;
    pcache->pstrategy = Strategy_Create(pcache);


    //Création du cache_Instrument
    struct Cache_Instrument instrument;
    instrument.n_reads = 0;
    instrument.n_writes = 0;
    instrument.n_hits = 0;
    instrument.n_syncs = 0;
    instrument.n_deref = 0;

    pcache->instrument = instrument;


    pcache->headers = malloc(sizeof(struct Cache_Block_Header) * nblocks);
    pcache->pfree = Get_Free_Block(pcache);

}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){

}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache){

    //On transforme le FILE * en fd pour écrire dessus
    int fd = fileno(pcache->fp);

    struct Cache_Block_Header *headers = pcache->headers;

    for(int i = 0 ; i < pcache->nblocks ; i++){
        if((headers[i].flags & MODIF) >> 1){

            //On récuper la valeur du block
            char * data = headers[i].data;

            //On déplace le file descriptor sur le block correspondant dans le fichier
            lseek(fd,headers[i].ibfile*pcache->blocksz, SEEK_SET);

            //On écrie data sur le fichier 
            write(fd,data,pcache->blocksz);
        }
    }

}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache){
    struct Cache_Block_Header *headers = pcache->headers;

    for(int i = 0 ; i < pcache->nblocks ; i++){
        if(headers[i].flags & VALID){
            headers[i].flags -= VALID;
        }
    }

    Strategy_Invalidate(pcache);
}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord);

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord);


//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache);