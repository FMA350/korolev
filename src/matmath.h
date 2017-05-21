#ifndef MATMATH
#define MATMATH


#include "various.h"

void summ(struct Coordinates* a, struct Coordinates* b);
void subm(struct Coordinates* a, struct Coordinates* b);
void molm(struct Coordinates* a, struct Coordinates* b);
void divm(struct Coordinates* a, struct Coordinates* b);

void sums(struct Coordinates* a, double b);
void subs(struct Coordinates* a, double b);
void mols(struct Coordinates* a, double b);
void divs(struct Coordinates* a, double b);
void assign(struct Coordinates* a, struct Coordinates* b);
#endif
