#include "lang.h"
#include "lexer.h"
#include "parser.h"
#include "astprint.h"
#include <stdio.h>

// use the new printer
// #define ASTPRINT

// clang-format off
extern struct glob_item_list *root;
int yyparse();
// clang-format on

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Error, not enough arguments!\n");
        return 0;
    }
    if (argc >= 3) {
        printf("Error, too many arguments!\n");
        return 0;
    }
    yyin = fopen(argv[1], "rb");
    if (yyin == NULL) {
        printf("File %s can't be opened.\n", argv[1]);
        return 0;
    }
    yyparse();
    fclose(yyin);

#ifdef ASTPRINT
    astprint_glob_item_list(root);
#else
    print_glob_item_list(root);
#endif

    return 0;
}
