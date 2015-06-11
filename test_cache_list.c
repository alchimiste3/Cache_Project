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
void print_around_dieze(const char * message);

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
    printf("\n\nCall 1 cache_list = %p AND cache_block_header1 = %p\n", cache_list, &cache_block_header1);
    Cache_List_Prepend(cache_list, &cache_block_header1);
    printf("\n\nCall 2 cache_list = %p AND cache_block_header2 = %p\n", cache_list, &cache_block_header2);
    Cache_List_Prepend(cache_list, &cache_block_header2);
    printf("\n\nCall 3 cache_list = %p AND cache_block_header3 = %p\n", cache_list, &cache_block_header3);
    Cache_List_Prepend(cache_list, &cache_block_header3);
    printf("\n\nCall 4 cache_list = %p AND cache_block_header4 = %p\n", cache_list, &cache_block_header4);
    Cache_List_Prepend(cache_list, &cache_block_header4);

    printf("###Affichage de la structure après Prepend###\n");
    afficher_struct_cache_list(cache_list);

    print_fin_test("test_Cache_List_Prepend");
}

/* Test des methodes precedente */
void test_Cache_List_Create_Append_Prepend_Delete(){
    print_debut_test("test_Cache_List_Create_Append_Prepend_Delete");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header * cache_block_header1 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header1->data = "data1";
    cache_block_header1->ibcache = 11;
    cache_block_header1->ibfile = 12;
    cache_block_header1->flags = 13;

    struct Cache_Block_Header * cache_block_header2 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header2->data = "data2";
    cache_block_header2->ibcache = 21;
    cache_block_header2->ibfile = 22;
    cache_block_header2->flags = 23;

    struct Cache_Block_Header * cache_block_header3 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header3->data = "data3";
    cache_block_header3->ibcache = 31;
    cache_block_header3->ibfile = 32;
    cache_block_header3->flags = 33;

    struct Cache_Block_Header * cache_block_header4 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header4->data = "data4";
    cache_block_header4->ibcache = 41;
    cache_block_header4->ibfile = 42;
    cache_block_header4->flags = 43;

    printf("###Affichage de la structure avant ajout###\n");
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    //Ajout
    printf("\n\nCall 1 Append cache_list = %p AND cache_block_header1 = %p\n", cache_list, &cache_block_header1);
    Cache_List_Append(cache_list, cache_block_header1);
    printf("\n\nCall 2 Prepend cache_list = %p AND cache_block_header2 = %p\n", cache_list, &cache_block_header2);
    Cache_List_Prepend(cache_list, cache_block_header2);
    printf("\n\nCall 3 Append cache_list = %p AND cache_block_header3 = %p\n", cache_list, &cache_block_header3);
    Cache_List_Append(cache_list, cache_block_header3);
    printf("\n\nCall 4 Prepend cache_list = %p AND cache_block_header4 = %p\n", cache_list, &cache_block_header4);
    Cache_List_Prepend(cache_list, cache_block_header4);

    printf("###Affichage de la structure après ajout###\n");
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    Cache_List_Delete(cache_list);

    printf("###Affichage de la structure après delete###\n");
    /* ATTENTION : affichage seras effectuer et des caractere bizarre vont surement apparaitre.
     * Comme on a fait un free() de cache_list on n'a plus le droit d'aller lire à cet emplacement memoire
     * Donc la reussite de ce test est un message "invalid read" en lancant valgrind.
     */
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    print_fin_test("test_Cache_List_Create_Append_Prepend_Delete");
}

