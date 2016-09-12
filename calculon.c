/*****
 * Main Calculon Source File
 * Author: Chipper Atkins
 *****/

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
static void printValue(FILE *,value *);
static int  priority(char *);
static int  isnum(value *);
static int  isparenthesis(value *);
static queue* convert(queue *);
static void postfixProccess(queue *,node*);


int main(int argc, char **argv) {
    FILE *fp;

    // option handling df, vf, and ec flags and expression count respectively
    int df = 0, vf = 0,ec = 0;
    if (argc > 1) {
        for (int j = 1; j < argc; j++) {
            if (strcmp(argv[j],"-d")==0) df = 1;
            else if (strcmp(argv[j],"-v")==0)vf = 1;
        }
        if (strcmp(argv[argc-1],"-d")!=0&&strcmp(argv[argc-1],"-v")!=0)
            fp = fopen(argv[argc-1],"r");
        else fp = stdin; //stdin by default
    }
    else fp = stdin;
    if (vf == 1) {printf("Patrick V. Atkins\n");Fatal("");} //if -v

    queue *i = newQ(),*p = newQ(); //infix and postfix queues
    node *bstRoot = 0;

    value *v = readValue(fp);
    while (v->type == VAR) {
        value *k = readValue(fp); // var name
        v = readValue(fp); // =
        v = readValue(fp);  // var assignment
        bstRoot = insert(bstRoot,newBstNode(v,k->sval,0,0));
        v = readValue(fp); // semicolon
        v = readValue(fp); // next
    }

    while (!feof(fp)) {
        while (v->type != SEMICOLON) {
            if (v->type == VARIABLE && search(bstRoot,v->sval) == 0)
                Fatal("variable %s was not declared\n",v->sval);
            enQ(i, newValueNode(v, 0));
            v = readValue(fp);
        }
        enQ(i,newValueNode(newSemicolonValue(),0)); //puts semicolon value at the end
        v = readValue(fp);
    }

    while(i->front != 0) {
        queue *t = newQ();
        while (i->front->value->type != SEMICOLON)
            enQ(t,deQ(i));
        queue *t1;
        t1 = convert(t);
        enQQ(p,t1); //p stored as a queue of postfix queues delimited by ;
        enQ(p,deQ(i));
        ec++;
    }

    while (p->front != 0 && df == 1) {
        while (p->front->value->type != SEMICOLON && ec == 1)
            printValue(stdout, deQ(p)->value);
        deQ(p);
        while (ec > 1) {
            while (p->front->value->type != SEMICOLON)
                deQ(p);
            deQ(p);
            ec--;
        }
    }
    if (df == 1) {printf("\n"); Fatal("");}

    postfixProccess(p,bstRoot);
    return 0;
}

