#include "cache_list.h"
#include <stddef.h>	

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){
	struct Cache_List *listVide = malloc(sizeof(struct Cache_List));

	struct Cache_Block_Header *head;
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
 		*(current->next) = NULL;
 	}

 	*current = NULL;	//On supprime le dernier élèment de la liste
 }

// /*! Insertion d'un élément à la fin */
// void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh);
// /*! Insertion d'un élément au début*/
// void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh);

// /*! Retrait du premier élément */
// struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list);
// /*! Retrait du dernier élément */
// struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list);
// /*! Retrait d'un élément quelconque */
// struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,
//                                              struct Cache_Block_Header *pbh);

// /*! Remise en l'état de liste vide */
// void Cache_List_Clear(struct Cache_List *list);

// /*! Test de liste vide */
// bool Cache_List_Is_Empty(struct Cache_List *list);

// /*! Transférer un élément à la fin */
// void Cache_List_Move_To_End(struct Cache_List *list,
//                             struct Cache_Block_Header *pbh);
// /*! Transférer un élément  au début */
// void Cache_List_Move_To_Begin(struct Cache_List *list,
//                               struct Cache_Block_Header *pbh);