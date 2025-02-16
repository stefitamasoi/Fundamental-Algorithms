#include <iostream>
#include "Profiler.h"
#include <time.h>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
#define NR_TESTS_TIMER 100

Profiler p("Quicksort");

//HEAPSORT
//build max cu array ul de intrare pornin de la mijlocul sirului si apoi functia heapify pentru fiecare nod in sus pana la radacina
//extragem elementele din heap unul cate unul si le punem la sf array ului. heapify iar
//tot asa pana cand sirul e sortat
//complexitate --> O(nlog n) în orice caz => eficient


//QUICKSORT
//aleg un pivot (ultimul pt lomuto)
//parcurg sirul si pun elementele mai mici decat pivotul in stanga si cele mai mari in dreapta
//pivotul ajunge sa fie pe pozitia dorita
//se repeta procesul cu subriruile stanga si dreapta pana sirul e sortat
//complexitatea algortmului --> O(n^2) in worst case (pivotul este mereu cel mai mare sau cel mai mic element) pt ca mereu va merge pe un singur brat
//average case --> O(nlog n)

//quicksort este mai rapid decat heapsort, exceptie facand worst case

//SELECTION SORT ITERATIV VS RECURSIV
//complexitate O(n^2) la ambele


void max_heapify(int *a, int heap_size, int i, Operation opComp, Operation opAtrib){
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < heap_size){
        opComp.count();
        if (a[left] > a[i]){
            largest = left;
        }
    }
    else{
        largest = i;
    }
    if(right < heap_size){
        opComp.count();
        if(a[right] > a[largest]){
            largest = right;
        }
    }

    if(largest != i){
        opAtrib.count(3);
        int aux = a[i];
        a[i] = a[largest];
        a[largest] = aux;
        max_heapify(a, heap_size, largest, opComp, opAtrib);
    }
}

void build_max(int *a, int heap_size, Operation opComp, Operation opAtrib){
    for(int i=(heap_size-1)/ 2 ; i>=0; i--){
        max_heapify(a, heap_size, i, opComp, opAtrib);
    }

}

void heap_sort(int *a, int heap_size){
    Operation opComp = p.createOperation("heapsort_comp", heap_size);
    Operation opAtrib = p.createOperation("heapsort_atrib", heap_size);
    build_max(a, heap_size, opComp, opAtrib);
    for(int i=heap_size-1;i>=1;i--){
        opAtrib.count(3);
        int aux = a[0];
        a[0] = a[i];
        a[i] = aux;
        heap_size--;
        max_heapify(a, heap_size, 0, opComp, opAtrib);
    }
}

