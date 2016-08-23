typedef struct value
{
    int type;
    int ival;
    double rval;
    char *sval;
}value;

extern int INTEGER;
extern int REAL;
extern int STRING;
extern int VARIABLE;

extern value *newValueI(int);
extern value *newValueR(double);
extern value *newValueS(char *);
extern value *newValueV(char *);
