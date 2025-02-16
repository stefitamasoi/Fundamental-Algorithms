#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100

Profiler p("interclasare");

/**
* @author Tamasoi Stefania
* @group 30229
*
* INTERCLASAREA K LISTE SORTATE
*
* algoritmul creeaza in mod random k liste care au in total n elemente. Dimensiuni generate in interv (n/2*k, n/k)
* initilal, se extrag primele elemente din fiecare lista si se formeaza un min heap din acestea
* se extrage minimul(root ul) si se pune intr o lista finala, si se continua parcurgerea de la lista curenta (unde a fost gasit min anterior)
* daca lista respectiva ramane fara elemente, se pune ultima lista pe pozitia acesteia
*
* COMPLEXITATE
*
* complexitate O(nlogk)
* initializarea listelor si crearea min heapului --> O(K)
* extragem minimul din min heap si il adaugam in lista finala --> O(1)
* HEAPIFY --> O(logk) pt ca inaltimea max este logk
* repetam pt toate celelalte n elemente
*
*/

typedef struct node {
    int data;
    struct node *next;
} node;

typedef struct IntLinkedList {
    node *first;
    node *last;
} IntLinkedList;

IntLinkedList *intLinkedList_new() {
    IntLinkedList *list = (IntLinkedList*)(malloc(sizeof(IntLinkedList)));
    list->first = NULL;
    list->last = NULL;
    return list;
}

void intLinkedList_print(IntLinkedList *list) {
    if (list == NULL || list->first == NULL) {
        //printf("Empty list\n");
        return;
    }
    node *current = list->first;
    while (current != NULL) {
        //printf("%d ", current->data);
        current = current->next;
    }
    //printf("\n");
}

void intLinkedList_push_back(IntLinkedList *list, int value, Operation opComp, Operation opAtrib) {
    node *new_node = (node*)(malloc(sizeof(node)));
    opAtrib.count();
    new_node->data = value;
    new_node->next = NULL;
    if (list->first == NULL) {
        list->first = new_node;
        list->last = new_node;
    }
    else {
        list->last->next = new_node;
        list->last = new_node;
    }
}

void sortare(IntLinkedList *list, Operation opComp, Operation opAtrib) {
    if (list == NULL || list->first == NULL) {
        return;
    }
    node *current = list->first;
    while (current != NULL) {
        node *temp = current->next;
        opComp.count();
        while (temp != NULL) {
            opComp.count();
            if (current->data > temp->data) {
                opAtrib.count(3);
                int tempData = current->data;
                current->data = temp->data;
                temp->data = tempData;
            }
            temp = temp->next;
        }
        current = current->next;
    }
}

