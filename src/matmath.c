#include "matmath.h"
#include "various.h"


void summ(struct Coordinates* a, struct Coordinates* b){
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}
void subm(struct Coordinates* a, struct Coordinates* b){
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}
void molm(struct Coordinates* a, struct Coordinates* b){
    a->x *= b->x;
    a->y *= b->y;
    a->z *= b->z;
}
void divm(struct Coordinates* a, struct Coordinates* b){
    a->x /= b->x;
    a->y /= b->y;
    a->z /= b->z;
}

void sums(struct Coordinates* a, double b){
    a->x += b;
    a->y += b;
    a->z += b;
}
void subs(struct Coordinates* a, double b){
    a->x -= b;
    a->y -= b;
    a->z -= b;
}
void mols(struct Coordinates* a, double b){
    a->x *= b;
    a->y *= b;
    a->z *= b;
}
void divs(struct Coordinates* a, double b){
    a->x /= b;
    a->y /= b;
    a->z /= b;
}

void assign(struct Coordinates* a, struct Coordinates* b){
    a->x = b->x;
    a->y = b->y;
    a->z = b->z;
}
