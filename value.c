#include <stdio.h>
#include <stdlib.h>
#include "value.h"

char *INTEGER = "INTEGER";
char *REAL = "REAL";
char *STRING = "STRING";
char *VARIABLE = "VARIABLE";
char *OPERATOR = "OPERATOR";
char *SEMICOLON = "SEMICOLON";
char *PARENTHESIS = "PARENTHESIS";
char *VAR = "VAR";
char *EQUALS = "EQUALS";

static value *newValue(char *);

/*****
 * Lusth's Value Class modified for use in Assignment 1 by Chipper Atkins
 */

/*************** public interface *************/

value *newIntegerValue(int v) {
    value *n = newValue(INTEGER);
    n->ival = v;
    return n;
}

value *newRealValue(double v) {
    value *n = newValue(REAL);
    n->rval = v;
    return n;
}

value *newStringValue(char *v) {
    value *n = newValue(STRING);
    n->sval = v;
    return n;
}

value *newVariableValue(char *v) {
    value *n = newValue(VARIABLE);
    n->sval = v;
    return n;
}

value *newOperatorValue(char *v) {
    value *n = newValue(OPERATOR);
    n->sval = v;
    return n;
}

value *newParenthesisValue(char *v) {
    value *n = newValue(PARENTHESIS);
    n->sval = v;
    return n;
}

value *newVarValue() {
    value *n = newValue(VAR);
    return n;
}

value *newEqualsValue(char *v) {
    value *n = newValue(EQUALS);
    n->sval = v;
    return n;
}

value *newSemicolonValue() {
    value *n = newValue(SEMICOLON);
    return n;
}

void displayValue(FILE *fp,value *v) {
    if (v->type == STRING)
       fprintf(fp,"\"%s\"",v->sval);
    else if (v->type == INTEGER)
       fprintf(fp,"%d",v->ival);
    else if (v->type == REAL)
       fprintf(fp,"%f",v->rval);
    else if (v->type == VARIABLE)
       fprintf(fp,"%s",v->sval);
    else if (v->type == OPERATOR)
       fprintf(fp,"%s",v->sval);
    else if (v->type == PARENTHESIS)
        fprintf(fp,"%s",v->sval);
    else
       fprintf(fp,"<UNKNOWN VALUE TYPE>");
}

/*************** private methods *************/

value *newValue(char *t) { //allocates space for value
    value *n = malloc(sizeof(value));
    if (n == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    n->type = t;
    return n;
}
