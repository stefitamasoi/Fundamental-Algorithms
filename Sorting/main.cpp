#include <iostream>
#include "Profiler.h"

#define LENGTH_A 5
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("Sortari");
/*
BEST CASE (ascending) ATRIBUIRI
insertion -->  2(n-1) atribuiri pt ca nu intra in while dar se fac atribuiri de doua ori de la 1 pana la n-1 inclusiv
selection, bubble --> 0 pt ca daca e deja sortat nu mai face deloc swap

BEST CASE (ascending) COMPARATII
 insertion, bubble --> n-1 comparatii cand intra in for (in while nu intra)
 selection --> 10(n-1) intra in ambele for uri

AVG CASE (unsorted) ATRIBUIRI
 selection are cele mai putine atribuiri si bubble cele mai multe

AVG CASE (unsorted) COMPARATII
 selection are cele mai putine comp si bubble cele mai multe

WORST CASE (descending la insertion si bubble si rotated_once la selection)
 selection --> 0 atribuiri
*/

//insertion sort presupune o parte sortata(stanga) si una nesortata(dreapta)
//scopul fiind ordonarea crescatoare a unui sir
//sortarea incepe de la al doilea element al sirului(current) si il compara cu elementul anterior, pana la ultimul elem inclusiv
//cat timp nu am iesit din sir si cat timp current e mai mic decat elementele anterioare, punem previous pe pozitia curenta
//facem acest lucru iar la final pe pozitia prev+1 adaugam elementul initial cu care am operat (current)
//--> 2 '7' 1 9 5
//--> 2 7 1 9 5
//--> 2 7 '1' 9 5
//--> 2 7 7 9 5
//--> 2 2 7 9 5
//--> 1 2 7 9 5
//--> 1 2 7 '9' 5
//--> 1 2 7 9 '5'
//--> 1 2 7 9 9
//--> 1 2 7 7 9
//--> 1 2 5 7 9

//best case  --> SIR CRESCATOR
//worst case --> SIR DESCRESCATOR

void insertion_sort(int *a, int n){
    Operation opComp = p.createOperation("insertion_comp", n);
    Operation opAtrib = p.createOperation("insertion_atrib", n);
    int current, prev_poz;
    for(int i=1; i<n; i++){
        opAtrib.count();
        current = a[i];
        prev_poz = i - 1;
        opComp.count();
        while(prev_poz >= 0 && current < a[prev_poz]){
            opComp.count();
            opAtrib.count();
            a[prev_poz+1] = a[prev_poz];
            prev_poz --;
        }
        opAtrib.count();
        a[prev_poz+1] = current;
    }
}

