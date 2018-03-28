/*
* Abstract Data Type - Binary Search Trees
* Laurence Webb 2017 <lw17773@my.bristol.ac.uk>
*/

#include "bst.h"

#define BRACKETS 3

int node_count(bstnode* n);
bstnode* create_node(bst* b, void* v);
bool node_isin(bst* b, bstnode* n, void* v);
void node_insert(bst* b, bstnode* n, void* v);
int tree_depth(bstnode* n);
void node_free(bstnode* n);
void nodes_to_array(bst* b, bstnode* n, char** index);
char* node_print(bst*b, bstnode* n);
void node_rebalance(bst* rb, char* sortedlist, int start, int end);

bst* bst_init(int sz,
              int(*comp)(const void* a, const void* b),
              char*(*prnt)(const void* a) )
{
    bst *tree;
    tree = (bst*)calloc(1,sizeof(bst));
    if (tree == NULL) {
    ON_ERROR("Creation of tree failed!\n");
    }
    tree->top = NULL;
    tree->elsz = sz;
    tree->compare = comp;
    tree->prntnode = prnt;

    return tree;
}

void bst_insert(bst* b, void* v)
{
    if (b == NULL || v == NULL) {
        ON_ERROR("Cannot pass null values to bst_insert!\n");
    }
    node_insert(b, b->top, v);
    return;
}

int bst_size(bst* b)
{
    if (b == NULL || b->top == NULL) {
        return 0;
    }
    return node_count(b->top);
}

int bst_maxdepth(bst* b)
{
    if (b == NULL) {
        return 0;
    }
    return tree_depth(b->top);
}

bool bst_isin(bst* b, void* v)
{
    if (b == NULL || v == NULL) {
        ON_ERROR("Cannot check null values!\n");
    }
    return (node_isin(b, b->top, v));
}

void bst_insertarray(bst* b, void* v, int n)
{
    int i;
    if (b == NULL) {
        ON_ERROR("Cannot insert into uninitialised tree!\n");
    }
    if (v == NULL || n == 0) {
        ON_ERROR("Cannot insert using NULL/0 array values!\n");
    }
    for (i = 0; i < n; i++) {
        bst_insert(b, v);
        v = (char*)v + b->elsz;
    }
    return;
}

void bst_free(bst** p)
{
    bst* temp;
    if (p == NULL) {
        return;
    }
    temp = *p;
    node_free(temp->top);
    free(temp);
    *p = NULL;
    return;
}

char* bst_print(bst* b)
{
    if (b == NULL) {
        ON_ERROR("Canot print NULL tree!\n");
    }
    return node_print(b, b->top);
}

void bst_getordered(bst* b, void* v)
{
    char *index;
    index = (char*)v;
    if (b == NULL || v == NULL) {
        printf("Cannot order null tree/data!\n");
    }
    nodes_to_array(b, b->top, &index);
    return;
}

bst* bst_rebalance(bst* b)
{
    bst* rb;
    char* list;
    int start, end;
    if (b == NULL) {
        ON_ERROR("Cannot rebalance NULL tree!\n");
    }
    rb = bst_init(b->elsz, b->compare, b->prntnode);
    list = (char*)calloc(bst_size(b), b->elsz);
    if (list == NULL) {
        ON_ERROR("Creation of list failed!\n");
    }
    start = 0;
    /*End of array*/
    end = bst_size(b) - 1;
    bst_getordered(b, list);
    node_rebalance(rb, list, start, end);
    free(list);
    return rb;
}

/*Node functions*/

bstnode* create_node(bst* b, void* v)
{
    bstnode *node;
    node = (bstnode*)calloc(1, sizeof(bstnode));
    if (node == NULL) {
        ON_ERROR("Creation of node failed!\n");
    }
    node->data = (void*)calloc(1, b->elsz);
    if (node->data == NULL) {
        ON_ERROR("Creation of data pointer failed!\n");
    }
    memcpy(node->data, v, b->elsz);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void node_insert(bst* b, bstnode* n, void* v)
{
    bstnode* temp;
    if (n == NULL) {
        b->top = create_node(b, v);
        return;
    }
    if (b->compare(v, n->data) == 0) {
        return;
    }
    if (b->compare(v, n->data) <= 0) {
        if (n->left == NULL) {
            temp = create_node(b, v);
            n->left = temp;
            return;
        }
        else {
            node_insert(b, n->left, v);
        }
    }
    if (b->compare(v, n->data) > 0) {
        if (n->right == NULL) {
            temp = create_node(b, v);
            n->right = temp;
            return;
        }
        else {
            node_insert(b, n->right, v);
        }
    }
}

bool node_isin(bst* b, bstnode* n, void* v)
{
    if (n == NULL) {
        return false;
    }
    if (b->compare(v, n->data) == 0) {
        return true;
    }
    if (b->compare(v, n->data) < 0) {
        return node_isin(b, n->left, v);
    }
    if (b->compare(v, n->data) > 0) {
        return node_isin(b, n->right, v);
    }
    return false;
}

int node_count(bstnode* n)
{
    if (n == NULL) {
        return 0;
    }
    return node_count(n->left) + node_count(n->right) + 1;
}

int tree_depth(bstnode* n)
{
    int ldepth, rdepth;
    if (n == NULL) {
        return 0;
    }
    ldepth = tree_depth(n->left);
    rdepth = tree_depth(n->right);

    if (ldepth > rdepth) {
        return ldepth + 1;
    }
    return rdepth + 1;
}

char* node_print(bst* b, bstnode* n)
{
    int t, l, r;
    char* tree, *left, *right;
    if (n == NULL) {
        tree = (char*)calloc(1 , 1);
    if (tree == NULL) {
        ON_ERROR("Creation of blank tree string failed!\n");
    }
    return tree;
    }

    left = node_print(b, n->left);
    right = node_print(b, n->right);

    t = strlen(b->prntnode(n->data));
    l = strlen(left);
    r = strlen(right);

    tree = (char*)malloc(t + l + r + BRACKETS);
    if (tree == NULL) {
        ON_ERROR("Creation of tree string failed!\n");
    }

    sprintf(tree, "(%s%s%s)", b->prntnode(n->data), left, right);
    printf("%s\n", tree);

    free(left);
    free(right);
    return tree;
}

void nodes_to_array(bst* b, bstnode* n, char** index)
{
    if (n == NULL) {
        return;
    }
    nodes_to_array(b, n->left, index);
    memcpy(*index, n->data, b->elsz);
    *index += b->elsz;
    nodes_to_array(b, n->right, index);
}

void node_rebalance(bst* rb, char* sortedlist, int start, int end)
{
    int mid;
    char *midlist;
    if (start > end) {
        return;
    }
    midlist = sortedlist;
    mid = (start + end)/2;
    midlist += (mid*rb->elsz);
    bst_insert(rb, midlist);

    node_rebalance(rb, sortedlist, start, mid-1);
    node_rebalance(rb, sortedlist, mid+1, end);
    return;
}

void node_free(bstnode* n)
{
    if (n == NULL) {
        return;
    }
    node_free(n->left);
    node_free(n->right);

    free(n->data);
    free(n);
    return;
}
