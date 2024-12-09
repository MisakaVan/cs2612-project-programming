// typedef with struct
typedef struct {
    int id;
    char name[50];
    int (*func)(double, char);
} FooStruct;
typedef struct {} EmptyStruct;

// typedef with enum
typedef enum {
    ENUM_A,
    ENUM_B
} FooEnum;

// typedef with union and nested struct
typedef union {
    float value;
    char name[20];
    struct {
        double doubleValue;
        char nestedChar;
    } nestedStruct;
} FooUnion;

// typedef with func pointer
typedef int (*funcPtr)(double, char);

// nested typedef declaration
typedef struct {
    FooStruct someStruct;
    FooUnion someSnion;
    funcPtr ptr;
} NestedStruct;

// use newly-defined types in function declaration
FooStruct createFooStruct(int, char*, int (*)(double, char));
FooStruct createFooStruct2(int, char*, funcPtr); // same as above
char* getFooName(FooStruct);
FooEnum getEnumValue(int);
EmptyStruct returnEmptyStruct();
NestedStruct createNestedStruct(FooStruct, FooUnion, funcPtr);