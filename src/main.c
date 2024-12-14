#include "lang.h"
#include "lexer.h"
#include "parser.h"
#include "astprint.h"
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

// use the new printer
// #define ASTPRINT

// clang-format off
extern struct glob_item_list *root;
extern struct SLL_hash_table *identifier_table;
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

    // read "PARSER_VERBOSE" environment variable
    char *log_level_str = getenv("PARSER_VERBOSE");
    if (log_level_str != NULL) {
        log_level = LOG_LEVEL_VERBOSE;
    }

    // initialize the identifier table
    identifier_table = init_SLL_hash(); 

    yyin = fopen(argv[1], "rb");
    if (yyin == NULL) {
        printf("File %s can't be opened.\n", argv[1]);
        return 0;
    }
    yyparse();
    fclose(yyin);

#ifdef ASTPRINT
    set_var_decl_expr_printer(VAR_DECL_EXPR_PRINTER_BOTH);
    astprint_glob_item_list(root);
#else
    print_glob_item_list(root);
#endif

    return 0;
}
