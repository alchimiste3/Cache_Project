#include "cache.h"
#include "low_cache.h"
#include <stddef.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

    return pcache;
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){
	free(pcache->headers);
	free(pcache);
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

    return CACHE_OK;
}


//! Lecture  (à travers le cache).
/*
Lecture à travers le cache de l’enregistrement d’indice irfile dans
le fichier. Le paramètre precord doit pointer sur un buffer fourni par l’application et
au moins de taille recordsz . L’enregistrement sera transféré du cache dans ce buffer
pour une lecture.*/
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord) {
	int ibfile = irfile/(pcache->nrecords);
	int n = irfile%(pcache->nrecords);
	char * copiePrecord = malloc(pcache->recordsz * sizeof(char));
	struct Cache_Block_Header *headers = pcache->headers;
	int i;
	for (i = 0; i < pcache->nblocks; i++) {
		if (headers[i].ibfile == ibfile) {
			char *d = headers[i].data;
			d += n*pcache->recordsz;
			int j;
			for (j = 0; j < pcache->recordsz; j++) {
				copiePrecord[j]= d[j];
			}
			precord = (void *) copiePrecord;
			return CACHE_OK;
		}
	}
	
	return CACHE_KO;
}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord) {
	int ibfile = irfile/(pcache->nrecords);
	int n = irfile%(pcache->nrecords);
	char * copiePrecord = (char *) precord;
	struct Cache_Block_Header *headers = pcache->headers;
	int i;
	for (i = 0; i < pcache->nblocks; i++) {
		if (headers[i].ibfile == ibfile) {
			char *d = headers[i].data;
			d += n*pcache->recordsz;
			int j;
            //printf("precord.i : %d\n", (struct*)precord);
            printf("copiePrecord : %s\n", copiePrecord);
            printf("taille copiePrecord : %s\n", strlen(copiePrecord));

            //strcpy(d, copiePrecord);
			for (j = 0; j < pcache->recordsz; j++) {
				d[j] = copiePrecord[j];
			}
			return CACHE_OK;
		}
	}
	return CACHE_KO;
}


//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache) {
	struct Cache_Instrument pinstrument = pcache->instrument;
	(pcache->instrument).n_deref = 0;
	(pcache->instrument).n_hits = 0;
	(pcache->instrument).n_reads = 0;
	(pcache->instrument).n_syncs = 0;
	(pcache->instrument).n_writes = 0;

	return &pinstrument;
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
