// redefine a union-registered identifier as other types

// some unions to be used in the test
union union1 {};
union union2 {};
union union3 {};
union union4 {};
union union5 {};
union union6 {};

// 1. redefine as a variable
int union1; // OK

// 2. redefine as an enumerator
enum SomeEnum { union2 }; // OK

// 3. redefine as a struct
struct union3 { int a; }; // Warning

// 4. redefine as a union
union union4 { int a; }; // Warning

// 5. redefine as an enum
enum union5 { a, b, c }; // Warning

// 6. redefine as a typedef
typedef int union6; // OK