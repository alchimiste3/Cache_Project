#ifndef TEST_CACHE_LIST

#include <stdio.h>
#include "cache_list.h"
#include <stddef.h>
#include "cache_list.h"
#include "low_cache.h"

//Test de chaque méthode

void print_debut_test(const char * methode);
void print_fin_test(const char * methode);
void afficher_struct_cache_list(struct Cache_List * list);

/*! Création d'une liste de blocs */
void test_Cache_List_Create(){
    print_debut_test("test_Cache_List_Create");
    struct Cache_List * cache_list = Cache_List_Create();
    printf("L'adresse de la structure cache creer %p\n", cache_list);
    afficher_struct_cache_list(cache_list);

    print_fin_test("test_Cache_List_Create");
}

/*! Destruction d'une liste de blocs */
void test_Cache_List_Delete(){
    print_debut_test("test_Cache_List_Delete");
    struct Cache_List * cache_list;
    printf("Creation d'une struct cache_list\n");
    cache_list = Cache_List_Create();
    afficher_struct_cache_list(cache_list);

    Cache_List_Delete(cache_list);

    afficher_struct_cache_list(cache_list);
    print_fin_test("test_Cache_List_Delete");
}

/*! Insertion d'un élément à la fin */
void test_Cache_List_Append(){
    print_debut_test("test_Cache_List_Append");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header cache_block_header1;
    cache_block_header1.data = "data1";
    cache_block_header1.ibcache = 11;
    cache_block_header1.ibfile = 12;
    cache_block_header1.flags = 13;

    struct Cache_Block_Header cache_block_header2;
    cache_block_header2.data = "data2";
    cache_block_header2.ibcache = 21;
    cache_block_header2.ibfile = 22;
    cache_block_header2.flags = 23;

    struct Cache_Block_Header cache_block_header3;
    cache_block_header3.data = "data3";
    cache_block_header3.ibcache = 31;
    cache_block_header3.ibfile = 32;
    cache_block_header3.flags = 33;

    struct Cache_Block_Header cache_block_header4;
    cache_block_header4.data = "data4";
    cache_block_header4.ibcache = 41;
    cache_block_header4.ibfile = 42;
    cache_block_header4.flags = 43;

    printf("###Affichage de la structure avant ajout###\n");
    afficher_struct_cache_list(cache_list);

    //Ajout
    Cache_List_Append(cache_list, &cache_block_header1);
    Cache_List_Append(cache_list, &cache_block_header2);
    Cache_List_Append(cache_list, &cache_block_header3);
    Cache_List_Append(cache_list, &cache_block_header4);

    printf("###Affichage de la structure après ajout###\n");
    afficher_struct_cache_list(cache_list);

    print_fin_test("test_Cache_List_Append");
}

/*! Insertion d'un élément au début*/
void test_Cache_List_Prepend(){
    print_debut_test("test_Cache_List_Prepend");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header cache_block_header1;
    cache_block_header1.data = "data1";
    cache_block_header1.ibcache = 11;
    cache_block_header1.ibfile = 12;
    cache_block_header1.flags = 13;

    struct Cache_Block_Header cache_block_header2;
    cache_block_header2.data = "data2";
    cache_block_header2.ibcache = 21;
    cache_block_header2.ibfile = 22;
    cache_block_header2.flags = 23;

    struct Cache_Block_Header cache_block_header3;
    cache_block_header3.data = "data3";
    cache_block_header3.ibcache = 31;
    cache_block_header3.ibfile = 32;
    cache_block_header3.flags = 33;

    struct Cache_Block_Header cache_block_header4;
    cache_block_header4.data = "data4";
    cache_block_header4.ibcache = 41;
    cache_block_header4.ibfile = 42;
    cache_block_header4.flags = 43;

    printf("###Affichage de la structure avant Preprend###\n");
    afficher_struct_cache_list(cache_list);

    //Ajout
    Cache_List_Prepend(cache_list, &cache_block_header1);
    Cache_List_Prepend(cache_list, &cache_block_header2);
    Cache_List_Prepend(cache_list, &cache_block_header3);
    Cache_List_Prepend(cache_list, &cache_block_header4);

    printf("###Affichage de la structure après Prepend###\n");
    afficher_struct_cache_list(cache_list);

    print_fin_test("test_Cache_List_Prepend");
}

/*! Retrait du premier élément */
void test_Cache_List_Remove_First(struct Cache_List *list){
    print_debut_test("test_Cache_List_Remove_First");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header cache_block_header1;
    cache_block_header1.data = "data1";
    cache_block_header1.ibcache = 11;
    cache_block_header1.ibfile = 12;
    cache_block_header1.flags = 13;

    struct Cache_Block_Header cache_block_header2;
    cache_block_header2.data = "data2";
    cache_block_header2.ibcache = 21;
    cache_block_header2.ibfile = 22;
    cache_block_header2.flags = 23;

    struct Cache_Block_Header cache_block_header3;
    cache_block_header3.data = "data3";
    cache_block_header3.ibcache = 31;
    cache_block_header3.ibfile = 32;
    cache_block_header3.flags = 33;

    struct Cache_Block_Header cache_block_header4;
    cache_block_header4.data = "data4";
    cache_block_header4.ibcache = 41;
    cache_block_header4.ibfile = 42;
    cache_block_header4.flags = 43;

    printf("###Affichage de la structure avant ajout###\n");
    afficher_struct_cache_list(cache_list);

    //Ajout
    Cache_List_Prepend(cache_list, &cache_block_header1);
    Cache_List_Prepend(cache_list, &cache_block_header2);
    Cache_List_Prepend(cache_list, &cache_block_header3);
    Cache_List_Prepend(cache_list, &cache_block_header4);

    printf("###Affichage de la structure après ajout###\n");
    afficher_struct_cache_list(cache_list);

    print_fin_test("test_Cache_List_Prepend");
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

void print_debut_test(const char * methode){
    printf("\n***************** Test de la methode %s ******************\n", methode);
}

void print_fin_test(const char * methode){
    printf("\n*****************FIN TEST %s******************\n", methode);
}

void afficher_struct_cache_list(struct Cache_List * list){
    int nb_element = 0;
    struct Cache_List * liste_parcours = list;
    printf("Parcours d'un Cache_List\n");
    while(liste_parcours){
        printf("\tParcours de l'élément %d à l'adresse %p %p\n", nb_element, liste_parcours, liste_parcours->pheader);
        if(nb_element != 0) {
            printf("\t\tibFile == %d\n", liste_parcours->pheader->ibfile);
            printf("\t\tibFile == %d\n", liste_parcours->pheader->ibcache);
            printf("\t\tibFile == %d\n", liste_parcours->pheader->flags);
            printf("\t\tibFile == %s\n", liste_parcours->pheader->data);
        }
        nb_element++;
        liste_parcours = liste_parcours->next;
    }
}

int main(int argc, char * argv[]){
    //Lancer un test
    int i;

    //Test 1 Cache_List_Create()
    /*
    for(i = 0; i < 4; i++)
        test_Cache_List_Create();
    */

    //Test 2 Cache_List_Delete()
    //test_Cache_List_Delete();

    //Test 3 test_Cache_List_Append
    //test_Cache_List_Append();

    //Test 3 test_Cache_List_Prepend
    test_Cache_List_Prepend();
}

#endif