int intLinkedList_size(IntLinkedList *list)
{
    if(list == NULL)
    {
        //printf("Empty list!\n");
        return 0;
    }
    int count = 0;
    node *current = list->first;
    while(current!=NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

void min_heapify(IntLinkedList *list, int heap_size, int i, Operation opComp, Operation opAtrib){
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < heap_size){
        opComp.count();
        if (list[left].first->data < list[i].first->data){
            smallest = left;
        }
    }
    else{
        smallest = i;
    }
    if(right < heap_size){
        opComp.count();
        if(list[right].first->data < list[smallest].first->data){
            smallest = right;
        }
    }

    if(smallest != i){
        opAtrib.count(3);
        node *aux = list[i].first;
        list[i].first = list[smallest].first;
        list[smallest].first = aux;
        min_heapify(list, heap_size, smallest, opComp, opAtrib);
    }
}

void build_min(IntLinkedList *list, int heap_size, Operation opComp, Operation opAtrib){
    for(int i=(heap_size-1)/ 2 ; i>=0; i--){
        min_heapify(list, heap_size, i, opComp, opAtrib);
    }
}

void generate_one_sorted_list(IntLinkedList *list, int k, Operation opComp, Operation opAtrib) {
    int i;
    for (i = 0; i < k; i++) {
        int value = 1 + rand() % 100;
        intLinkedList_push_back(list, value,opComp,opAtrib);

    }
    sortare(list, opComp, opAtrib);
}

void generare_k_liste(IntLinkedList *list, int k, int n, Operation opComp, Operation opAtrib) {
    int size_liste[k];
    for (int i = 0; i < k; ++i) {
        list[i] = *intLinkedList_new();
    }
    for (int i = 0; i < k - 1; i++) {
        size_liste[i] = n / (2 * k) + rand() % (n / k - n / (2 * k) + 1);
        n = n - size_liste[i];
    }
    size_liste[k - 1] = n;
    for (int i = 0; i < k; i++) {
        generate_one_sorted_list(&list[i], size_liste[i], opComp, opAtrib);
    }


}

int find_min(IntLinkedList *list) {
    if (list->first == NULL) {
        return -1;
    }
    int min = list->first->data;
    node *current = list->first;
    while (current != NULL) {
        if (current->data < min) {
            min = current->data;
        }
        current = current->next;
    }
    return min;
}

int heap_extract_min(IntLinkedList *list,int *heap_size, Operation opComp, Operation opAtrib) {
    if (*heap_size <= 0) {
        printf("underflow!\n");
        return -1;
    }

    int min = list[0].first->data;
    list[0].first = list[0].first->next;

    if(list[0].first == NULL){
        opAtrib.count(3);
        node *aux = list[0].first;
        list[0].first = list[(*heap_size)-1].first;
        list[(*heap_size)-1].first = aux;
        (*heap_size)--;
    }
    min_heapify(list, *heap_size, 0, opComp, opAtrib);
    return min;
}

void lista_iesire(IntLinkedList *list, int value){
    node *new_node = (node*) (malloc(sizeof(node)));
    new_node->data = value;
    new_node->next = NULL;
    if (list->first == NULL){
        list->first = new_node;
        list->last = new_node;
    }
    else{
        list->last->next = new_node;
        list->last = new_node;
    }
}

void interclasare(int k, int n, Operation opComp, Operation opAtrib) {
    IntLinkedList lists[k];
    IntLinkedList *lista_finala = intLinkedList_new();
    int size = intLinkedList_size(lista_finala);

    generare_k_liste(lists, k, n,opComp,opAtrib);
    build_min(lists, k,opComp,opAtrib);

    while (k>0) {
        int min = heap_extract_min(lists, &k,opComp,opAtrib);
        lista_iesire(lista_finala, min);
        //printf("Min: %d\n", min);
        intLinkedList_print(lista_finala);
    }
}

void demo_interclasare() {
    srand(time(NULL));
    int k = 4;
    int n = 20;
    Operation unused = p.createOperation("unused",n); //unused operation
    interclasare(k,n, unused, unused);
}

void test(int sorted){
    int k;
    int n;

    k = 5;
    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE){
        Operation opComp = p.createOperation("comparatii", n);
        Operation opAtrib = p.createOperation("atribuiri", n);
        interclasare(k,n,opComp,opAtrib);
    }
    p.addSeries("interclasare5", "interclasare5_comp", "interclasare5_atrib");

    k = 10;
    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE){
        Operation opComp = p.createOperation("comparatii", n);
        Operation opAtrib = p.createOperation("atribuiri", n);
        interclasare(k,n,opComp,opAtrib);
    }
    p.addSeries("interclasare10", "interclasare10_comp", "interclasare10_atrib");

    k = 100;
    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE){
        Operation opComp = p.createOperation("comparatii", n);
        Operation opAtrib = p.createOperation("atribuiri", n);
        interclasare(k,n,opComp,opAtrib);
    }
    p.addSeries("interclasare100", "interclasare100_comp", "interclasare100_atrib");

    p.createGroup("comp", "interclasare5_comp", "interclasare10_comp", "interclasare100_comp");
    p.createGroup("atrib", "interclasare5_atrib", "heapsort_atrib", "interclasare100_atrib");
    p.createGroup("total", "interclasare5", "interclasare10", "interclasare100");

    p.showReport();
}

int main() {
    //demo_interclasare();
    test(UNSORTED);
    return 0;
}
