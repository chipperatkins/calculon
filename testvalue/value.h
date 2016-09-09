typedef struct valueobject
    {
    char *type;

    int ival;
    double rval;
    char *sval;
    } value;

extern value *newIntegerValue(int);
extern value *newRealValue(double);
extern value *newStringValue(char *);
extern value *newVariableValue(char *);
extern value *newOperatorValue(char *);
extern value *newSemicolonValue(void);
extern void displayValue(FILE *,value *);

extern char *INTEGER;
extern char *REAL;
extern char *STRING;
extern char *VARIABLE;
extern char *OPERATOR;
extern char *SEMICOLON;
