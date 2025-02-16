#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("Statistici");

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int size;
}node;

typedef struct IntBinarySearchTree {
    node* root;
}IntBinarySearchTree;

IntBinarySearchTree* intBinarySearchTree_new()
{
    IntBinarySearchTree* tree = (IntBinarySearchTree*)malloc(sizeof(IntBinarySearchTree));
    tree->root = NULL;
    return tree;
}

node* init_node(int value) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data = value;
    new_node->size = 1;
    return new_node;
}

//static unsigned IntBinarySearchTree_size_helper(node *node1) {
//    if (node1 == NULL)
//    {
//        return 0;
//    }
//    return (1 + IntBinarySearchTree_size_helper(node1->left) + IntBinarySearchTree_size_helper(node1->right));
//}

void IntBinarySearchTree_size(node* current_node) {
    if (current_node != NULL) {
        IntBinarySearchTree_size(current_node->left);
        IntBinarySearchTree_size(current_node->right);

        current_node->size = 1;

        if (current_node->left != NULL) {
            current_node->size += current_node->left->size;
        }

        if (current_node->right != NULL) {
            current_node->size += current_node->right->size;
        }
    }
}

node* create_BST(int* a, int start, int end) {
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;

    node* new_node = init_node(a[mid]);
    new_node->size = end - start + 1;
    new_node->left = create_BST(a, start, mid - 1);
    new_node->right = create_BST(a, mid + 1, end);

    IntBinarySearchTree_size(new_node);

    return new_node;

}

node* OS_SELECT(node* x, int i) {
    int r = 1;
    if (x->left != NULL) {
        r += x->left->size;
    }
    if (i == r) {
        return x;
    }
    else if (i < r) {
        return OS_SELECT(x->left, i);
    }
    else {
        return OS_SELECT(x->right, i - r);
    }
}

void pretty_print_helper(node* current_node, int nivel) {
    if (current_node != NULL) {
        int tab = 0;
        while (tab < nivel) {
            printf("\t");
            tab++;
        }
        printf("%d (%d)\n", current_node->data, current_node->size);
        pretty_print_helper(current_node->left, nivel + 1);
        pretty_print_helper(current_node->right, nivel + 1);
    }
}

void pretty_print(IntBinarySearchTree* tree) {
    if (tree != NULL && tree->root != NULL) {
        pretty_print_helper(tree->root, 0);
    }
}


node* delete_node(node** pNode) {
    if (*pNode == NULL) {           //daca nodul curent e null nu avem ce sterge
        return NULL;
    }
    node* deletedNode = NULL;

    if ((*pNode)->left == NULL && (*pNode)->right == NULL) {
        deletedNode = *pNode;                                   //daca nodul este frunza
        *pNode = NULL;
    }
    else if ((*pNode)->left == NULL) {       //daca nodul nu are left child, il inlocuim cu right child
        deletedNode = *pNode;
        *pNode = (*pNode)->right;
    }
    else if ((*pNode)->right == NULL) {    //daca nodul nu are right child, il inlocuim cu left child
        deletedNode = *pNode;
        *pNode = (*pNode)->left;
    }
    else {
        node** pNodeMin = &((*pNode)->right);   //daca nodul are ambii copii, caut SUCCESORUL, adica minimul din subarborele drept
        while ((*pNodeMin)->left != NULL) {
            pNodeMin = &((*pNodeMin)->left);
        }
        (*pNode)->data = (*pNodeMin)->data;
        deletedNode = delete_node(pNodeMin);    //inlocuiesc nodul cu acest minim si sterg recursiv nodul
    }
    return deletedNode;
}


void search_and_delete_node(node** pNode, int value) {              //functia cauta in arbore nosul de sters
    int hasChildren = 0;
    if ((*pNode)->left || (*pNode)->right) {
        hasChildren = 1;
    }
    if (*pNode == NULL) {
        return;
    }
    if (value < (*pNode)->data) {                                   //daca e mai mic decat nodul curent, merge in subarborele stang
        search_and_delete_node(&((*pNode)->left), value);
    }
    else if (value > (*pNode)->data) {
        search_and_delete_node(&((*pNode)->right), value);    //daca e mai mare decat nodul curent, merge in subarborele drept
    }
    else {
        node* deletedNode = delete_node(pNode);
        if (*pNode == NULL && hasChildren) {
            (*pNode) = deletedNode;
        }
        //daca root ul este sters, referinta root ului este actualizata
        //cu referinta catre nodul care inlocuieste root ul dupa stergere
    }
    //actualizat size
    IntBinarySearchTree_size(*pNode);
}

void OS_delete(IntBinarySearchTree* tree, int value)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }
    search_and_delete_node(&(tree->root), value);
}

void demo() {
    srand(time(NULL));
    int n = 11;
    int* array = (int*)malloc(n * sizeof(int));
    IntBinarySearchTree* tree = intBinarySearchTree_new();

    for (int i = 0; i < n; i++) {
        array[i] = i + 1;
    }

    tree->root = create_BST(array, 0, n - 1);
    printf("BST initial:\n");
    pretty_print(tree);

    int k = 5;
    node* result = OS_SELECT(tree->root, k);
    printf("al %d-lea cel mai mic elem este: %d\n\n", k, result->data);

    for (int i = 0; i < 3; i++) {
        int delete_key = (rand() % 11) + 1;
        printf("dupa stergerea nodului %d:\n", delete_key);
        OS_delete(tree, delete_key);
        pretty_print(tree);
    }
}

int main() {
    demo();
    return 0;
}