void demo_insertion_sort(){
    int a[LENGTH_A] = {2, 7, 1, 9, 5};
    insertion_sort(a, LENGTH_A);
    printf("Sirul sortat cu insertion sort: ");
    for(int i=0; i<LENGTH_A; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//la selection avem o parte sortata si una nesortata
//initilizam pozitia minimului cu min (primul elem) si presupunem ca e ok
//mergem la urmatorul elem pana la n ca sa gasim minimul din partea nesortata adica cel care e mai mic decat minimul presupus
//daca gasim un numar mai mic, poz elem minim va fi cea gasita
//daca poz initial presupusa este diferita de cea gasita ulterior facem swap intre cele doua elemente
//--> '2' 7 -1- 9 5
//--> 1 7 2 9 5
//--> 1 '7' -2- 9 5
//--> 1 2 7 9 5
//--> 1 2 '7' 9 -5-
//--> 1 2 5 7 9
//--> 1 2 5 '7' 9
//--> 1 2 5 7 9

//best case  --> SIR SORTAT CRESC
//worst case --> ROTIT O DATA, de ex: 7 1 2 3 4 5 6
void selection_sort(int *a, int n){
    Operation opComp = p.createOperation("selection_comp", n);
    Operation opAtrib = p.createOperation("selection_atrib", n);
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

void demo_selection_sort(){
    int a[LENGTH_A] = {2, 7, 1, 9, 5};
    selection_sort(a, LENGTH_A);
    printf("Sirul sortat cu selection sort: ");
    for(int i=0; i<LENGTH_A; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//bubble sort compara 2 cate doua pana cand sirul este sortat crescator
//dupa prima accesare a for ului maximul ajunge pe ultima pozitie deci putem verifica ulterior cu un elem in minus
//--> '2' '7' 1 9 5
//--> 2 7 1 9 5
//--> 2 '7' '1' 9 5
//--> 2 1 7 9 5
//--> 2 1 '7' '9' 5
//--> 2 1 7 9 5
//--> 2 1 7 '9' '5'
//--> 2 1 7 5 9
//--> '2' '1' 7 5 9
//--> 1 2 7 5 9
//--> etc

//best case  -->SIR ORDONAT CRESCATOR
//worst case -->SIR ORDONAT DESCRESCATOR

void bubble_sort(int *a, int n){
    Operation opComp = p.createOperation("bubble_comp", n);
    Operation opAtrib = p.createOperation("bubble_atrib", n);
    bool ok = false;
    int aux;
    while(ok == false){
        ok = true;
        for(int i=0; i<n-1; i++) {
            opComp.count();
            if (a[i + 1] < a[i]) {
                opAtrib.count(3);
                aux = a[i + 1];
                a[i + 1] = a[i];
                a[i] = aux;
                ok = false;
            }
        }
        n--;
    }
}

void demo_bubble_sort(){
    int a[LENGTH_A] = {2, 7, 1, 9, 5};
    bubble_sort(a, LENGTH_A);
    printf("Sirul sortat cu bubble sort: ");
    for(int i=0; i<LENGTH_A; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void test(int sorted){
    int a[MAX_SIZE];
    int n;
    int copie[MAX_SIZE];

    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE){
        for(int test=0; test<NR_TESTS; test++){
            FillRandomArray(a, n, 10, 50000, false, sorted);
            for(int i=0;i<n;i++){
                copie[i] = a[i];
            }
            insertion_sort(copie,n);
            for(int i=0;i<n;i++){
                copie[i] = a[i];
            }
            bubble_sort(copie,n);
        }

        if(sorted==DESCENDING){
            for(int i=0;i<n;i++){
                copie[i] = a[i];
            }
            FillRandomArray(copie, n, 10, 50000, false, ASCENDING);
            int last = copie[MAX_SIZE-1];
            for(int i=MAX_SIZE-1; i>0; i--){
                copie[i] = copie[i-1];
            }
            copie[0] = last;
            selection_sort(copie,n);
        }
        else{
            FillRandomArray(a, n, 10, 50000, false, sorted);
            for(int i=0;i<n;i++){
                copie[i] = a[i];
            }
            selection_sort(copie,n);
        }
    }
    p.divideValues("insertion_comp", NR_TESTS);
    p.divideValues("insertion_atrib", NR_TESTS);
    p.addSeries("insertion", "insertion_comp", "insertion_atrib");

    p.divideValues("selection_comp", NR_TESTS);
    p.divideValues("selection_atrib", NR_TESTS);
    p.addSeries("selection", "selection_comp", "selection_atrib");

    p.divideValues("bubble_comp", NR_TESTS);
    p.divideValues("bubble_atrib", NR_TESTS);
    p.addSeries("bubble", "bubble_comp", "bubble_atrib");

    p.createGroup("comp", "insertion_comp", "selection_comp", "bubble_comp");
    p.createGroup("atrib", "insertion_atrib", "selection_atrib", "bubble_atrib");
    p.createGroup("total", "insertion", "selection", "bubble");
    p.showReport();
}

int main() {
    //demo_insertion_sort();
    //demo_selection_sort();
    //demo_bubble_sort();
    test(ASCENDING);
    //test(UNSORTED);
    //test(DESCENDING);
    return 0;
}
