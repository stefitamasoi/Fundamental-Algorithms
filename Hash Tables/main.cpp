#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Profiler.h"

Profiler p("hash_table");

#define TABLE_SIZE 11
#define N 10007

typedef struct {
    int id;
    char name[30];
} Entry;

typedef struct {
    Entry *table;
    unsigned size;
} IntHashTable;

Entry *new_entry(int id, char *name) {
    Entry *new_entry = (Entry *) malloc(sizeof(Entry));
    new_entry->id = id;
    strncpy(new_entry->name, name, sizeof(new_entry->name) - 1);
    new_entry->name[sizeof(new_entry->name) - 1] = '\0';
    return new_entry;
}

IntHashTable *intHashTable_new(unsigned size) {
    IntHashTable *table = (IntHashTable *) (malloc(sizeof(IntHashTable)));
    table->size = size;
    table->table = (Entry *) (malloc(size * sizeof(Entry)));
    for (int i = 0; i < size; i++) {
        table->table[i].id = -1;
    }
    return table;
}

void intHashTable_free(IntHashTable *hashTable) {
    free(hashTable);
}

int hash_function(int k, int table_size) {
    return k % table_size;
}

int hash_insert(IntHashTable *T, Entry entry) {
    //int m = T->size;
    int m = TABLE_SIZE;
    int i = 0;
    int k = entry.id;

    do {
        int j = (hash_function(k, m) + 7 * i + 11 * i * i) % m;
        if (T->table[j].id == -1) {
            T->table[j] = entry;
            return j;
        } else {
            i++;
        }
    } while (i < m);

    printf("hash table overflow\n");
    return -1;
}

int hash_search(IntHashTable *T, int k, int *efort) {
    // m = T->size;
    int m = TABLE_SIZE;
    int i = 0;
    while (i < m) {
        int j = (hash_function(k, m) + 7 * i + 11 * i * i) % m;
        if (T->table[j].id == k && strlen(T->table[j].name) > 0) {
            printf("key %d la pos %d avand numele %s\n", k, j, T->table[j].name);
            return j;
        } else {
            i++;
            *efort = i + 1;
        }
    }
    printf("key %d NOT FOUND\n", k);
    *efort = i;
    return -1;
}

void hash_delete(IntHashTable *T, int k, int *efort) {
    int index = hash_search(T, k, efort);
    if (index != -1) {
        T->table[index].id = -1; // -1 inseamna ca locul e gol
        printf("key = %d DELETED!\n", k);
    } else {
        printf("key %d NOT FOUND\n", k);
    }
}

float return_n(float alfa) {
    return alfa * TABLE_SIZE;
}

void demo() {
    IntHashTable *hash_table = intHashTable_new(TABLE_SIZE);

    int efort = -1;
    Entry entry1 = {1, "George"};
    Entry entry2 = {2, "Costel"};
    Entry entry3 = {3, "Mara"};
    Entry entry4 = {4, "Stefi"};
    Entry entry5 = {5, "Gigel"};

    hash_insert(hash_table, entry1);
    hash_insert(hash_table, entry2);
    hash_insert(hash_table, entry3);
    hash_insert(hash_table, entry4);
    hash_insert(hash_table, entry5);

    hash_search(hash_table, 3, &efort);
    hash_search(hash_table, 2, &efort);
    hash_search(hash_table, 5, &efort);

    hash_delete(hash_table, 3, &efort);
    hash_search(hash_table, 3, &efort);

    intHashTable_free(hash_table);
}

void efort() {
    srand(time(NULL));
    // float alfa[5] = {0.8, 0.85, 0.9, 0.95, 0.99};
    float alfa = 0.95;
    float efort_mediu_gasite = 0;
    float efort_maxim_gasite = 0;
    float efort_mediu_negasite = 0;
    float efort_maxim_negasite = 0;
    float nr_gasite = 0;
    float nr_negasite = 0;
    for (int i = 0; i < 5; i++) {
        int n = (int) return_n(alfa);
        IntHashTable *T = intHashTable_new(N);

        int *array = (int *) malloc(n * sizeof(int));
        FillRandomArray(array, n, 10, 25000, true, 0);

        Entry *entry = NULL;
        for (int i = 0; i < n; i++) {
            entry = new_entry(array[i], T->table[i].name);
            hash_insert(T, *entry);
        }
        //ca sa ne asiguram ca jumatate din elem sunt gasite trebuie sa le luam in mod aleator din vectortul array unde am inserat valori initial
        int m = 3000;
        int *array_search = (int *) malloc(m * sizeof(int));
        for (int i = 0; i < m / 2; i++) {
            int rand_poz = rand() % n;  //ne asiguram ca pozitia nu iese din interval
            array_search[i] = array[rand_poz];
        }
        //cealalta jumatate va avea valori random si unice care sigur nu se gasesc in array pt ca avem range uri mai mari
        FillRandomArray(array_search + m / 2, m / 2, 25001, 50000, true, 0);

        int found_key;
        int efort = 0;
        for (int i = 0; i < m; i++) {
            found_key = hash_search(T, array_search[i], &efort);
            if (found_key != -1) {
                efort_mediu_gasite += (float) efort;
                nr_gasite++;
                if ((float) efort > efort_maxim_gasite) {
                    efort_maxim_gasite = (float) efort;
                }

            } else {
                efort_mediu_negasite += (float) efort;
                nr_negasite++;
                if ((float) efort > efort_maxim_negasite) {
                    efort_maxim_negasite = (float) efort;
                }
            }
        }

        efort_mediu_gasite = efort_maxim_gasite / nr_gasite;
        efort_mediu_negasite = efort_maxim_negasite / nr_negasite;

    }


    printf("Factor de umplere\tEfort mediu gasite\tEfort maxim gasite\tEfort mediu negasite\tEfort maxim negasite\n");

    printf("%.2f\t\t\t%.2f\t\t\t%.2f\t\t\t%.2f\t\t\t%.2f\n", alfa,
           efort_mediu_gasite, efort_maxim_gasite,
           efort_mediu_negasite, efort_maxim_negasite);

}

int main() {
    //demo();
    efort();
    return 0;
}
