#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "bfs.h"
#include "Profiler.h"


int check_valid_position(const Grid *grid, Point p) {
    return (p.row >= 0 && p.row < grid->rows && p.col >= 0 && p.col < grid->cols);
}

int get_neighbors(const Grid *grid, Point p, Point neighb[]) {
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    int count = 0;

    if (check_valid_position(grid, p)) {
        //verif vecin sus
        if (grid->mat[p.row - 1][p.col] == 0) {
            neighb[count].row = p.row - 1;
            neighb[count].col = p.col;
            count++;
        }

        //verif vecin jos
        if (grid->mat[p.row + 1][p.col] == 0) {
            neighb[count].row = p.row + 1;
            neighb[count].col = p.col;
            count++;
        }

        //verif vecin stanga
        if (grid->mat[p.row][p.col - 1] == 0) {
            neighb[count].row = p.row;
            neighb[count].col = p.col - 1;
            count++;
        }

        //verif vecin dreapta
        if (grid->mat[p.row][p.col + 1] == 0) {
            neighb[count].row = p.row;
            neighb[count].col = p.col + 1;
            count++;
        }
        return count;
    } else {
        return 0;
    }
}

void grid_to_graph(const Grid *grid, Graph *graph) {
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node *) malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            } else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node **) malloc(graph->nrNodes * sizeof(Node *));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node **) malloc(graph->v[i]->adjSize * sizeof(Node *));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node **) realloc(graph->v[i]->adj, k * sizeof(Node *));
            }
        }
    }
}

void free_graph(Graph *graph) {
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op) {
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    if (op != NULL) {
        op->count();
    }
    for (int i = 0; i < graph->nrNodes; i++) {
        Node *u = graph->v[i];
        u->color = COLOR_WHITE;
        u->dist = INT_MAX;
        u->parent = NULL;
    }
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;

    Node **queue = (Node **) malloc(graph->nrNodes * sizeof(Node *));
    int front = 0;
    int back = 0;

    queue[back++] = s;
    while (front < back) {
        Node *u = queue[front++];
        int adj_size = u->adjSize;
        for (int i = 0; i < adj_size; ++i) {
            Node *v = u->adj[i];

            if (v->color == COLOR_WHITE) {
                v->color = COLOR_GRAY;
                v->dist = u->dist + 1;
                v->parent = u;
                queue[back++] = v;
            }
        }
        u->color = COLOR_BLACK;
    }
    free(queue);
}

void pretty_print_helper(int *p, Point *repr, int n, int index, int nivel) {
    for (int i = 0; i < n; i++) {
        if (p[i] == index) {
            int tab = 0;
            while (tab < nivel) {
                printf("\t");
                tab++;
            }
            printf("(%d, %d)\n", repr[i].row, repr[i].col);
            pretty_print_helper(p, repr, n, i, nivel + 1);
        }
    }
}


void print_bfs_tree(Graph *graph) {
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int *) malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        } else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int *) malloc(n * sizeof(int));
    repr = (Point *) malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            } else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                } else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        pretty_print_helper(p, repr, n, -1, 0);

    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]) {
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    // Implement the BFS algorithm to find the shortest path

    bfs(graph, start);

    if (end->dist == INT_MAX) {
        printf("not reachable!!!");
        return -1;
    }

    int path_length = 0;
    Node *current = end;


    while (current != NULL) {                         //refac path ul de la sfarsit spre inceput
        path[path_length++] = current;
        current = current->parent;
    }

    for (int i = 0; i < path_length / 2; i++) {     //inversez ca sa am ordinea buna a path ului
        int j = path_length - 1 - i;
        Node *aux = path[i];
        path[i] = path[j];
        path[j] = aux;
    }

    return path_length;
}


int adiacent(Node *u, Node *v) {
    for (int i = 0; i < u->adjSize; i++) {
        if (u->adj[i] == v) {
            return 1;
        }
    }
    return 0;
}


void performance() {
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected


        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
