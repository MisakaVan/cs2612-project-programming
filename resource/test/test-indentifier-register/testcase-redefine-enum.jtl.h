// redefine a enum-registered identifier as other types

// some enums to be used in the test
enum enum1 { e1 };
enum enum2 { e2 };
enum enum3 { e3 };
enum enum4 { e4 };
enum enum5 { e5 };
enum enum6 { e6 };

// 1. redefine as a variable
int enum1; // OK

// 2. redefine as an enumerator
enum SomeEnum { enum2 }; // OK

// 3. redefine as a struct
struct enum3 { int a; }; // Warning

// 4. redefine as a union
union enum4 { int a; }; // Warning

// 5. redefine as an enum
enum enum5 { a, b, c }; // Warning

// 6. redefine as a typedef
typedef int enum6; // OK