#include <iostream>
#include "limits.h"
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

int length_a = 8;

Profiler p("Heap");

//worst case pt ambii algoritmi -->sir sortat crescator
//best cazse pt ambii algoritmi -->sir sortat descrescator

//buttom up--> O(nlg)
//top down --> O(lgn)


void max_heapify(int *a, int heap_size, int i, int *comp, int *atrib){
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < heap_size){
        (*comp) ++;
        if (a[left] > a[i]){
            largest = left;
        }
    }
    else{
        largest = i;
    }
    if(right < heap_size){
        (*comp) ++;
        if(a[right] > a[largest]){
            largest = right;
        }
    }

    if(largest != i){
        (*atrib) += 3;
        int aux = a[i];
        a[i] = a[largest];
        a[largest] = aux;
        max_heapify(a, heap_size, largest, comp, atrib);
    }
}

void build_max(int *a, int heap_size){
    Operation opComp = p.createOperation("heap_buttom_up_comp", heap_size);
    Operation opAtrib = p.createOperation("heap_buttom_up_atrib", heap_size);
    int comp = 0;
    int atrib = 0;
    for(int i=(heap_size-1)/ 2 ; i>=0; i--){
        max_heapify(a, heap_size, i, &comp, &atrib);
    }
    opAtrib.count(atrib);
    opComp.count(comp);
}

void heap_sort(int *a, int heap_size){
    int x = 0; //valori nefolosite
    int y = 0;
    build_max(a, heap_size);
    for(int i=length_a-1;i>=1;i--){
        int aux = a[0];
        a[0] = a[i];
        a[i] = aux;
        heap_size--;
        max_heapify(a, heap_size, 0, &x, &y);
    }
}

void demo_build_heap_buttom_up(){
    int a[] = {5, 12, 64, 1, 37, 90, 91, 97};
    int heap_size = length_a;
    heap_sort(a,heap_size);
    printf("Construirea heap ului cu metoda buttom up si heap ul sortat:\n");
    for(int i=0;i<length_a;i++){
        printf("%d ", a[i]);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int heap_maximum(int *a){
    return a[0];
}

int heap_extract_max(int *a, int heap_size) {
    int x = 0;
    int y = 0;
    if (heap_size <= 0) {
        printf("underflow!\n");
        return -1;
    }
   // (*atrib) += 2;
    int max = a[0];
    a[0] = a[heap_size - 1];
    heap_size--;
    max_heapify(a, heap_size, 0, &x,&y);
    return max;
}

void heap_increase_key(int *a, int i, int key){
    //(*comp) ++;
    if(key < a[i]){
        printf("New key is smaller than current key!\n");
        return;
    }
    //(*atrib) ++;
    a[i] = key;
    //(*comp) ++;
    while(i > 0 && a[(i-1)/2] < a[i]){
        //(*comp) ++;
        //(*atrib) += 3;
        int aux = a[i];
        a[i] = a[(i-1)/2];
        a[(i-1)/2] = aux;
        i = (i-1)/2;
    }
}

void max_heap_insert(int *a, int *heap_size, int key) {
    //int x = 0;
   // int y = 0;
    (heap_size)++;
    a[*heap_size-1] = INT_MIN;
    heap_increase_key(a, *heap_size-1, key);
}

void insertion_top_down(int *a, int heap_size){
//    Operation opComp = p.createOperation("heap_buttom_up_comp", heap_size);
//    Operation opAtrib = p.createOperation("heap_buttom_up_atrib", heap_size);
//    int comp = 0;
//    int atrib = 0;
    heap_size = 1;
    for(int i=0;i<length_a; i++){
        max_heap_insert(a, &heap_size, a[i]);
    }
//    opComp.count(comp);
//    opAtrib.count(atrib);
}

void demo_max_priority_queue(){
    int a[] = {5, 12, 64, 1, 37, 90, 91, 97};
    int heap_size = length_a;
//    int x = 0;
//    int y = 0;
    insertion_top_down(a, heap_size);
    for(int i=heap_size-2; i>=0; i--){
        a[heap_size-1] = heap_extract_max(a,heap_size);
        heap_size--;
    }
    for(int i=0;i<length_a;i++){
        printf("%d ", a[i]);
    }
}

void test(int order){
    int a[MAX_SIZE];
    int n;
    int copie[MAX_SIZE];

    for(n = STEP_SIZE; n <= MAX_SIZE; n+=STEP_SIZE){
        for(int test=0; test<NR_TESTS; test++) {
            FillRandomArray(a, n, 10, 50000, false, order);
            for (int i = 0; i < n; i++) {
                copie[i] = a[i];
            }
            length_a += 100;
            build_max(copie, n);

//            for (int i = 0; i < n; i++) {
//                copie[i] = a[i];
//            }
//            length_a += 100;
//            insertion_top_down(copie, n);
        }

    }
    p.divideValues("build_heap_buttom_up_comp", NR_TESTS);
    p.divideValues("build_heap_buttom_up_atrib", NR_TESTS);
    p.addSeries("build_heap_buttom_up", "build_heap_buttom_up_comp", "build_heap_buttom_up_atrib");
//
//    p.divideValues("build_heap_top_down_comp", NR_TESTS);
//    p.divideValues("build_heap_top_down_atrib", NR_TESTS);
//    p.addSeries("build_heap_top_down", "build_heap_top_down_comp", "build_heap_top_down_atrib");
//
//    p.createGroup("comp", "build_heap_buttom_up_comp", "build_heap_top_down_comp");
//    p.createGroup("atrib", "build_heap_buttom_up_atrib", "build_heap_top_down_atrib");
//    p.createGroup("total", "build_heap_buttom_up", "build_heap_top_down");

    p.showReport();
}

int main() {
    //demo_build_heap_buttom_up();
    //demo_max_priority_queue();
    test(ASCENDING);
    return 0;
}
