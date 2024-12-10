// simple typedef
typedef int int_t;
typedef char char_t;

// typedef with pointer
typedef int* int_ptr_t;
typedef int** int_ptr_ptr_t;

// typedef with array
typedef int arr_of_int_t[10];
typedef int *arr_of_ptr_to_int_t[10]; // array of pointers to int
typedef int (*ptr_to_arr_of_int_t)[10]; // pointer to array of int

// typedef with function pointer
// points to (int, int) -> char
typedef char (*func_ptr_t)(int, int);
// points to (int, int, int) -> pointer to array of pointer to (char, char) -> pointer to int
typedef int *
            (*
                (*
                    (*func)(int, int, int)
                )
                [5]
            )
        (char, char);

struct TestStruct {
    int_ptr_t (*func)(func_ptr_t, int_ptr_t);
};