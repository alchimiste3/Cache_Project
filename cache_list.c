#include <stdlib.h>
#include "cache_list.h"

/* A l'initialisation nous avons un bloc qui pointe vers lui même car il s'agit d'une liste circulaire */
struct Cache_List *Cache_List_Create() {
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
    struct Cache_List * current;    //La cellule courante
    struct Cache_Block_Header * pbh;

    current = list->next;
    if (current == list) {
        return NULL; //Liste vide
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    pbh = current->pheader;
    free(current);
    return pbh;
}

/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list)
{
    struct Cache_Block_Header *pbh;
    struct Cache_List *current;

    current = list->prev;
    if (current == list){
        return NULL;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    pbh = current->pheader;
    free(current);
    return pbh;
}

/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,
                                             struct Cache_Block_Header *pbh)
{
    struct Cache_List *current;

    for (current = list->next; current != list; current = current->next)
    {
        if (current->pheader == pbh)
        {
            if (current == list){
                return NULL;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            return pbh;
        }  
    }

    return NULL;   
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list)
{
    while ((list->next) != list) {
        (void) Cache_List_Remove_First(list);
    }
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list)
{
    if(list == list->next){
        return true;
    }
    return false;
    
}


/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list, struct Cache_Block_Header *pbh) {
    struct Cache_Block_Header *pbh1;

    if (list->prev->pheader == pbh){
        return;
    }

    pbh1 = Cache_List_Remove(list, pbh);

    if (pbh1 == NULL){
        pbh1 = pbh;
    }

    Cache_List_Append(list, pbh1);
}

/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,
                              struct Cache_Block_Header *pbh) {
    struct Cache_Block_Header *pbh1;

    if ((list->next)->pheader == pbh){
        return;
    }

    pbh1 = Cache_List_Remove(list, pbh);

    if (pbh1 == NULL){

        pbh1 = pbh;
    }

    Cache_List_Prepend(list, pbh1);
}