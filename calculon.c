#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "scanner.h"
#include "node.h"
#include "queue.h"
#include "stack.h"
#include "bst.h"

void Fatal(char *,...);

static value *readValue(FILE *);
static void printValue(FILE *,char *,value *);
static void freeValue(value *);
static int  priority(char *v1);
static int  isnum(value *v);
static int  isparenthesis(value *v);

//TODO ? marks
//TODO add variables
//TODO add file input
//TODO add args

int main(int argc, char **argv)
{
    queue *i,*p;
    i = newQ();
    p = newQ();
    stack *s;
    s = newStack();

    printf("enter an infix expression: ");
    value *v = readValue(stdin);
    while ((v->type == STRING && strcmp(v->sval,"D")!=0) || v->type !=STRING)
    {
        enQ(i, newValueNode(v, 0));
        v = readValue(stdin);
    }

    while(i->front != 0)
    {
        if (isnum(i->front->value)==1)
            enQ(p, deQ(i));

        else if (isparenthesis(i->front->value)==1)
        {
            if (strcmp(i->front->value->sval,"(")==0)
                push(s,deQ(i));
            else
            {
                while (strcmp(s->top->value->sval,"(")!=0)
                    enQ(p, pop(s));

                pop(s);
                deQ(i);
            }
        }

        else if (i->front->value->type == OPERATOR && s->top == 0)
            push(s, deQ(i));

        else
        {
            if (isparenthesis(s->top->value) == 0)
            {
                while (s->top != 0 && priority(i->front->value->sval) <= priority(s->top->value->sval))
                {
                    enQ(p, pop(s));
                }
                push(s, deQ(i));
            }
            else
                push(s, deQ(i));
        }
    }

    if (s->top != 0)
    {
        while (s->top != 0)
        {
            if (isparenthesis(s->top->value)==0)
                enQ(p, pop(s));
            else pop(s);
        }
    }
    while (p->front != 0)
    {
        printValue(stdout, 0, deQ(p)->value);
    }
    return 0;
}

static value *readValue(FILE *fp)
{
    value *v;

    if (stringPending(fp))
        v = newStringValue(readString(stdin));
    else
    {
        char *token = readToken(fp);
        if (strchr(token,'.') != 0) //dot found
            v = newRealValue(atof(token));
        else if (((*token == '-' || isdigit(*token)) && strlen(token) > 1) || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else if (*token == '+')
            v = newOperatorValue("+");
        else if (*token == '-')
            v = newOperatorValue("-");
        else if (*token == '*')
            v = newOperatorValue("*");
        else if (*token == '/')
            v = newOperatorValue("/");
        else if (*token == '(')
            v = newParenthesisValue("(");
        else if (*token == ')')
            v = newParenthesisValue(")");
        else
            Fatal("The token %s is not a value\n",token);
    }
    return v;
}

static void printValue(FILE *fp,char *order,value *v)
{
    if (v->type == INTEGER)
        fprintf(fp,"The %s value is an integer: %d\n",order,v->ival);
    else if (v->type == REAL)
        fprintf(fp,"The %s value is a real: %f\n",order,v->rval);
    else if (v->type == OPERATOR)
        fprintf(fp,"The %s value is an operator: %s\n",order,v->sval);
    else //must be a string
        fprintf(fp,"The %s value is a string: %s\n",order,v->sval);
}

static void freeValue(value *v)
{
    if (v->type == STRING) free(v->sval);
    free(v);
}

static int priority(char *v1)
{
    char * p = "=+-*/%^()";
    char * i;
    char c = v1[0];
    int index;

    i = strchr(p,c);
    index = (int)(i-p);
    return index;
}

static int isnum(value *v)
{
    if (v->type == INTEGER || v->type == REAL || v->type == VARIABLE)
        return 1;
    else
        return 0;
}

static int isparenthesis(value *v)
{
    if (v->type == PARENTHESIS)
        return 1;
    else
        return 0;
}
void Fatal(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}