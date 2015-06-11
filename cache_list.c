#include <stdlib.h>
#include <stdio.h>
#include "cache_list.h"
#include <stddef.h> 

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){
    struct Cache_List *listVide = malloc(sizeof(struct Cache_List));

    struct Cache_Block_Header *head = NULL;
    listVide->pheader = head;

    listVide->next = NULL;
    listVide->prev = NULL;

    return listVide;
}

/*! Destruction d'une liste de blocs */
 void Cache_List_Delete(struct Cache_List *list){
    struct Cache_List* current = list;
    
    //On se positionne à la fin de la liste 
    while(current->next){
        current = current->next;
    }

    //on part de la fin de la liste et on supprime tout tant qu'il existe un prev
    while(current->prev){
        current = current->prev;
        printf("DEBUG Function [Cache_List_Delete] : will free { current->next = %p AND current->next->header = %p } \n", current->next, current->next->pheader);
        free(current->next->pheader);
        free(current->next);
    }

    printf("DEBUG Function [Cache_List_Delete] : dernier free { current = %p AND current->pheader = %p }\n", current, current->pheader);
    free(current->pheader);
    current->next = NULL;
    current->prev = NULL;
    free(current);  //On supprime le dernier élèment de la liste
 }

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
    if(list->pheader == NULL){
        printf("\t\tDEBUG Function[Cache_List_Append] : { list->pheader = %p AND pbh = %p } ajout de pbh here\n", list->pheader, pbh);
        list->pheader = pbh;
    }
    else{
        struct Cache_List *current = list;
        while(current->next){
            printf("\t\t\tDEBUG Function[Cache_List_Append] : INSIDE! { current = %p AND current->next = %p }\n", current, current->next);
            current = current->next;
            printf("dans  boucle \n");
        }


        printf("sortie  de la boucle\n");
        //Création du nouvel élément
        struct Cache_List *newElem = malloc(sizeof(struct Cache_List));
        newElem->pheader = pbh;
        newElem->next = NULL;
        newElem->prev = current;

        current->next = newElem;    //Insertion de l'élément à la fin de la liste
        printf("\t\t\tDEBUG Function[Cache_List_Append] : AJOUT FINI { current = %p AND current->prev = %p AND current->next = %p }\n", current, current->prev, current->next);
    }
}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh){
    if(list->pheader == NULL){
        printf("\t\tDEBUG Function[Cache_List_Prepend] : { list->pheader = %p AND pbh = %p } ajout de pbh here\n", list->pheader, pbh);
        list->pheader = pbh;
    }
    else{
        //On se positionne au début de la liste
        struct Cache_List *current = list;
        //printf("\t\tDEBUG Function[Cache_List_Prepend] : current = %p and current->prev = %p\n", current, current->prev);
        while(current->prev){
            printf("\t\t\tDEBUG Function[Cache_List_Prepend] : INSIDE! { current = %p AND current->prev = %p }\n", current, current->prev);
            current = current->prev;
        }

        //Création du nouvel élément
        struct Cache_List *newElem = malloc(sizeof(struct Cache_List));
        newElem->pheader = pbh;
        newElem->next = current;
        newElem->prev = NULL;

        current->prev = newElem; //On ajoute l'élément à la fin de la liste
        printf("\t\t\tDEBUG Function[Cache_List_Prepend] : AJOUT FINI { current = %p AND current->prev = %p AND current->next = %p }\n", current, current->prev, current->next);
    }
}

