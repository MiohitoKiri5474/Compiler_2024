#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

typedef struct node {
    struct node *l, *r;
    int pri, val, ma, sz;
} node;

node *new_node(int _val)
{
    node *res = malloc(sizeof(node));
    res->pri = rand();
    res->val = res->ma = _val;
    res->l = res->r = NULL;
    return res;
}

int Sz(node *o)
{
    return o ? o->sz : 0;
}

void up(node **_o)
{
    node *o = *_o;
    o->ma = o->val;
    o->sz = 1;
    if (o->l) {
        o->ma = max(o->ma, o->l->ma);
        o->sz += o->l->sz;
    }
    if (o->r) {
        o->ma = max(o->ma, o->r->ma);
        o->sz += o->r->sz;
    }
}

node *merge(node *a, node *b)
{
    if (!a || !b)
        return a ? a : b;
    if (a->pri < b->pri) {
        a->r = merge(a->r, b);
        up(&a);
        return a;
    } else {
        b->l = merge(a, b->l);
        up(&b);
        return b;
    }
}

void print(node *o)
{
    if (o->l)
        print(o->l);
    printf("%d ", o->val);
    if (o->r)
        print(o->r);
}

int main()
{
    srand(clock());
    node *root = NULL;
    int n;
    scanf("%d", &n);
    for (int i = 0, in; i < n; i++) {
        scanf("%d", &in);
        printf("\t%d\n", in);
        root = merge(root, new_node(in));
    }
    print(root);
    puts("");
}
