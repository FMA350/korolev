#include <stdio.h>

struct str{
    int value;
    struct str *next;
};

void function(struct str a){
    a.value = 7;
    return;
}

int main(){
    struct str a, b;
    struct str * strp;
    // int a = 0;
    // int *p;
    // int **pp;
    strp = &a;
    strp->value = 5;
    strp->next = &b;
    strp->next->value = 8;
    function(a);

    // pp = &p;
    // a  += 1;
    printf("%d\n", strp->value);
}