/*! Retrait du premier élément */
void test_Cache_List_Remove_First(){
    print_debut_test("test_Cache_List_Remove_First");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header * cache_block_header1 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header1->data = "data1";
    cache_block_header1->ibcache = 11;
    cache_block_header1->ibfile = 12;
    cache_block_header1->flags = 13;

    struct Cache_Block_Header * cache_block_header2 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header2->data = "data2";
    cache_block_header2->ibcache = 21;
    cache_block_header2->ibfile = 22;
    cache_block_header2->flags = 23;

    struct Cache_Block_Header * cache_block_header3 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header3->data = "data3";
    cache_block_header3->ibcache = 31;
    cache_block_header3->ibfile = 32;
    cache_block_header3->flags = 33;

    struct Cache_Block_Header * cache_block_header4 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header4->data = "data4";
    cache_block_header4->ibcache = 41;
    cache_block_header4->ibfile = 42;
    cache_block_header4->flags = 43;

    printf("###Affichage de la structure avant ajout###\n");
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    //Ajout
    printf("\n\nCall 1 Append cache_list = %p AND cache_block_header1 = %p\n", cache_list, &cache_block_header1);
    Cache_List_Append(cache_list, cache_block_header1);
    printf("\n\nCall 2 Prepend cache_list = %p AND cache_block_header2 = %p\n", cache_list, &cache_block_header2);
    Cache_List_Append(cache_list, cache_block_header2);
    printf("\n\nCall 3 Append cache_list = %p AND cache_block_header3 = %p\n", cache_list, &cache_block_header3);
    Cache_List_Append(cache_list, cache_block_header3);
    printf("\n\nCall 4 Prepend cache_list = %p AND cache_block_header4 = %p\n", cache_list, &cache_block_header4);
    Cache_List_Append(cache_list, cache_block_header4);


    printf("###Affichage de la structure après remove first###\n");
    cache_list = Cache_List_Remove_First(cache_list);
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_First(cache_list);
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_First(cache_list);
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_First(cache_list);
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    print_fin_test("test_Cache_List_Remove_First");
}

/*! Retrait du dernier élément */
void test_Cache_List_Remove_Last(){
    print_debut_test("test_Cache_List_Remove_Last");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header * cache_block_header1 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header1->data = "data1";
    cache_block_header1->ibcache = 11;
    cache_block_header1->ibfile = 12;
    cache_block_header1->flags = 13;

    struct Cache_Block_Header * cache_block_header2 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header2->data = "data2";
    cache_block_header2->ibcache = 21;
    cache_block_header2->ibfile = 22;
    cache_block_header2->flags = 23;

    struct Cache_Block_Header * cache_block_header3 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header3->data = "data3";
    cache_block_header3->ibcache = 31;
    cache_block_header3->ibfile = 32;
    cache_block_header3->flags = 33;

    struct Cache_Block_Header * cache_block_header4 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header4->data = "data4";
    cache_block_header4->ibcache = 41;
    cache_block_header4->ibfile = 42;
    cache_block_header4->flags = 43;

    printf("###Affichage de la structure avant ajout###\n");
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    //Ajout
    printf("\n\nCall 1 Append cache_list = %p AND cache_block_header1 = %p\n", cache_list, &cache_block_header1);
    Cache_List_Prepend(cache_list, cache_block_header1);
    printf("\n\nCall 2 Prepend cache_list = %p AND cache_block_header2 = %p\n", cache_list, &cache_block_header2);
    Cache_List_Append(cache_list, cache_block_header2);
    printf("\n\nCall 3 Append cache_list = %p AND cache_block_header3 = %p\n", cache_list, &cache_block_header3);
    Cache_List_Prepend(cache_list, cache_block_header3);
    printf("\n\nCall 4 Prepend cache_list = %p AND cache_block_header4 = %p\n", cache_list, &cache_block_header4);
    Cache_List_Prepend(cache_list, cache_block_header4);

    printf("\n#############Affichage des blocks apres les ajouts\n\n");
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_Last(cache_list);
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_Last(cache_list);
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_Last(cache_list);
    afficher_struct_cache_list(cache_list);

    cache_list = Cache_List_Remove_Last(cache_list);
    afficher_struct_cache_list(cache_list);

    printf("###########################################\n\n");

    print_fin_test("test_Cache_List_Remove_Last");
}

