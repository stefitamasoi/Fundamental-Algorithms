#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    int rank;
    struct node *parent;
} node;

typedef struct {
    node *root;
} Tree;

void make_set(Tree *tree, int x) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->data = x;
    new_node->rank = 0;
    new_node->parent = new_node;
    tree->root = new_node;
}

node* find_set(node *x) {
    if (x != x->parent) {
        x->parent = find_set(x->parent);
    }
    return x->parent;
}

void link(node *x, node *y) {
    if (x->rank > y->rank) {
        y->parent = x;
    } else {
        x->parent = y;
        if (x->rank == y->rank) {
            y->rank = y->rank + 1;
        }
    }
}

void union_sets(node *x, node *y) {
    node *root_x = find_set(x);
    node *root_y = find_set(y);
    link(root_x, root_y);
}

int partition(int muchie[][3], int l, int r) {
    int pivot = muchie[r][2];
    int i = l - 1;
    for (int j = l; j < r; j++) {
        if (muchie[j][2] <= pivot) {
            i++;
            for (int k = 0; k < 3; k++) {
                int aux = muchie[i][k];
                muchie[i][k] = muchie[j][k];
                muchie[j][k] = aux;
            }
        }
    }
    for (int k = 0; k < 3; k++) {
        int aux = muchie[i + 1][k];
        muchie[i + 1][k] = muchie[r][k];
        muchie[r][k] = aux;
    }
    return i + 1;
}

void quicksort(int muchie[][3], int l, int r) {
    if (l < r) {
        int q = partition(muchie, l, r);
        quicksort(muchie, l, q - 1);
        quicksort(muchie, q + 1, r);
    }
}

void kruskal(Tree *sets, int nr_noduri, int nr_muchii, int muchie[][3]) {
    for (int i = 0; i < nr_noduri; i++) {
        make_set(&sets[i], i);
    }
    quicksort(muchie, 0, nr_muchii - 1);
    printf("\n");
    for (int i = 0; i < nr_muchii; i++) {
        int u = muchie[i][0];
        int v = muchie[i][1];

        if (find_set(sets[u].root) != find_set(sets[v].root)) {
            printf("(%d, %d)\n", u, v);
            union_sets(sets[u].root, sets[v].root);
        }
    }
}

void demo_op(){
    Tree sets[10];
    for (int i = 0; i < 10; i++) {
        make_set(&sets[i], i + 1);
    }

    printf("initial:\n");
    for (int i = 0; i < 10; i++) {
        node *root = find_set(sets[i].root);
        printf("%d: %d\n", i + 1, root->data);
    }

    union_sets(sets[0].root, sets[1].root);
    union_sets(sets[2].root, sets[3].root);
    union_sets(sets[4].root, sets[5].root);
    union_sets(sets[6].root, sets[7].root);
    union_sets(sets[8].root, sets[9].root);



    printf("\nunion: \n");
    for (int i = 0; i < 10; i++) {
        node *root = find_set(sets[i].root);
        printf("%d: %d\n", i + 1, root->data);
    }

}

void demo_kruskal(){
    int nr_noduri = 10;
    int nr_muchii = 14;

    int muchie[][3] = {
            {0, 1, 4}, {0, 7, 8}, {1, 2, 8}, {1, 7, 15},
            {2, 3, 7}, {2, 5, 4}, {2, 8, 2}, {3, 4, 9},
            {3, 5, 15}, {4, 5, 9}, {5, 6, 2}, {6, 7, 1},
            {6, 8, 6}, {7, 8, 7}
    };

    Tree sets[nr_noduri];
    kruskal(sets, nr_noduri, nr_muchii, muchie);

}
int main() {
    demo_op();
    demo_kruskal();
    return 0;
}
