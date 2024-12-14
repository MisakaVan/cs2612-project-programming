// redefine a struct-registered identifier as other types

// some structs to be used in the test
struct struct1 {};
struct struct2 {};
struct struct3 {};
struct struct4 {};
struct struct5 {};
struct struct6 {};

// 1. redefine as a variable
int struct1; // OK

// 2. redefine as an enumerator
enum SomeEnum { struct2 }; // OK

// 3. redefine as a struct
struct struct3 { int a; }; // Warning

// 4. redefine as a union
union struct4 { int a; }; // Warning

// 5. redefine as an enum
enum struct5 { a, b, c }; // Warning

// 6. redefine as a typedef
typedef int struct6; // OK