void demo_heapsort(){
    int a[] = {5, 12, 64, 1, 37, 90, 91, 97};
    int heap_size = sizeof(a)/sizeof(a[0]);
    heap_sort(a,heap_size);
    printf("Heap-ul sortat:\n");
    for(int i=0;i<heap_size;i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int partition(int *a, int l, int r, int n) {
    Operation opComp = p.createOperation("quicksort_comp", n);
    Operation opAtrib = p.createOperation("quicksort_atrib", n);
    opAtrib.count();
    int x = a[r];
    int i = l - 1;
    for (int j = l; j < r; j++) {
        opComp.count();
        if (a[j] <= x) {
            i++;
            opAtrib.count(3);
            int aux = a[i];
            a[i] = a[j];
            a[j] = aux;
        }
    }
    opAtrib.count(3);
    int aux = a[i + 1];
    a[i + 1] = a[r];
    a[r] = aux;
    return i + 1;
}

void quicksort(int *a, int l, int r, int n) {
    if (l < r) {
        int q = partition(a, l, r, n);
        quicksort(a, l, q - 1, n);
        quicksort(a, q + 1, r,n);
    }
}

void demo_quicksort() {
    int a[] = {5, 12, 64, 1, 37, 90, 91, 97};
    int n= sizeof(a) / sizeof(a[0]);
    int l = 0;
    int r = n - 1;
    quicksort(a, l, r, n);
    printf("Sirul sortat cu quicksort este:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//functia primește limitele intervalului si returneaza un nr random din interval care va fi ulterior pivotul
int random(int l, int r) {
    return l + rand() % (r - l + 1);
}

int randomized_partition(int *a, int l, int r, int n){
    int i = random(l,r);
    int aux = a[r];
    a[r] = a[i];
    a[i] = aux;
    return partition(a,l,r, n);
}

void randomized_quicksort(int *a, int l, int r, int n){
    if(l<r){
        int q = randomized_partition(a,l,r,n);
        randomized_quicksort(a,l,q-1,n);
        randomized_quicksort(a,q+1,r,n);
    }
}

void demo_randomized_quicksort(){
    int a[] = {12, 3, 1, 14, 9, 4, 7, 6};
    int n = sizeof(a) / sizeof(a[0]);
    srand(time(NULL));
    randomized_quicksort(a, 0, n - 1, n);
    printf("Sirul sortat cu randomized quicksort este: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void swap(int *a, int *b){
    int aux = *a;
    *a = *b;
    *b = aux;
}

void quick_bestcase_func(int *a, int l, int r){
    if(l == r || l<0 || l>r){
        return;
    }
    int m = (l + r) / 2;
    quick_bestcase_func(a, l, m-1);
    quick_bestcase_func(a,m+1, r);
    swap(a+r, a+m);
}

void demo_quick_bestcase_func(){
    int a[] = {1, 2, 3, 4, 5, 8};
    int n = sizeof(a)/sizeof(a[0]);
    quick_bestcase_func(a,0,n-1);
    printf("Sirul pe best case este: ");
    for(int i=0;i<n;i++){
        printf("%d ", a[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void selection_sort_iterativ(int *a, int n){
    Operation opComp = p.createOperation("selection_iterativ_comp", n);
    Operation opAtrib = p.createOperation("selection_iterativ_atrib", n);
    int indexMin;
    int aux;
    for(int i=0; i<n-1; i++){
        indexMin = i;
        for(int j=i+1; j<n; j++){
            opComp.count();
            if(a[j] < a[indexMin]){
                indexMin = j;
            }
        }
        if(i != indexMin){
            opAtrib.count(3);
            aux = a[i];
            a[i] = a[indexMin];
            a[indexMin] = aux;
        }
    }
}

void demo_selection_sort_iterativ(){
    int a[] = {2, 7, 1, 9, 5};
    int n = sizeof(a)/sizeof(a[0]);
    selection_sort_iterativ(a, n);
    printf("Sirul sortat cu selection sort iterativ: ");
    for(int i=0; i<n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void selection_sort_recursiv(int *a, int n, int currentIndex){
    Operation opComp = p.createOperation("selection_recursiv_comp", n);
    Operation opAtrib = p.createOperation("selection_recursiv_atrib", n);
    if (currentIndex == n - 1) {
        return;
    }
    int minIndex = currentIndex;
    for (int i = currentIndex + 1; i < n; i++) {
        if (a[i] < a[minIndex]) {
            minIndex = i;
        }
    }
    if (minIndex != currentIndex) {
        opAtrib.count(3);
        swap(&a[minIndex], &a[currentIndex]);
    }
    selection_sort_recursiv(a, n, currentIndex + 1);
}

void demo_selection_sort_recursiv(){
    int a[] = {2, 7, 1, 9, 5};
    int n = sizeof(a)/sizeof(a[0]);
    selection_sort_recursiv(a, n, 0);
    printf("Sirul sortat cu selection sort recursiv: ");
    for(int k=0; k<n; k++){
        printf("%d ", a[k]);
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test(int sorted){
    int a[MAX_SIZE];
    int n;
    int copie[MAX_SIZE];

    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE){
        if(sorted == UNSORTED){
            for(int test=0; test<NR_TESTS; test++){
                FillRandomArray(a, n, 10, 50000, false, sorted);
                for(int i=0;i<n;i++){
                    copie[i] = a[i];
                }
                quicksort(copie,0,n-1,n);
                for(int i=0;i<n;i++){
                    copie[i] = a[i];
                }
                heap_sort(copie, n);

                for(int i=0;i<n;i++){
                    copie[i] = a[i];
                }
                selection_sort_iterativ(copie, n);

                for(int i=0;i<n;i++){
                    copie[i] = a[i];
                }
                selection_sort_recursiv(copie, n, 0);
            }

//            for(int i=0;i<n;i++){
//                copie[i] = a[i];
//            }
//            p.startTimer("selection_sort_iterativ", n);
//              for (int testt = 0; testt < NR_TESTS_TIMER; testt++) {
//                selection_sort_iterativ(copie, n);
//            }
//            p.stopTimer("selection_sort_iterativ", n);
//
//             for(int i=0;i<n;i++){
//                 copie[i] = a[i];
//             }
//             p.startTimer("selection_sort_recursiv", n);
//            for (int testt = 0; testt < NR_TESTS_TIMER; testt++) {
//                selection_sort_recursiv(copie, n, 0);
//            }
//            p.stopTimer("selection_sort_recursiv", n);
//
        }

        if(sorted == ASCENDING){            //worst case pt quicksort
            FillRandomArray(a, n, 10, 50000, false, sorted);
            for(int i=0;i<n;i++){
                copie[i] = a[i];
            }
            quicksort(a,0,n-1,n);
        }
        if(sorted == DESCENDING){           //best case pt quicksort
            for(int test=0; test<NR_TESTS; test++){
                FillRandomArray(a, n, 10, 50000, false, sorted);
                for(int i=0;i<n;i++){
                    copie[i] = a[i];
                }
                quick_bestcase_func(a,0,n-1);
                quicksort(a,0,n-1,n);
            }
        }
    }
    p.divideValues("quicksort_comp", NR_TESTS);
    p.divideValues("quicksort_atrib", NR_TESTS);
    p.addSeries("quicksort", "quicksort_comp", "quicksort_atrib");

    p.divideValues("heapsort_comp", NR_TESTS);
    p.divideValues("heapsort_atrib", NR_TESTS);
    p.addSeries("heapsort", "heapsort_comp", "heapsort_atrib");

    p.createGroup("comp", "quicksort_comp", "heapsort_comp");
    p.createGroup("atrib", "quicksort_atrib", "heapsort_atrib");
    p.createGroup("total", "quicksort", "heapsort");

    p.divideValues("selection_iterativ_comp", NR_TESTS);
    p.divideValues("selection_iterativ_atrib", NR_TESTS);
    p.addSeries("selection_iterativ", "selection_iterativ_comp", "selection_iterativ_atrib");

    p.divideValues("selection_recursiv_comp", NR_TESTS);
    p.divideValues("selection_recursiv_atrib", NR_TESTS);
    p.addSeries("selection_recursiv", "selection_recursiv_comp", "selection_recursiv_atrib");

    p.createGroup("comp_selection", "selection_iterativ_comp", "selection_recursiv_comp");
    p.createGroup("atrib_selection", "selection_iterativ_atrib", "selection_recursiv_atrib");
    p.createGroup("total_selection", "selection_iterativ", "selection_recursiv");

    p.showReport();
}
int main() {
    //demo_heapsort();
    //demo_quicksort();
    //demo_randomized_quicksort();
    //demo_quick_bestcase_func();
    //demo_selection_sort_iterativ();
    //demo_selection_sort_recursiv();
    test(UNSORTED);
    //test(ASCENDING);
    //test(DESCENDING);
    return 0;
}

