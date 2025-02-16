#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100

Profiler p("Parcurgere");

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
    struct node *parent;
} node;

typedef struct {
    node *root;
} BinaryTree;

BinaryTree *BinaryTree_new() {
    BinaryTree *tree = (BinaryTree *) malloc(sizeof(BinaryTree));
    tree->root = NULL;
    return tree;
}

node *init_node(int value) {
    node *new_node = (node *) malloc(sizeof(node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->data = value;
    return new_node;
}

node *create_BST(int *a, int start, int end) {
    if (start > end) {
        return NULL;
    }
    int mid = (start + end) / 2;
    node *new_node = init_node(a[mid]);
    new_node->left = create_BST(a, start, mid - 1);
    if (new_node->left != NULL) {
        new_node->left->parent = new_node;
    }
    new_node->right = create_BST(a, mid + 1, end);
    if (new_node->right != NULL) {
        new_node->right->parent = new_node;
    }
    return new_node;
}

node *add_helper(node *current_node, int value) {
    if (current_node == NULL) {
        return init_node(value);
    }
    if (value < current_node->data) {
        node *left_child = add_helper(current_node->left, value);
        current_node->left = left_child;
        left_child->parent = current_node;
    } else if (value > current_node->data) {
        node *right_child = add_helper(current_node->right, value);
        current_node->right = right_child;
        right_child->parent = current_node;
    }
    return current_node;
}


void BinaryTree_add(BinaryTree *tree, int value) {
    if (tree == NULL) {
        return;
    }
    if (tree->root == NULL) {
        tree->root = init_node(value);
    } else {
        add_helper(tree->root, value);
    }
}

void iterative(BinaryTree *bst, Operation op, int isDemo) {
    if (bst->root == NULL) {
        return;
    }
    int d = 1;
    node *tree_node = bst->root;
    do {
        if (d == 1) {
            if (tree_node->left != NULL) {
                tree_node = tree_node->left;
            } else {
                d = 2;
            }
        }
        if (d == 2) {
            if (isDemo) {
                printf("%d ", tree_node->data);
            } else {
                op.count();
            }
            if (tree_node->right != NULL) {
                tree_node = tree_node->right;
                d = 1;
            } else {
                d = 3;
            }
        }
        if (d == 3) {
            if (tree_node->parent != NULL) {
                if (tree_node == tree_node->parent->left) {
                    d = 2;
                }
                tree_node = tree_node->parent;
            }
        }
    } while (!(tree_node == bst->root && d == 3));
}

void recursive_print_inorder_helper(node *current_node, Operation op, int isDemo) {
    if (current_node != NULL) {
        recursive_print_inorder_helper(current_node->left, op, isDemo);
        if (isDemo) {
            printf("%d ", current_node->data);
        } else {
            op.count();
        }
        recursive_print_inorder_helper(current_node->right, op, isDemo);
    }
}

void BinaryTree_printInorder(BinaryTree *tree, Operation op, int isDemo) {
    if (tree != NULL && tree->root != NULL) {
        recursive_print_inorder_helper(tree->root, op, isDemo);
    }
}

void demo() {
    int isDemo = 1;
    BinaryTree *bst = BinaryTree_new();
    BinaryTree_add(bst, 1);
    BinaryTree_add(bst, -10);
    BinaryTree_add(bst, 30);
    Operation op = p.createOperation("iterativ_vs_recursiv", 3);
    printf("Parcurgere iterativa: ");
    iterative(bst, op, isDemo);
    printf("\n");
    printf("Parcurgere recursiva: ");
    BinaryTree_printInorder(bst, op, isDemo);
}

void test() {
    int isDemo = 0;

    int a[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        FillRandomArray(a, n, 10, 50000, TRUE, 0);
        BinaryTree *bst = BinaryTree_new();
        Operation op1 = p.createOperation("iterative", n);
        Operation op2 = p.createOperation("recursive", n);
        for (int i = 0; i < n; i++) {
            BinaryTree_add(bst, a[i]);
        }

        iterative(bst, op1, isDemo);
        BinaryTree_printInorder(bst, op2, isDemo);
    }
    p.createGroup("total", "iterative", "recursive");
    p.showReport();
}

int partition(int *a, int l, int r, int n) {
    int x = a[r];
    int i = l - 1;
    for (int j = l; j < r; j++) {
        if (a[j] <= x) {
            i++;
            int aux = a[i];
            a[i] = a[j];
            a[j] = aux;
        }
    }
    int aux = a[i + 1];
    a[i + 1] = a[r];
    a[r] = aux;
    return i + 1;
}

void quicksort(int *a, int l, int r, int n) {
    if (l < r) {
        int q = partition(a, l, r, n);
        quicksort(a, l, q - 1, n);
        quicksort(a, q + 1, r, n);
    }
}

void insertion_sort(int *a, int n) {
    int current, prev_poz;
    for (int i = 1; i < n; i++) {
        current = a[i];
        prev_poz = i - 1;
        while (prev_poz >= 0 && current < a[prev_poz]) {
            a[prev_poz + 1] = a[prev_poz];
            prev_poz--;
        }
        a[prev_poz + 1] = current;
    }
}

void demo_hibridizare() {
    int threshold = 30;
    int n;
    scanf("%d", &n);
    int a[n];
    int l = 0;
    int r = n - 1;
    for (int i = 0; i < n; i++) {
        a[i] = 30 - i;
    }
    if (n < threshold) {
        printf("Sortare efectuata cu insertion sort: ");
        insertion_sort(a, n);
    } else {
        printf("Sortare efectuata cu quicksort: ");
        quicksort(a, l, r, n);
    }
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main() {
    //demo();
    test();
    //demo_hibridizare();
    return 0;
}
