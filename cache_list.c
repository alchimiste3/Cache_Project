#include "cache_list.h"
#include <stddef.h>	

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){
	struct Cache_List *listVide;

	struct Cache_Block_Header *head;
	listVide->pheader = head;

	listVide->next = NULL;
	listVide->prev = NULL;

	return listVide;
}

// /*! Destruction d'une liste de blocs */
//  void Cache_List_Delete(struct Cache_List *list){
//  	struct Cache_List* current = list;
//  	while(current){
 		
//  	}
//  }

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh);
/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh);

/*! Retrait du premier élément */
struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list);

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
    return liste_a_parcourir->pheader;s
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
    list->pheaders = NULL;
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