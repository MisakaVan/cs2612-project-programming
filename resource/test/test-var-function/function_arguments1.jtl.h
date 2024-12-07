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







