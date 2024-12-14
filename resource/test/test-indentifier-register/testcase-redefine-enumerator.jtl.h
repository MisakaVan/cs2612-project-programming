// redefine a enumerator-registered identifier as other types

// some enumerators to be used in the test
enum Foo { var1, var2, var3, var4, var5, var6 };

// 1. redefine as a variable
int var1; // Warning

// 2. redefine as an enumerator
enum SomeEnum { var2 }; // Warning

// 3. redefine as a struct
struct var3 { int a; }; // OK

// 4. redefine as a union
union var4 { int a; }; // OK

// 5. redefine as an enum
enum var5 { a, b, c }; // OK

// 6. redefine as a typedef
typedef int var6; // Warning