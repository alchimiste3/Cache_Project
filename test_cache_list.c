#ifndef TEST_CACHE_LIST

#include <stdio.h>
#include "cache_list.h"
#include <stddef.h>	

//Test de chaque méthode

/*! Création d'une liste de blocs */
void test_Cache_List_Create(){
    
}

/*! Destruction d'une liste de blocs */
void test_Cache_List_Delete(struct Cache_List *list){
    
}

/*! Insertion d'un élément à la fin */
void test_Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
    
}

/*! Insertion d'un élément au début*/
void test_Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh){
    
}

/*! Retrait du premier élément */
void test_Cache_List_Remove_First(struct Cache_List *list){
    
}

/*! Retrait du dernier élément */
void test_Cache_List_Remove_Last(struct Cache_List *list){
    
}

/*! Retrait d'un élément quelconque */
void test_Cache_List_Remove(struct Cache_List *list,
                                             struct Cache_Block_Header *pbh){
    
}

/*! Remise en l'état de liste vide */
void test_Cache_List_Clear(struct Cache_List *list){
    
}

/*! Test de liste vide */
void test_Cache_List_Is_Empty(struct Cache_List *list){
    
}

/*! Transférer un élément à la fin */
void test_Cache_List_Move_To_End(struct Cache_List *list,
                            struct Cache_Block_Header *pbh){
    
}

/*! Transférer un élément  au début */
void test_Cache_List_Move_To_Begin(struct Cache_List *list,
                              struct Cache_Block_Header *pbh){
    
}

int main(){
    //Lancer un test
}

#endif

