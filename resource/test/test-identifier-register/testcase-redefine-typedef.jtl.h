// redefine a typedef-registered identifier as other types

// some types to be used in the test
typedef int int_t1;
typedef int int_t2;
typedef int int_t3;
typedef int int_t4;
typedef int int_t5;
typedef int int_t6;

// 1. redefine as a variable
int int_t1; // Warning

// 2. redefine as an enumerator
enum SomeEnum { int_t2 }; // Warning

// 3. redefine as a struct
struct int_t3 { int a; }; // OK

// 4. redefine as a union
union int_t4 { int a; }; // OK

// 5. redefine as an enum
enum int_t5 { a, b, c }; // OK

// 6. redefine as a typedef
typedef int int_t6; // Warning