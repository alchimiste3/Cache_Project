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

 	//on part de la fin de la liste et on supprime tout
 	while(current->prev){	
 		current = current->prev;
 		free(current->next);
 	}

 	free(current);	//On supprime le dernier élèment de la liste
 }

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
	struct Cache_List *current = list;
	while(current->next){
		current = current->next;
	}
	
	//Création du nouvel élément
	struct Cache_List *newElem = malloc(sizeof(struct Cache_List));
	newElem->pheader = pbh;
	newElem->next = NULL;
	newElem->prev = list;

	list->next = newElem;	//Insertion de l'élément à la fin de la liste

}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh){
	//Création du nouvel élément
	struct Cache_List *newElem = malloc(sizeof(struct Cache_List));
	newElem->pheader = pbh;
	newElem->next = list;
	newElem->prev = NULL;

	//On se positionne au début de la liste
	struct Cache_List *current = list;
	while(current->prev){
		current = current->prev;
	}

	current->prev = newElem; //On ajoute l'élément à la fin de la liste
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