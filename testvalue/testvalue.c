/*
 * tester for the value class
 *
 *    written by John C. Lusth
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "scanner.h"

void Fatal(char *,...);

static value *readValue(FILE *);
static void printValue(FILE *,char *,value *);
static void freeValue(value *);

int
main(int argc,char **argv)
    {
    value *v1,*v2,*v3;

    // read in three values from stdin

    printf("enter three values: ");

    v1 = readValue(stdin);
    v2 = readValue(stdin);
    v3 = readValue(stdin);

    printValue(stdout,"first",v1);
    printValue(stdout,"second",v2);
    printValue(stdout,"third",v3);

    freeValue(v1);
    freeValue(v2);
    freeValue(v3);

    return 0;
    }

static value *
readValue(FILE *fp)
    {
    value *v;

    if (stringPending(fp))
        v = newStringValue(readString(stdin));
    else
        {
        char *token = readToken(fp);
        if (strchr(token,'.') != 0) // dot found!
            v = newRealValue(atof(token));
        else if (*token == '-' || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else
            Fatal("The token %s is not a value\n",token);
        }
    return v;
    }

static void
printValue(FILE *fp,char *order,value *v)
    {
    if (v->type == INTEGER)
        fprintf(fp,"The %s value is an integer: %d\n",order,v->ival);
    else if (v->type == REAL)
        fprintf(fp,"The %s value is a real: %f\n",order,v->rval);
    else //must be a string
        fprintf(fp,"The %s value is a string: %s\n",order,v->sval);
    }

static void
freeValue(value *v)
    {
    if (v->type == STRING) free(v->sval);
    free(v);
    }

void
Fatal(char *fmt, ...)
    {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
    }
