#include "cache.h"
#include "low_cache.h"
#include "strategy.h"
#include <stddef.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>



int compteurAccess;

//! Création du cache.
/*
 * Crée un cache associé au fichier de nom fic : la cache comporte nblocks , chaque
 * bloc contenant nrecords enregistrements de taille recordsz caractères.
 * Le dernier paramètre ( nderef ) n’est utilisé que pour la stratégie nur ; pour les autres
 * stratégies sa valeur est ignorée. Dans le cas de nur, le bit de référence R devra être
 * remis à 0 (pour tous les blocs du cache) tous les nderef accès (lecture ou écriture).
 * La fonction ouvre le fichier en le créant et le remplissant si nécessaire, alloue et
 * initialise les structures de données du cache, et retourne un pointeur sur le nouveau
 * cache.
 */
struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,
                           size_t recordsz, unsigned nderef){
    
    struct Cache *pcache = malloc(sizeof(struct Cache));
    pcache->file = malloc(sizeof(char)*strlen(fic));
    strcpy(pcache->file, fic);
    pcache->fp = fopen(fic, "wb+");
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
    	pcache->headers[i].flags &= 0x0;
    	pcache->headers[i].ibfile = -1;
    	pcache->headers[i].ibcache = i;
    	pcache->headers[i].data = malloc(sizeof(char) * recordsz * nrecords);
    }

    pcache->pfree = &(pcache->headers[0]); //Le premier bloc est libre  puisque le cache vient d'être initialisé.

    //Mise à zero du compteur d'acces
    compteurAccess = 0;

    return pcache;
}

//! Fermeture (destruction) du cache.
/*
 * Détruit le cache pointé par pcache : synchronise le cache et le fichier grâce à Cache_Sync() ,
 * ferme le fichier et détruit toutes les structures de données du cache.
 */
Cache_Error Cache_Close(struct Cache *pcache){
    if(Cache_Sync(pcache) == CACHE_KO) return CACHE_KO;
    Strategy_Close(pcache);

    if(fclose(pcache->fp) == EOF) return CACHE_KO;
    
    int i;
    for(i = 0; i<pcache->nblocks; i++){
        //free(pcache->headers[i].data);
    }

    //free(pcache->headers);
    free(pcache->file);
    free(pcache);
    return CACHE_OK;
}

//! Synchronisation du cache.
/*
 * Synchronise le contenu du cache avec celui du fichier : écrit sur disque tous les blocs
 * dont le bit M vaut 1 et remet à 0 ce bit. L’application peut appeler Cache_Sync()
 * quand elle le souhaite, mais il y a un appel automatique tous les NSYNC accès (par
 * défaut NSYNC vaut 1000, défini dans low_cache.c ).
 */
Cache_Error Cache_Sync(struct Cache *pcache){

    //printf("Synchro\n");

    //On remet le compteur à 0
    compteurAccess = 0;

    //On récupère la liste des header (block cache)
    struct Cache_Block_Header *headers = pcache->headers;

    //On parcoure tout les headers
    int i;
    for(i = 0 ; i < pcache->nblocks ; i++){
        //Si le block est modifier ou si 
        if(((headers[i].flags & MODIF) > 0)&&((headers[i].flags & VALID) > 0)){

            //printf("header %d\n", i);

            //On se decale de headers[i].ibfile * pcache->blocksz dans le fichier, sur la zone qui contient le block
            if(fseek(pcache->fp, headers[i].ibfile * pcache->blocksz, SEEK_SET) != 0) return CACHE_KO;
            
            //On écrie le block cache sur block fichier
            if (fwrite(headers[i].data, pcache->recordsz, pcache->nrecords, pcache->fp) == 0) return CACHE_KO;

            //On met modifier à 0
            headers[i].flags = headers[i].flags & ~MODIF;
        }
    }

    //On incrémente n_syncs
    pcache->instrument.n_syncs++;


    return CACHE_OK;

}

//! Invalidation du cache.
/*
 * Invalide le cache, c’est-à-dire met à 0 le bit V de tous les blocs. C’est donc comme si
 * le cache était vide : aucun bloc ne contient plus d’information utile.
 * Noter que cette fonction Cache_Invalidate() ne devrait pas faire partie de l’interface
 * utilisateur du cache. Néanmoins, elle est nécessaire au simulateur, puisqu’elle permet
 * d’enchainer des tests différents sans avoir à réallouer le cache.
 */
Cache_Error Cache_Invalidate(struct Cache *pcache){
    struct Cache_Block_Header *headers = pcache->headers;

    int i;

    //Pour chaque block cache, on met validité = 0 et mofifier = 0 
    for(i = 0 ; i < pcache->nblocks ; i++){
        headers[i].flags = headers[i].flags & ~VALID;
        headers[i].flags = headers[i].flags & ~MODIF;

    }

    pcache->instrument.n_hits = 0;
    pcache->instrument.n_writes = 0;
    pcache->instrument.n_reads = 0;
    pcache->instrument.n_syncs = 0;

    Strategy_Invalidate(pcache);


    return CACHE_OK;
}


