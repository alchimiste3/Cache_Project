#include <stdlib.h>
#include "cache_list.h"

/*! Création d'une liste de blocs
 * C'est une liste Circulaire
 * */
struct Cache_List *Cache_List_Create() {
    struct Cache_List *list = malloc(sizeof(struct Cache_List));
    
    list->pheader = NULL;
    list->next = list->prev = list;

    return list;
}

/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list) {
    Cache_List_Clear(list);
    free(list);
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh) {
    struct Cache_List *current = malloc(sizeof(struct Cache_List));

    //Ajout au prochain du dernier
    current->pheader = pbh;
    current->next = list;   //le prochain de la nouvelle cellule est la list
    current->prev = list->prev; //le precedent de la nouvelle cellule est le precedent de list
    list->prev->next = current;
    list->prev = current;

    if (list->next == list) {
        list->next = current;
    }
}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh) {
    struct Cache_List * current = malloc(sizeof(struct Cache_List));

    //Ajout au suivant de liste
    current->pheader = pbh;
    current->next = list->next;
    current->prev = list;
    list->next->prev = current;
    list->next = current;

    /* cas de la liste initialement vide */
    if (list->prev == list) list->prev = current;
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
    struct Cache_List *current;
    struct Cache_Block_Header *pbh;

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
    while (list->next != list) {
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

    if (list->prev->pheader == pbh)
        return;
    pbh1 = Cache_List_Remove(list, pbh);
    if (pbh1 == NULL) pbh1 = pbh;
    Cache_List_Append(list, pbh1);
}

/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,
                              struct Cache_Block_Header *pbh) {
    struct Cache_Block_Header *pbh1;

    if (list->next->pheader == pbh)
        return;
    pbh1 = Cache_List_Remove(list, pbh);
    if (pbh1 == NULL)
        pbh1 = pbh;
    Cache_List_Prepend(list, pbh1);
}