/*! Retrait du premier élément */
struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list){
    //On se positionne au début de la liste
    struct Cache_List *current = list;
    printf("\tDEBUG Function[Cache_List_Remove_FIrst] : { list = %p }\n", current);
    while(current->prev){
        current = current->prev;
    }

    printf("\tDEBUG Function[Cache_List_Remove_FIrst] : { current = %p }\n", current);
    if(current == list){
        if(!current->next){
            if(current->pheader) {
                printf("\t\tDEBUG Function[Cache_List_Remove_FIrst] : (current = %p) == (list = %p) AND (current->next = %p) == NULL. Will free current->pheader = %p \n",
                       current, list, current->next, current->pheader);
                free(current->pheader); //La list est maintenant vide, pas de next, pas de prev
                struct Cache_Block_Header *head = NULL;
                current->pheader = head;  
                current->next = NULL;
                current->prev = NULL;
                return current->pheader;
            }
            else{
                printf("\t\tLa liste cache_list donnee est vide!\n");
                return NULL; //Liste vide
            }
        }
    }
    current = current->next;
    printf("\tDEBUG Function[Cache_List_Remove_FIrst] : At beginning { current = %p } will free current->prev->pheader = %p AND current->prev = %p\n", current, current->prev->pheader, current->prev);
    free(current->prev->pheader);
    free(current->prev);
    printf("\tDEBUG Function[Cache_List_Remove_FIrst] : After free current = %p\n", current);
    current->prev = NULL;
    return current->pheader;
}

/*! Retrait du dernier élément 
 * Supprime le dernier élément de la liste.
 * \Retourne 
 */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
    //On se positionne à la fin de la liste
    struct Cache_List *current = list;
    printf("\tDEBUG Function[Cache_List_Remove_Last] : { list = %p }\n", current);
    while(current->next){
        current = current->next;
    }

    printf("\tDEBUG Function[Cache_List_Remove_Last] : { current = %p }\n", current);
    if(current == list){
        if(!current->prev){
            if(current->pheader) {
                printf("\t\tDEBUG Function[Cache_List_Remove_Last] : (current = %p) == (list = %p) AND (current->prev = %p) == NULL. Will free current->pheader = %p \n",
                       current, list, current->prev, current->pheader);
                free(current->pheader); //La list est maintenant vide, pas de next, pas de prev
                struct Cache_Block_Header *head = NULL;
                current->pheader = head;                
                current->next = NULL;
                current->prev = NULL;
                return current->pheader;
            }
            else{
                printf("\t\tLa liste cache_list donnee est vide!\n");
                return NULL; //Liste vide
            }
        }
    }
    current = current->prev;
    printf("\tDEBUG Function[Cache_List_Remove_Last] : At beginning { current = %p } will free current->next->pheader = %p AND current->next = %p\n", current, current->next->pheader, current->next);
    free(current->next->pheader);
    free(current->next);
    printf("\tDEBUG Function[Cache_List_Remove_Last] : After free current = %p\n", current);
    current->next = NULL;
    return current->pheader;
}

/*! Retrait d'un élément quelconque */
struct Cache_Block_Header * Cache_List_Remove(struct Cache_List *list, struct Cache_Block_Header *pbh){
    //Positionnnement a la fin de la liste
    struct Cache_List * liste_a_parcourir = list;
    printf("\tDEBUG Function[Cache_List_Remove] : { list = %p }\n", liste_a_parcourir);
    while(liste_a_parcourir->next){
        liste_a_parcourir = liste_a_parcourir->next;
    }

    printf("\tDEBUG Function[Cache_List_Remove] : { liste_a_parcourir = %p }\n", liste_a_parcourir);
    if(liste_a_parcourir == list){
        if(!liste_a_parcourir->prev){
            if(liste_a_parcourir->pheader != pbh){
                printf("\t\tDEBUG Function[Cache_List_Remove] : No Previous AND No Next\n");
                //Existe pas
                return NULL;
            }
            else{
                printf("\t\tDEBUG Function[Cache_List_Remove] : Found! Was only element. List is now empty list\n");
                struct Cache_Block_Header * retour = liste_a_parcourir->pheader;
                struct Cache_Block_Header *head = NULL;
                liste_a_parcourir->pheader = head;  
                liste_a_parcourir->prev = NULL;
                liste_a_parcourir->next = NULL;
                return retour;
            }
        }
    }

