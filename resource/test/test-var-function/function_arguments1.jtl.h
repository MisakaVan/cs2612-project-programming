// void func
void hello();

char foo(
    int       , // int <empty>
    int [5]   , // int <empty>[5]
    int ()    , // int <empty>()
    int (())  , // int (<empty>())

    int  *    , // int  *<empty>
    int (*)   , // int (*<empty>)
    int ((*)) , // int ((*<empty>))
    int (*()) , // int (*<empty>())

    int *   (), // int * <empty>()
    int **  (), // int **<empty>()
    int *** (), // int ***<empty>()
    int *(*)(), // int * (*<empty>)()

    int * (*) , // int * (*<empty>)

    int    () , // int <empty>()
    int (*)()   // int (*<empty>)()
);

void bar(
    void *[5]       , // void *<empty>[5]
    void *(*())     , // void *(*<empty>())
    void *(*)[5]      // void *(*<empty>)[5]
);