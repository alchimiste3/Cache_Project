#include <stdlib.h>
#include "cache_list.h"

/* A l'initialisation nous avons un bloc qui pointe vers lui même car il s'agit d'une liste circulaire */
struct Cache_List *Cache_List_Create() {

    //On crée une sentinelle qui représente la liste vide
    struct Cache_List *list = malloc(sizeof(struct Cache_List));
    
    list->pheader = NULL;
    list->next = list;
    list->prev = list;

    return list;
}

/*! On supprime la liste de bloc et on libère la mémoire */
void Cache_List_Delete(struct Cache_List *list) {
    Cache_List_Clear(list);
    free(list);
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh) {
    struct Cache_List *newElem = malloc(sizeof(struct Cache_List));

    //Ajout au prochain du dernier
    newElem->pheader = pbh;
    newElem->next = list;   //le prochain de la nouvelle cellule est la list
    newElem->prev = list->prev; //le precedent de la nouvelle cellule est le precedent de list
    list->prev->next = newElem;
    list->prev = newElem;

    if (list->next == list) {
        list->next = newElem;
    }
}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh) {
    struct Cache_List * newElem = malloc(sizeof(struct Cache_List));

    //Ajout au suivant de liste
    newElem->pheader = pbh;
    newElem->next = list->next;
    newElem->prev = list;

    list->next->prev = newElem;
    list->next = newElem;

    /* cas de la liste initialement vide */
    if (list->prev == list){
        list->prev = newElem;
    }
}

/*! Retrait du premier élément */
struct Cache_Block_Header * Cache_List_Remove_First(struct Cache_List *list) {

    struct Cache_List * current;
    struct Cache_Block_Header * pbh;

    current = list->next;
    if (current == list) { //Si la liste est vide on retourne NULL
        return NULL; 
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    pbh = current->pheader; //On sauvegarde le header
    free(current);
    return pbh;
}

/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list)
{
    struct Cache_Block_Header *pbh;
    struct Cache_List *current;

    current = list->prev;
    if (current == list){   //Si la liste est vide on retourne NULL
        return NULL;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    pbh = current->pheader; //On sauvegarde le header
    free(current);
    return pbh;
}

/*! Retrait d'un élément dans la liste */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list, struct Cache_Block_Header *pbh) {
    struct Cache_List *current =  list->next;

    for (current; current != list; current = current->next){ //On parcours la liste jusqu'a la fin
        if (current->pheader == pbh){
            //Nous avons trouver l'élément à supprimer nous refaisons donc le chainage
            current->prev->next = current->next;
            current->next->prev = current->prev;

            free(current); //On libère la mémoire
            return pbh;
        }  
    }
    return NULL; //On retourn NULL si on a pas trouvé l'élémeent à supprimer
}

/* Suppression de tous les blocs de la liste pour retourner à la liste vide */
void Cache_List_Clear(struct Cache_List *list)
{
    while ((list->prev) != list) {      //Tant que la liste n'est pas vide
        Cache_List_Remove_Last(list);   //On supprime le dernière élèment
    }
}

/* Retourne true si la liste est vide */
bool Cache_List_Is_Empty(struct Cache_List *list){
    if(list == list->next){ //Si l'élément boucle sur lui même il s'agit de la liste vide
        return true;
    }
    return false;   //Sinon la liste n'est pas vide
}


/* Treansfère d'un élément de la liste à la fin de celle-ci */
void Cache_List_Move_To_End(struct Cache_List *list, struct Cache_Block_Header *pbh) {

    struct Cache_Block_Header *pbh1;

    pbh1 = Cache_List_Remove(list, pbh);

    if (pbh1 == NULL) pbh1 = pbh; //Si l'élement n'as pas été trouvé on l'ajoute à la fin
    
    Cache_List_Append(list, pbh1);
}

/*! Treansfère d'un élément de la liste au début de celle-ci */
void Cache_List_Move_To_Begin(struct Cache_List *list, struct Cache_Block_Header *pbh) {

    struct Cache_Block_Header *pbh1;

    pbh1 = Cache_List_Remove(list, pbh); //On supprime l'élément de la liste

    if (pbh1 == NULL)  pbh1 = pbh; //Si l'élement n'as pas été trouvé on l'ajoute au début

    Cache_List_Prepend(list, pbh1);
}