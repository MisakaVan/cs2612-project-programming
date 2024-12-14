// declare struct but not define it
struct SomeStruct;

// define a simple struct with two fields
struct Foo {
    int a;
    int b;
};

// define a struct with more complex fields
struct Bar {
    struct Foo foo;
    struct SomeStruct* ptr;

    // declare a function pointer
    void (*func)(int);
};

// define an empty struct
struct EmptyStruct {};

// define a struct with newly defined type
struct WeatherConditions {
    enum Weather {
        SUNNY,
        RAINY,
        CLOUDY,
        SNOWY
    } weather;
    struct Wind {
        char* direction;
        int speed;
    } wind;
    union {
        char* warning[5];
        char* advice;
    } info;
    int temperature;
};

// define a complex struct
struct ComplexStruct {
    struct WeatherConditions (*func1)(int, struct Foo*);
};