// introduce struct/union/enum/typedef
struct SomeStruct someStructInstance;
struct Foo {
    int a;
    int b;
} fooInstance;
struct {
    struct Foo foo;
} anonymousStructInstance;


union SomeUnion someUnionInstance;
union Bar {
    char a;
    int b;
} barInstance;
union {
    union Bar bar;
} anonymousUnionInstance;

enum SomeEnum {
    ENUM_A,
    ENUM_B
} someEnumInstance;

enum {
    ENUM_ANONYMOUS_A,
    ENUM_ANONYMOUS_B
} anonymousEnumInstance;


typedef int int_t;

int_t int_t_instance;