/*! Retrait d'un élément quelconque */
void test_Cache_List_Remove(){
    print_debut_test("test_Cache_List_Remove");
    struct Cache_List * cache_list = Cache_List_Create();

    struct Cache_Block_Header * cache_block_header1 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header1->data = "data1";
    cache_block_header1->ibcache = 11;
    cache_block_header1->ibfile = 12;
    cache_block_header1->flags = 13;

    struct Cache_Block_Header * cache_block_header2 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header2->data = "data2";
    cache_block_header2->ibcache = 21;
    cache_block_header2->ibfile = 22;
    cache_block_header2->flags = 23;

    struct Cache_Block_Header * cache_block_header3 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header3->data = "data3";
    cache_block_header3->ibcache = 31;
    cache_block_header3->ibfile = 32;
    cache_block_header3->flags = 33;

    struct Cache_Block_Header * cache_block_header4 = malloc(sizeof(struct Cache_Block_Header));
    cache_block_header4->data = "data4";
    cache_block_header4->ibcache = 41;
    cache_block_header4->ibfile = 42;
    cache_block_header4->flags = 43;

    printf("###Affichage de la structure avant ajout###\n");
    afficher_struct_cache_list(cache_list);
    printf("###########################################\n\n");

    //Ajout
    printf("\n\nCall 1 Append cache_list = %p AND cache_block_header1 = %p\n", cache_list, &cache_block_header1);
    Cache_List_Prepend(cache_list, cache_block_header1);
    printf("\n\nCall 2 Prepend cache_list = %p AND cache_block_header2 = %p\n", cache_list, &cache_block_header2);
    Cache_List_Append(cache_list, cache_block_header2);
    printf("\n\nCall 3 Append cache_list = %p AND cache_block_header3 = %p\n", cache_list, &cache_block_header3);
    Cache_List_Prepend(cache_list, cache_block_header3);
    printf("\n\nCall 4 Prepend cache_list = %p AND cache_block_header4 = %p\n", cache_list, &cache_block_header4);
    Cache_List_Append(cache_list, cache_block_header4);

    printf("\n#############Affichage des blocks apres les ajouts\n\n");
    afficher_struct_cache_list(cache_list);

    print_around_dieze("DELETE cache_block_header1");
    cache_list = Cache_List_Remove(cache_list, cache_block_header1);
    afficher_struct_cache_list(cache_list);
    print_around_dieze("");

    print_around_dieze("DELETE cache_block_header2");
    cache_list = Cache_List_Remove(cache_list, cache_block_header2);
    afficher_struct_cache_list(cache_list);
    print_around_dieze("");

    print_around_dieze("DELETE cache_block_header3");
    cache_list = Cache_List_Remove(cache_list, cache_block_header3);
    afficher_struct_cache_list(cache_list);
    print_around_dieze("");

    print_around_dieze("DELETE cache_block_header4");
    cache_list = Cache_List_Remove(cache_list, cache_block_header4);
    afficher_struct_cache_list(cache_list);
    print_around_dieze("");

    printf("###########################################\n\n");

    print_fin_test("test_Cache_List_Remove");
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

void print_around_dieze(const char * message){
    printf("\n########################### %s ###########################\n\n", message);
}

void afficher_struct_cache_list(struct Cache_List * list){

    int nb_element = 0;
    struct Cache_List * liste_parcours = list;
    while(liste_parcours->prev){
        liste_parcours = liste_parcours->prev;
    }

    printf("Parcours d'un Cache_List avec { liste_parcours = %p }\n", liste_parcours);
    while(liste_parcours){
        printf("\tAffichage du cache_block_header %d à l'adresse { liste_parcours = %p AND liste_parcours->pheader = %p }\n", nb_element, liste_parcours, liste_parcours->pheader);
        if(liste_parcours->pheader != NULL) {
            printf("\t\tibFile == %d\n", liste_parcours->pheader->ibfile);
            printf("\t\tibcache == %d\n", liste_parcours->pheader->ibcache);
            printf("\t\tibflags == %d\n", liste_parcours->pheader->flags);
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
    //test_Cache_List_Prepend();

    //Test 4 test_Cache_List_Create_Append_Prepend_Delete
    //test_Cache_List_Create_Append_Prepend_Delete();

    //test_Cache_List_Remove_First
    //test_Cache_List_Remove_First();

    //test_Cache_List_Remove_Last
    //test_Cache_List_Remove_Last();

    //test_Cache_List_Remove
    test_Cache_List_Remove();
}

#endif