    //Parcours de la liste tant qu'il existe un precedent et que le pheader actuel n'est pas celui recherche
    while( (liste_a_parcourir->prev) && (liste_a_parcourir->pheader != pbh)){
        liste_a_parcourir = liste_a_parcourir->prev;
    }

    if(liste_a_parcourir->pheader == pbh){
        struct Cache_Block_Header *retour = liste_a_parcourir->pheader;
        if(liste_a_parcourir == list){

            //Ici liste_a_parcourir est le meme pointeur que list. Si on le supprime le pointeur sur list ne vaudra
            //rien. Du coup, il faut decaler les struct.
            if(!liste_a_parcourir->prev && !liste_a_parcourir->next){
                //Only element
                liste_a_parcourir->pheader = NULL;
                return retour;
            }
            else if(!liste_a_parcourir->prev){
                //Decale tout vers la gauche
                printf("\t\tDEBUG Function[Cache_List_Remove] : Decalage a gauche\n");
                int deplace = 0;
                while(liste_a_parcourir->next) {
                    deplace = 1;
                    liste_a_parcourir->pheader = liste_a_parcourir->next->pheader;
                    liste_a_parcourir = liste_a_parcourir->next;
                }
                if(deplace){
                    liste_a_parcourir->prev->next = NULL;
                    free(liste_a_parcourir);
                }
                return retour;
            }
            else if(!liste_a_parcourir->next){
                //Decale tout vers la droite
                printf("\t\tDEBUG Function[Cache_List_Remove] : Decalage a droite\n");
                int deplace = 0;
                while(liste_a_parcourir->prev) {
                    deplace = 1;
                    liste_a_parcourir->pheader = liste_a_parcourir->prev->pheader;
                    liste_a_parcourir = liste_a_parcourir->prev;
                }
                if(deplace){
                    liste_a_parcourir->next->prev = NULL;
                    free(liste_a_parcourir);
                }
                return retour;
            }
            else{
                printf("\t\tDEBUG Function[Cache_List_Remove] : Decalage a aleatoire(a gauche)\n");
                int deplace = 0;
                while(liste_a_parcourir->next) {
                    deplace = 1;
                    liste_a_parcourir->pheader = liste_a_parcourir->next->pheader;
                    liste_a_parcourir = liste_a_parcourir->next;
                }
                if(deplace){
                    liste_a_parcourir->prev->next = NULL;
                    free(liste_a_parcourir);
                }
                return retour;
            }
        }
        else {
            if (liste_a_parcourir->prev) {
                liste_a_parcourir->prev->next = liste_a_parcourir->next;
            }
            if (liste_a_parcourir->next) {
                liste_a_parcourir->next->prev = liste_a_parcourir->prev;
            }
            free(liste_a_parcourir);
            return retour;
        }
    }
    else{
        //Not Found
        return NULL;
    }
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
    struct Cache_List * current = list;
    while(current->prev){
        printf("\tDEBUG Function[Cache_List_Remove] : moving at beginning of list { current = %p }\n", current);
        current = current->prev;
    }

    while(current->next){
        printf("\tDEBUG Function[Cache_List_Remove] : cleaning list on { current = %p }\n", current);
        if(!(current == list)){
            free(current->pheader);
            free(current);
        }
        current = current->next;
    }
    free(list->pheader);
    struct Cache_Block_Header *head = NULL;
    current->pheader = head;  
    list->prev = NULL;
    list->next = NULL;
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list){
    return (list->pheader == NULL) && (list->next == NULL) && (list->prev == NULL);
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list,struct Cache_Block_Header *pbh){
    struct Cache_Block_Header * cbh = Cache_List_Remove(list, pbh);
    Cache_List_Append(list, cbh);
}

/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,struct Cache_Block_Header *pbh){
    struct Cache_Block_Header * cbh = Cache_List_Remove(list, pbh);
    Cache_List_Prepend(list, cbh);
}