static void postfixProccess(queue *p,node* bstRoot) {
    //postfix proccessing
    stack* s = newStack();
    while(p->front!=0) { //loop through all of p
        while (p->front->value->type != SEMICOLON) { //delimited by ;
            if (isnum(p->front->value)==1) //handle legal operands
                push(s,deQ(p));
            else if (p->front->value->type == EQUALS) { // handle equals signs (complex assignment) a = EXPRESSION
                deQ(p);
                if (s->top->value->type == VARIABLE) { //handle variables by looking up in tree
                    value *temp1;
                    temp1 = search(bstRoot,s->top->value->sval)->value;
                    s->top->value = temp1;
                }
                search(bstRoot,s->top->next->value->sval)->value = s->top->value;
                pop(s);
            }
            else if (p->front->value->type == OPERATOR) { //must have 2 operands on stack, handles operators
                node* temp; //temp used to hold arg1, retVal stores result
                temp = pop(s);
                if (s->top->value->type == VARIABLE) { //handle variables by looking up in tree
                    value *temp1;
                    temp1 = search(bstRoot,s->top->value->sval)->value;
                    s->top->value = temp1;
                }
                if (temp->value->type == VARIABLE) { //handle variables by looking up in tree
                    value *temp1;
                    temp1 = search(bstRoot,temp->value->sval)->value;
                    temp->value = temp1;
                }
                if (strcmp(p->front->value->sval,"+")==0) { // addition for all operator type combinations
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival + temp->value->ival),0));
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval + temp->value->rval),0));
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                        push(s,newValueNode(newRealValue(pop(s)->value->ival + temp->value->rval),0));
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval + temp->value->ival),0));
                    else if (temp->value->type == STRING && s->top->value->type == STRING) {
                        char *temp2 = pop(s)->value->sval;
                        char *c = malloc(sizeof(char) * (strlen(temp->value->sval) + strlen(temp2) + 1));
                        if (c==0) Fatal("out of memory\n");
                        sprintf(c,"%s%s",temp2,temp->value->sval);
                        push(s,newValueNode(newStringValue(c),0));
                    }
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER) {
                        int i = atoi(temp->value->sval);
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival + i),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING) {
                        int i = atoi(pop(s)->value->sval);
                        push(s,newValueNode(newIntegerValue(i + temp->value->ival),0));
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL) {
                        double i = atof(temp->value->sval);
                        push(s,newValueNode(newRealValue(pop(s)->value->rval + i), 0));
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING) {
                        double i = atof(pop(s)->value->sval);
                        push(s,newValueNode(newRealValue(i + temp->value->rval),0));
                    }
                }
                else if (strcmp(p->front->value->sval,"-")==0) {//subtraction for all types
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival - temp->value->ival),0));
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval - temp->value->rval),0));
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                        push(s,newValueNode(newRealValue(pop(s)->value->ival - temp->value->rval),0));
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval - temp->value->ival),0));
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER) {
                        int i = atoi(temp->value->sval);
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival - i),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING) {
                        int i = atoi(pop(s)->value->sval);
                        push(s,newValueNode(newIntegerValue(i - temp->value->ival),0));
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL) {
                        double i = atof(temp->value->sval);
                        push(s,newValueNode(newRealValue(pop(s)->value->rval - i),0));
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING) {
                        double i = atof(pop(s)->value->sval);
                        push(s,newValueNode(newRealValue(i - temp->value->rval),0));
                    }
                }
                else if (strcmp(p->front->value->sval,"*")==0) {//multiplication for all types
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival * temp->value->ival),0));
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval * temp->value->rval),0));
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                        push(s,newValueNode(newRealValue(pop(s)->value->ival * temp->value->rval),0));
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval * temp->value->ival),0));
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER) {
                        int i = atoi(temp->value->sval);
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival * i),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING) {
                        int i = atoi(pop(s)->value->sval);
                        push(s,newValueNode(newIntegerValue(i * temp->value->ival),0));
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL) {
                        double i = atof(temp->value->sval);
                        push(s,newValueNode(newRealValue(pop(s)->value->rval * i),0));
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING) {
                        double i = atof(pop(s)->value->sval);
                        push(s,newValueNode(newRealValue(i * temp->value->rval),0));
                    }
                }
                else if (strcmp(p->front->value->sval,"/")==0) {//division for all types
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival / temp->value->ival),0));
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval / temp->value->rval),0));
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                        push(s,newValueNode(newRealValue(pop(s)->value->ival / temp->value->rval),0));
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pop(s)->value->rval / temp->value->ival),0));
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER) {
                        int i = atoi(temp->value->sval);
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival * i),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING) {
                        int i = atoi(pop(s)->value->sval);
                        push(s,newValueNode(newIntegerValue(i * temp->value->ival),0));
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL) {
                        double i = atof(temp->value->sval);
                        push(s,newValueNode(newRealValue(pop(s)->value->rval * i),0));
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING) {
                        double i = atof(pop(s)->value->sval);
                        push(s,newValueNode(newRealValue(i * temp->value->rval),0));
                    }
                }
                else if (strcmp(p->front->value->sval,"%")==0) { //modulous for INTs and Strings containing INTS
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival % temp->value->ival),0));
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER) {
                        int i = atoi(temp->value->sval);
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival % i),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING) {
                        int i = atoi(pop(s)->value->sval);
                        push(s,newValueNode(newIntegerValue(i % temp->value->ival),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                        push(s,newValueNode(newIntegerValue((int)pop(s)->value->rval % temp->value->ival),0));
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                        push(s,newValueNode(newIntegerValue(pop(s)->value->ival % (int)temp->value->rval),0));
                }
                else if (strcmp(p->front->value->sval,"^")==0) { //exponentiation for all types stored in REAL
                    if (temp->value->type == INTEGER && s->top->value->type == INTEGER)
                        push(s,newValueNode(newRealValue(pow(pop(s)->value->ival,temp->value->ival)),0));
                    else if (temp->value->type == REAL && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pow(pop(s)->value->rval,temp->value->rval)),0));
                    else if (temp->value->type == REAL && s->top->value->type == INTEGER)
                        push(s,newValueNode(newRealValue(pow(pop(s)->value->ival,temp->value->rval)),0));
                    else if (temp->value->type == INTEGER && s->top->value->type == REAL)
                        push(s,newValueNode(newRealValue(pow(pop(s)->value->rval,temp->value->ival)),0));
                    else if (temp->value->type == STRING && s->top->value->type == INTEGER) {
                        int i = atoi(temp->value->sval);
                        push(s,newValueNode(newRealValue(pow(pop(s)->value->ival,i)),0));
                    }
                    else if (temp->value->type == INTEGER && s->top->value->type == STRING) {
                        int i = atoi(pop(s)->value->sval);
                        push(s,newValueNode(newRealValue(pow(i,temp->value->ival)),0));
                    }
                    else if (temp->value->type == STRING && s->top->value->type == REAL) {
                        double i = atof(temp->value->sval);
                        push(s,newValueNode(newRealValue(pow(pop(s)->value->rval,i)),0));
                    }
                    else if (temp->value->type == REAL && s->top->value->type == STRING) {
                        double i = atof(pop(s)->value->sval);
                        push(s,newValueNode(newRealValue(pow(i,temp->value->rval)),0));
                    }
                }
                deQ(p);
            }
        }
        deQ(p);

        if (p->front == 0) { //print last expression answer
            if (s->top->value->type == VARIABLE) { //handle variables by looking up in tree
                value *temp1;
                temp1 = search(bstRoot,s->top->value->sval)->value;
                s->top->value = temp1;
            }
            printValue(stdout, s->top->value);
            printf("\n");
        }
    }
    return;
}

