#include "value.h"
#include <stdio.h>
#include <stdlib.h>

static value *newValue(int);

/**** Public Interface ****/

int INTEGER = 0;
int REAL = 1;
int STRING = 2;
int VARIABLE = 3;

value *newValueI(int i)
{
    value *v = newValue(INTEGER);
    v->ival = i;
    return v;
}

value *newValueR(double d)
{
    value *v = newValue(REAL);
    v->rval = d;
    return v;
}

value *newValueS(char *c)
{
    value *v = newValue(STRING);
    v->sval = c;
    return v;
}

value *newValueV(char *c)
{
    value *v = newValue(VARIABLE);
    v->sval = c;
    return v;
}

/**** Private Interface ****/

static value *newValue(int t)
{
    value *v;
    if((v = malloc(sizeof(value))) == 0)
        printf("%s\n","out of memory");
    v->type = t;
    v->ival = 0;
    v->rval = 0;
    v->sval = 0;
    return v;
}