Cache_Error Write_Fichier(struct Cache *pcache, struct Cache_Block_Header * tmp){
    
    //Si modifier = 1
    if((tmp->flags & MODIF) > 0){
        //On décale le curseur sur  le fichier
        if(fseek(pcache->fp, tmp->ibfile * pcache->nrecords, SEEK_SET) != 0) return CACHE_KO;

        //On écrie le block fichier dans data
        if(fwrite(tmp->data, pcache->recordsz, pcache->nrecords, pcache->fp) == 0) return CACHE_KO;
       
        tmp->flags -= MODIF;
    }
    return CACHE_OK;
}

struct Cache_Block_Header * Trouver_Block_Cache(struct Cache * pcache, int irfile, const void * precord) 
{
    int ibfile = irfile/(pcache->nrecords);

    struct Cache_Block_Header *headers = pcache->headers;
   

    struct Cache_Block_Header * tmp = NULL;

    //On cherche un block dans le cache
    int i;
    for (i = 0; i < pcache->nblocks; i++) {
        if (headers[i].ibfile == ibfile) {
            if(headers[i].flags & VALID)
                //On a trouver le bon block
                pcache->instrument.n_hits++;
                tmp = &headers[i];
            break;
        }
    }

    //Si on pass trouver un block correspondant dans le cache
    if(tmp == NULL){
        ///printf("creation nouveau block cache");

        //On cherche un block a libérer
        tmp = Strategy_Replace_Block(pcache);

        //On écrie le block cache tmp sur le fichier aveent de le modifier
        Write_Fichier(pcache,tmp);

        //On modifier ibfile du block pour qu'il corresponde au nouvelle valeur 
        tmp->ibfile = ibfile;

        tmp->flags |= VALID;

    }

    return tmp;
}

//! Lecture  (à travers le cache).
/*
 * Lecture à travers le cache de l’enregistrement d’indice irfile dans
 * le fichier. Le paramètre precord doit pointer sur un buffer fourni par l’application et
 * au moins de taille recordsz . L’enregistrement sera transféré du cache dans ce buffer
 * pour une lecture.
 */
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord) {
    // printf("\nRead :\n");

    // printf("irfile = %d\n", irfile);

    // int ibfile = irfile/(pcache->nrecords);
    // printf("ibfile = %d\n", ibfile);

    // int n = irfile%(pcache->nrecords);
    // printf("num enregistrement = %d\n", n);

    //On comvertie les void * en char *
    char *copiePrecord = (char*)precord; 

    //On crer un block temporaire
    struct Cache_Block_Header * tmp;

    //On cherche un block qui soit contient déja la valeur que l'on cherche soit un nouveau block qu'on a remplie
    tmp = Trouver_Block_Cache(pcache, irfile, precord); 

    //On écrie dans le buffer copiePrecord
    if(snprintf(copiePrecord, pcache->recordsz, "%s", ADDR(pcache, irfile, tmp)) < 0) return CACHE_KO;
    
    tmp->flags |= MODIF;

    //On synchronise tout les NSYNC acces
    if(++compteurAccess >= NSYNC)
        Cache_Sync(pcache);


    //On incrémente n_reads
    pcache->instrument.n_reads++;

    //On utilise la fonction read de stratégie pour appliquer des changements éventuelles
    Strategy_Read(pcache, tmp);
    
    return CACHE_OK;

}

//! Écriture (à travers le cache).
/*
 * Ecriture à travers le cache de l’enregistrement d’indice irfile dans
 * le fichier. Le paramètre precord doit pointer sur un buffer fourni par l’application et
 * au moins de taille recordsz . L’enregistrement sera transféré du buffer vers le cache.
 */
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord) {
    // printf("\nWrite :\n");
    // printf("irfile = %d\n", irfile);
    // int ibfile = irfile/(pcache->nrecords);
    // printf("ibfile = %d\n", ibfile);
    // int n = irfile%(pcache->nrecords);
    // printf("num enregistrement = %d\n", n);

    //On comvertie les void * en char *
    char *copiePrecord = (char*)precord; 

    //On crer un block temporaire
    struct Cache_Block_Header * tmp;

    //On cherche un block qui soit contient déja la valeur que l'on cherche soit un nouveau block qu'on a remplie
    tmp = Trouver_Block_Cache(pcache, irfile, precord); 

    //On écrie le buffer copiePrecord sur le block fichier correspondant
    if(snprintf(ADDR(pcache, irfile, tmp), pcache->recordsz, "%s", copiePrecord) < 0) return CACHE_KO;

    tmp->flags |= MODIF;

    //On synchronise tout les NSYNC acces
    if(++compteurAccess >= NSYNC){
        Cache_Sync(pcache);
    }

    //On utilise la fonction write de stratégie pour appliquer des changements éventuelles
    Strategy_Write(pcache, tmp);
    
    pcache->instrument.n_writes++;

    return CACHE_OK;

}


//! Résultat de l'instrumentation.
/*
 * Retourne une copie de la structure d’instrumentation du cache pointé par pcache .
 * Tous les compteurs de la structure courante sont remis à 0 par cette
 * fonction.
 */
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