static queue* convert(queue *i) {
    queue *p = newQ(); //postfix queue
    stack *s = newStack();
    while(i->front != 0) { //loop through all infix
        //assignment
        if (i->front->next != 0 && i->front->value->type == VARIABLE && i->front->next->value->type == EQUALS) {
            enQ(p,deQ(i));
            push(s,deQ(i));
        }
        else if (isnum(i->front->value)==1) //handles operators
            enQ(p, deQ(i));

        else if (isparenthesis(i->front->value)==1) {
            if (strcmp(i->front->value->sval,"(")==0)
                push(s,deQ(i));
            else { //closed paren
                while (strcmp(s->top->value->sval,"(")!=0) //loop back to open, pop stack
                    enQ(p, pop(s));
                pop(s);
                deQ(i);
            }
        }

        else if (i->front->value->type == OPERATOR && s->top == 0)
            push(s, deQ(i));

        else { //operators on partially full stack
            if (isparenthesis(s->top->value) == 0) {
                while (s->top != 0 && priority(i->front->value->sval) <= priority(s->top->value->sval)) //precendence
                    enQ(p, pop(s));
                push(s, deQ(i));
            }
            else push(s, deQ(i));
        }
    }

    if (s->top != 0) {
        while (s->top != 0) {
            if (isparenthesis(s->top->value)==0)
                enQ(p, pop(s));
            else pop(s);
        }
    }
    return p;
}

static value *readValue(FILE *fp) { //Lusth's readValue, handles reading in values using scanner
    value *v;
    if (stringPending(fp))
        v = newStringValue(readString(fp));
    else {
        char *token = readToken(fp);
        if (token == 0)
            v = 0;
        else if (strchr(token,'.') != 0) //dot found
            v = newRealValue(atof(token));
        else if (((*token == '-' || isdigit(*token)) && strlen(token) > 1) || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else if (strcmp(token,"var")==0)
            v = newVarValue();
        else if ((token[0] >= 'A' && token[0] <= 'Z') || (token[0] >= 'a' && token[0] <= 'z'))
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

static void printValue(FILE *fp,value *v) { //Lusth's modified printValue handles printing Value
    if (v->type == INTEGER)
        fprintf(fp,"%d ",v->ival);
    else if (v->type == REAL)
        fprintf(fp,"%f ",v->rval);
    else if (v->type == OPERATOR)
        fprintf(fp,"%s ",v->sval);
    else //must be a string
        fprintf(fp,"%s ",v->sval);
}

static int priority(char *v1) {
    char * p = "=+-*/%^";
    char * i;
    char c = v1[0];
    int index;

    i = strchr(p,c);
    index = (int)(i-p);
    return index;
}

static int isnum(value *v) { //check if operand return 1 for true 0 for fals
    if (v->type == INTEGER || v->type == REAL || v->type == VARIABLE || v->type == STRING)
        return 1;
    else
        return 0;
}

static int isparenthesis(value *v) { //check for paren 1 for true 0 for false
    if (v->type == PARENTHESIS)
        return 1;
    else
        return 0;
}

void Fatal(char *fmt, ...) { //modifies Lusth's fatal, prints error exits program
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}
