#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct node {
    struct node *adj_list;
    int length;
    int color;
    int d;
    int f;
    struct node *pi;
    int key;
} node;

typedef struct sorted_node {
    struct sorted_node *next;
    int key;
} sorted_node;

typedef struct stack_node {
    struct stack_node *next;
    node *data;
} stack_node;

void push(stack_node **top, node *data) {
    stack_node *new_node = (stack_node *)malloc(sizeof(stack_node));
    new_node->data = data;
    new_node->next = *top;
    *top = new_node;
}

node* pop(stack_node **top) {
    if (*top == NULL) {
        return NULL;
    }
    stack_node *temp = *top;
    *top = (*top)->next;
    node *data = temp->data;
    free(temp);
    return data;
}

void DFS_VISIT(node *nodes, node *current, int *time) {
    (*time)++;
    current->d = *time;
    current->color = GRAY;

    for (int i = 0; i < current->length; i++) {
        int neighbor_key = current->adj_list[i].key;
        node *neighbor = &nodes[neighbor_key];
        if (neighbor->color == GRAY) {
            // am gasit un back edge
            printf("graf ciclic, nu se poate face sortarea topologica:(\n");
            return;
        }
        if (neighbor->color == WHITE) {
            neighbor->pi = current;
            DFS_VISIT(nodes, neighbor, time);
        }
    }
    current->color = BLACK;
    (*time)++;
    current->f = *time;
}


void DFS(node *nodes, int nr_vertices) {
    for (int i = 0; i < nr_vertices; i++) {
        nodes[i].color = WHITE;
        nodes[i].pi = NULL;
    }
    int time = 0;
    for (int i = 0; i < nr_vertices; i++) {
        if (nodes[i].color == WHITE) {
            DFS_VISIT(nodes, &nodes[i], &time);
        }
    }
}

void add_edge(node *nodes, int start, int end) {
    int length = nodes[start].length;
    nodes[start].adj_list = (node *)realloc(nodes[start].adj_list, (length + 1) * sizeof(node));
    nodes[start].adj_list[length].key = end;
    nodes[start].adj_list[length].color = WHITE;
    nodes[start].adj_list[length].d = 0;
    nodes[start].adj_list[length].f = 0;
    nodes[start].adj_list[length].pi = NULL;
    nodes[start].adj_list[length].adj_list = NULL;
    nodes[start].adj_list[length].length = 0;
    nodes[start].length++;
}

void print_graph(node *nodes, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        printf("node %d (d:%d, f:%d)---->", nodes[i].key, nodes[i].d, nodes[i].f);
        for (int j = 0; j < nodes[i].length; j++) {
            printf("%d ", nodes[i].adj_list[j].key);
        }
        printf("\n");
    }
}

sorted_node* topological_sort(node *nodes, int nr_vertices) {
    DFS(nodes, nr_vertices);

    sorted_node *sorted_list = NULL;

    for (int i = nr_vertices - 1; i >= 0; i--) {
        sorted_node *new_node = (sorted_node *)malloc(sizeof(sorted_node));
        new_node->key = nodes[i].key;
        new_node->next = sorted_list;
        sorted_list = new_node;
    }

    return sorted_list;
}

void print_sorted_list(sorted_node *list) {
    printf("sorted list:\n");
    while (list != NULL) {
        printf("%d ", list->key);
        list = list->next;
    }
    printf("\n");
}

void demo(){
    int n = 6;
    node *nodes = (node *)malloc(n * sizeof(node));

    for (int i = 0; i < n; i++) {
        nodes[i].key = i;
        nodes[i].color = WHITE;
        nodes[i].d = 0;
        nodes[i].f = 0;
        nodes[i].pi = NULL;
        nodes[i].adj_list = NULL;
        nodes[i].length = 0;
    }

    add_edge(nodes, 0, 1);
    add_edge(nodes, 0, 2);
    add_edge(nodes, 1, 3);
    add_edge(nodes, 1, 5);
    add_edge(nodes, 2, 4);
    add_edge(nodes, 4, 5);

    printf("before DFS:\n");
    print_graph(nodes, n);


    DFS(nodes, n);

    printf("\nafter DFS:\n");
    print_graph(nodes, n);
    printf("\n");

    printf("before topological sort:\n");
    print_graph(nodes, n);

    sorted_node *sorted_list = topological_sort(nodes, n);
    print_sorted_list(sorted_list);

    while (sorted_list != NULL) {
        sorted_node *temp = sorted_list;
        sorted_list = sorted_list->next;
        free(temp);
    }

    free(nodes);
}

int main() {
    demo();
    return 0;
}
