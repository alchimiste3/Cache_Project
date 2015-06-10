#include "cache.h"
#include "low_cache.h"
#include "strategy.h"
#include <stddef.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern int fileno(FILE *stream);

//! Création du cache.
struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,
                           size_t recordsz, unsigned nderef){
    
    struct Cache *pcache = malloc(sizeof(struct Cache));
    pcache->file = malloc(sizeof(char)*strlen(fic));
    strcpy(pcache->file, fic);
    pcache->fp = fopen(fic, "r+");
    pcache->nblocks = nblocks;
    pcache->nrecords = nrecords;
    pcache->recordsz = recordsz;
    pcache->blocksz = nrecords*recordsz;
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


    //Création du tableau de headers
    pcache->headers = malloc(sizeof(struct Cache_Block_Header) * nblocks);
    int i;
    for(i = 0; i<nblocks; i++){
    	pcache->headers[i].flags = 0;
    	pcache->headers[i].ibfile = -1;
    	pcache->headers[i].ibcache = i;
    	pcache->headers[i].data = malloc(sizeof(char)*recordsz);
    }

    pcache->pfree = pcache->headers; //Le premier bloc est libre  puisque le cache vient d'être initialisé.

    // -------------- TEST ----------------
    // int j = 0;
    // while(j<nrecords){
    // 	printf(" -- bloc header %d -- \n", pcache->headers[j].ibcache);
    // 	printf(" -- data : %s -- \n", pcache->headers[j].data);
    // 	j++;
    // }

    // printf(" --- bloc libre : %d --- \n", pcache->pfree->ibcache);
    // ---------------- TEST ----------------------------

    return pcache;
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){
	// int i = 0;
	// for(i = 0; i<pcache->nblocks; i++){
	// 	free(pcache->headers[i].data);
	// }

	// free(pcache->headers);
	// free(pcache->file);
	// free(pcache);
	return CACHE_OK;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache){

    //On transforme le FILE * en fd pour écrire dessus
    int fd = fileno(pcache->fp);

    struct Cache_Block_Header *headers = pcache->headers;

    int i;
    for(i = 0 ; i < pcache->nblocks ; i++){
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

    int i;
    for(i = 0 ; i < pcache->nblocks ; i++){
        if(headers[i].flags & VALID){
            headers[i].flags -= VALID;
        }
    }

    Strategy_Invalidate(pcache);

    pcache->pfree = pcache->headers;

    return CACHE_OK;
}

//! Lecture  (à travers le cache).
/*
Lecture à travers le cache de l’enregistrement d’indice irfile dans
le fichier. Le paramètre precord doit pointer sur un buffer fourni par l’application et
au moins de taille recordsz . L’enregistrement sera transféré du cache dans ce buffer
pour une lecture.*/
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord) {
    printf("\nRead :\n");
    printf("irfile = %d\n", irfile);
    int ibfile = irfile/(pcache->nrecords);
    printf("ibfile = %d\n", ibfile);
    int n = irfile%(pcache->nrecords);
    printf("num enregistrement = %d\n", n);

    struct Cache_Block_Header *headers = pcache->headers;
    int i;

    struct Cache_Block_Header * tmp = NULL;

    int trouver = 0;

    for (i = 0; i < pcache->nblocks; i++) {
        //printf("headers[i].ibfile = %d\n", headers[i].ibfile);
        if (headers[i].ibfile == ibfile) {
            tmp = &headers[i];
            printf("trouver dans cache avec ibcache = %d\n",i);
            trouver = 1;
            break;
        }
    }


    if(trouver == 0){
        printf("creation nouveau block cache");

        tmp = Strategy_Replace_Block(pcache);
        tmp->ibfile = ibfile;

        Cache_Sync(pcache);

        printf("tmp->ibcache = %ld\n", tmp->ibcache);
    }


    memcpy(precord, &(tmp->data[n*pcache->recordsz]), pcache->recordsz);


    //printf("data.i = %d\n",(*(struct Any *)tmp->data).x);
    
    return CACHE_OK;

}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord) {
    printf("\nWrite :\n");
    printf("irfile = %d\n", irfile);
    int ibfile = irfile/(pcache->nrecords);
    printf("ibfile = %d\n", ibfile);
    int n = irfile%(pcache->nrecords);
    printf("num enregistrement = %d\n", n);

    struct Cache_Block_Header *headers = pcache->headers;
    int i;

    struct Cache_Block_Header * tmp = NULL;

    int trouver = 0;

    for (i = 0; i < pcache->nblocks; i++) {
        //printf("headers[i].ibfile = %d\n", headers[i].ibfile);
        if (headers[i].ibfile == ibfile) {
            tmp = &headers[i];
            printf("trouver dans cache avec headers[%d].ibfile = %d et ibcache = %d\n",i, headers[i].ibfile, i);
            trouver = 1;
            break;
        }
    }


    if(trouver == 0){
        printf("creation nouveau block cache");

        tmp = Strategy_Replace_Block(pcache);

        tmp->ibfile = ibfile;

        printf("tmp->ibcache = %ld\n", tmp->ibcache);
        printf("data = %s\n", tmp->data);

    }


    memcpy(&(tmp->data[n*pcache->recordsz - 1]), precord,  pcache->recordsz);

    printf("zzeefzef\n");

    //printf("data.i = %d\n",(*(struct Any *)tmp->data).x);
    
    return CACHE_OK;

}


//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache) {
	struct Cache_Instrument *pinstrument = malloc(sizeof(struct Cache_Instrument));
	memcpy(pinstrument, &(pcache->instrument), sizeof(struct Cache_Instrument));
	
	(pcache->instrument).n_deref = 0;
	(pcache->instrument).n_hits = 0;
	(pcache->instrument).n_reads = 0;
	(pcache->instrument).n_syncs = 0;
	(pcache->instrument).n_writes = 0;

	return pinstrument;
}




// void afficherBlockCache(struct Cache *pcache, int n){
//     printf("Block numero %d\n", n);

//     printf("%s\n", pcache->headers[n].data);
    

// }


// int main(int argc, char const *argv[])
// {
//     /* code */
//     return 0;
// }
