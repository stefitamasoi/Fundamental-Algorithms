#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    struct node *child[];
} node;

typedef struct {
    node *root;
} Tree;


Tree *new_tree() {
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    tree->root = NULL;
    return tree;
}

static unsigned size_helper(node *node1) {
    if (node1 == NULL) {
        return 0;
    }
    return 1 + size_helper(node1->child[0]);
}

unsigned size(Tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    return size_helper(tree->root);
}

node *add_node(int value, int n) {
    node *new_node = (node *) malloc(n * sizeof(node *));
    new_node->key = value;
    for (int i = 0; i < n; i++) {
        new_node->child[i] = NULL;
    }
    return new_node;
}


void pretty_print_helper(int *a, int n, int index, int nivel) {
    for (int i = 0; i < n; i++) {
        if (a[i] == index) {
            int tab = 0;
            while (tab < nivel) {
                printf("\t");
                tab++;
            }
            printf("%d\n", i + 1);
            pretty_print_helper(a, n, i + 1, nivel + 1);
        }
    }
}

void pretty_print(int *a, int n) {
    pretty_print_helper(a, n, -1, 0);
}

void demo_pretty_print() {
    int a[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    int n = sizeof(a) / sizeof(a[0]);
    pretty_print(a, n);
}


int main() {
    demo_pretty_print();
    //r1_to_r2();

    return 0;
}
