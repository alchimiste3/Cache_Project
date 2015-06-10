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
 	free(current);	//On supprime le dernier élèment de la liste
 }

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
	if(list->pheader == NULL){
        printf("\t\tDEBUG Function[Cache_List_Append] : { list->pheader = %p AND pbh = %p } ajout de pbh here\n", list->pheader, pbh);
        list->pheader = pbh;
        list->next = list->next;
        list->prev = list->prev;
    }
    else{
        struct Cache_List *current = list;
        while(current->next){
            printf("\t\t\tDEBUG Function[Cache_List_Append] : INSIDE! { current = %p AND current->next = %p }\n", current, current->next);
            current = current->next;
        }

        //Création du nouvel élément
        struct Cache_List *newElem = malloc(sizeof(struct Cache_List));
        newElem->pheader = pbh;
        newElem->next = NULL;
        newElem->prev = current;

        current->next = newElem;	//Insertion de l'élément à la fin de la liste
        printf("\t\t\tDEBUG Function[Cache_List_Append] : AJOUT FINI { current = %p AND current->prev = %p AND current->next = %p }\n", current, current->prev, current->next);
    }
}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh){
	if(list->pheader == NULL){
        printf("\t\tDEBUG Function[Cache_List_Prepend] : { list->pheader = %p AND pbh = %p } ajout de pbh here\n", list->pheader, pbh);
        list->pheader = pbh;
        list->next = list->next;
        list->prev = list->prev;
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
	while(current->prev){
		current = current->prev;
	}
	(current->next)->prev = NULL;
	free(current);
    //TODO return
    return NULL;
}

/*! Retrait du dernier élément 
 * Supprime le dernier élément de la liste.
 * \Retourne 
 */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
    if(Cache_List_Is_Empty(list)){
        perror("La liste cache_list donnee est vide!");
        exit(1);
    }
    
    struct Cache_Block_Header * header;
    
    struct Cache_List * last = NULL;    //Le dernier élément de la liste, s'il existe
    
    struct Cache_List * list_pour_parcours = list;
    
    //Récupère le dernier de la liste
    while(list_pour_parcours->next != NULL){
        last = list_pour_parcours->next;
        list_pour_parcours = list_pour_parcours->next;
    }
    
    if(last == NULL){
        //On avait une liste d'un élément
        header = list->pheader;
        Cache_List_Clear(list);
        return header;
    }
    
    //Re-structuration de la liste
    struct Cache_List * new_Last = last->prev;
    new_Last->next = NULL;
    
    return last->pheader;
}

/*! Retrait d'un élément quelconque */
struct Cache_Block_Header * Cache_List_Remove(struct Cache_List *list, struct Cache_Block_Header *pbh){
    if(Cache_List_Is_Empty(list)){
        perror("La liste cache_list donnee est vide!");
        exit(1);
    }
    
    struct Cache_List * liste_a_parcourir = list;
    struct Cache_Block_Header * cbh;
    while((liste_a_parcourir = liste_a_parcourir->next) != NULL){
        if(liste_a_parcourir->pheader == pbh){
            //remove
            struct Cache_List * prev = liste_a_parcourir->prev;
            struct Cache_List * next = liste_a_parcourir->next;
            prev->next = next;
            next->prev = prev;
            return liste_a_parcourir->pheader;
        }
    }
    return liste_a_parcourir->pheader;
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
    list->pheader = NULL;
    list->prev = NULL;
    list->next = NULL;
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list){
    return (list->next == NULL) && (list->prev == NULL);
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