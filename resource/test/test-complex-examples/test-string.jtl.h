// // A simple string implementation header file.

typedef struct String_ {
    char* data;
    int length;
} String;

String* String_init(char *);
void String_free(String *);
int String_length(String *);
String* String_concat(String *, String *);
int String_compare(String *, String *);