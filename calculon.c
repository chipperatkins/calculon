#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
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
static queue*  convert(queue *i);

//TODO ? marks
//TODO add variables
//TODO add file input
//TODO add args
//TODO ^, math.h????

int main(int argc, char **argv)
{
    queue *i,*p;
    i = newQ();
    p = newQ();
    node *bstRoot = 0;

    FILE *fp;
    fp = stdin;
    value *v = readValue(fp);
    while (v->type == VAR)
    {
        value *k = readValue(fp); // var name
        v = readValue(fp); // =
        v = readValue(fp);  // var assignment
        bstRoot = insert(bstRoot,newBstNode(v,k->sval,0,0));
        v = readValue(fp); // semicolon
        v = readValue(fp); // next operation
    }
    while (!feof(fp))
    {
        while (v->type != SEMICOLON)
        {
            enQ(i, newValueNode(v, 0));
            v = readValue(fp);
        }
        enQ(i,newValueNode(newSemicolonValue(),0));
        v = readValue(fp);
    }

    while(i->front != 0)
    {
        queue *t = newQ();
        while (i->front->value->type != SEMICOLON)
        {
            enQ(t,deQ(i));
        }
        queue *t1;
        t1 = convert(t);
        enQQ(p,t1);
        enQ(p,deQ(i));
    }

    while (p->front != 0)
    {
        while (p->front->value->type != SEMICOLON)
        {
            printValue(stdout, 0, deQ(p)->value);
        }
        deQ(p);
    }

    stack* s = newStack();
    while(p->front!=0)
    {
        while (p->front->value->type != SEMICOLON)
        {
            if (p->front->value->type == VARIABLE)
            {
                value *temp;
                temp = search(bstRoot,p->front->value->sval)->value;
                p->front->value = temp;
            }
            if (isnum(p->front->value)==1)
                push(s,deQ(p));
            else if (p->front->value->type == EQUALS)
            {
                deQ(p);
                value *temp;
                temp = search(bstRoot,p->front->value->sval)->value;
                temp = s->top->value;
            }
            else if (p->front->value->type == OPERATOR) //must have 2 operands on stack
            {
                node* temp,*retVal;
                temp = pop(s);
                if (strcmp(p->front->value->sval,"+")==0)
                {
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival + temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval + temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->ival + temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval + temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == STRING)
                    {
                        char *temp2 = pop(s)->value->sval;
                        char *c = malloc(sizeof(char) * (strlen(temp->value->sval) + strlen(temp2) + 1));
                        if (c==0) Fatal("out of memory\n");
                        sprintf(c,"%s%s",temp2,temp->value->sval);
                        retVal = newValueNode(newStringValue(c),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER)
                    {
                        int i = atoi(temp->value->sval);
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival + i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING)
                    {
                        int i = atoi(pop(s)->value->sval);
                        retVal = newValueNode(newIntegerValue(i + temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL)
                    {
                        double i = atof(temp->value->sval);
                        retVal = newValueNode(newRealValue(pop(s)->value->rval + i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING)
                    {
                        double i = atof(pop(s)->value->sval);
                        retVal = newValueNode(newRealValue(i + temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                }
                else if (strcmp(p->front->value->sval,"-")==0)
                {
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival - temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval - temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->ival - temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval - temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER)
                    {
                        int i = atoi(temp->value->sval);
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival - i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING)
                    {
                        int i = atoi(pop(s)->value->sval);
                        retVal = newValueNode(newIntegerValue(i - temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL)
                    {
                        double i = atof(temp->value->sval);
                        retVal = newValueNode(newRealValue(pop(s)->value->rval - i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING)
                    {
                        double i = atof(pop(s)->value->sval);
                        retVal = newValueNode(newRealValue(i - temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                }
                else if (strcmp(p->front->value->sval,"*")==0)
                {
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival * temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval * temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->ival * temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval * temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER)
                    {
                        int i = atoi(temp->value->sval);
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival * i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING)
                    {
                        int i = atoi(pop(s)->value->sval);
                        retVal = newValueNode(newIntegerValue(i * temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL)
                    {
                        double i = atof(temp->value->sval);
                        retVal = newValueNode(newRealValue(pop(s)->value->rval * i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING)
                    {
                        double i = atof(pop(s)->value->sval);
                        retVal = newValueNode(newRealValue(i * temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                }
                else if (strcmp(p->front->value->sval,"/")==0)
                {
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival / temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval / temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->ival / temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pop(s)->value->rval / temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER)
                    {
                        int i = atoi(temp->value->sval);
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival * i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING)
                    {
                        int i = atoi(pop(s)->value->sval);
                        retVal = newValueNode(newIntegerValue(i * temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL)
                    {
                        double i = atof(temp->value->sval);
                        retVal = newValueNode(newRealValue(pop(s)->value->rval * i),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING)
                    {
                        double i = atof(pop(s)->value->sval);
                        retVal = newValueNode(newRealValue(i * temp->value->rval),0);
                        push(s,retVal);
                        deQ(p);
                    }
                }
                else if (strcmp(p->front->value->sval,"%")==0)
                {
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newIntegerValue(pop(s)->value->ival % temp->value->ival),0);
                        push(s,retVal);
                        deQ(p);
                    }
                }
                else if (strcmp(p->front->value->sval,"^")==0)
                {
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newRealValue(pow(pop(s)->value->ival,temp->value->ival)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pow(pop(s)->value->rval,temp->value->rval)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                    {
                        retVal = newValueNode(newRealValue(pow(pop(s)->value->ival,temp->value->rval)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                    {
                        retVal = newValueNode(newRealValue(pow(pop(s)->value->rval,temp->value->ival)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER)
                    {
                        int i = atoi(temp->value->sval);
                        retVal = newValueNode(newRealValue(pow(pop(s)->value->ival,i)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING)
                    {
                        int i = atoi(pop(s)->value->sval);
                        retVal = newValueNode(newRealValue(pow(i,temp->value->ival)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL)
                    {
                        double i = atof(temp->value->sval);
                        retVal = newValueNode(newRealValue(pow(pop(s)->value->rval,i)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING)
                    {
                        double i = atof(pop(s)->value->sval);
                        retVal = newValueNode(newRealValue(pow(i,temp->value->rval)),0);
                        push(s,retVal);
                        deQ(p);
                    }
                }
            }
        }
        deQ(p);

        if (p->front == 0)
        {
            printf("d here: %d\n",s->top->value->ival);
            printf("lf here: %lf\n",s->top->value->rval);
            printf("s here: %s\n",s->top->value->sval);
        }
    }
    return 0;
}

static queue* convert(queue *i)
{
    queue *p = newQ();
    stack *s = newStack();
    int f = 0;
    value *vmem;
    while(i->front != 0)
    {
        if (i->front->next != 0 && i->front->value->type == VARIABLE && i->front->next->value->type == EQUALS)
        {
            f = 1;
            vmem = deQ(i)->value;
            deQ(i);
        }
        else if (isnum(i->front->value)==1)
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
    if (f == 1)
    {
        enQ(p,newValueNode(newEqualsValue("="),0));
        enQ(p,newValueNode(vmem,0));
    }
    return p;
}

static value *readValue(FILE *fp)
{
    value *v;
    if (stringPending(fp))
        v = newStringValue(readString(stdin));
    else
    {
        char *token = readToken(fp);
        if (token == 0)
            v = 0;
        else if (strchr(token,'.') != 0) //dot found
            v = newRealValue(atof(token));
        else if (((*token == '-' || isdigit(*token)) && strlen(token) > 1) || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else if (strcmp(token,"var")==0)
            v = newVarValue();
        else if (token[0] > 'A' && token[0] < 'z')
            v = newVariableValue(token);
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
        else if (*token == '%')
            v = newOperatorValue("%");
        else if (*token == '^')
            v = newOperatorValue("^");
        else if (*token == '=')
            v = newEqualsValue("=");
        else if (*token == ';')
            v = newSemicolonValue();
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
    if (v->type == INTEGER || v->type == REAL || v->type == VARIABLE || v->type == STRING)
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