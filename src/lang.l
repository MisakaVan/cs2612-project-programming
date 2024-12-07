%option noyywrap yylineno

%{
#include "lib.h"
#include "lang.h"
#include "parser.h"
%}

%%


0|[1-9][0-9]* {   
    /* natual number */
    yylval.natVal = build_nat(yytext, yyleng);
    return TM_NAT;
}

    /* reserved words that may be used as identifiers comes first */

"enum"          { return TM_ENUM; }
"struct"        { return TM_STRUCT; }
"union"         { return TM_UNION; }
"typedef"       { return TM_TYPEDEF; }
"int"           { return TM_INT; }
"char"          { return TM_CHAR; }

    /* Then parse the identifiers */

[_A-Za-z][_A-Za-z0-9]* {   
    /* identifier */
    yylval.strVal = new_str(yytext, yyleng);
    return TM_IDENT;
}

    /* operators */

"{"             { return TM_LEFT_BRACE; }
"}"             { return TM_RIGHT_BRACE; }
"["             { return TM_LEFT_BRACKET; }
"]"             { return TM_RIGHT_BRACKET; }
"("             { return TM_LEFT_PAREN; }
")"             { return TM_RIGHT_PAREN; }
";"             { return TM_SEMICOL; }
"*"             { return TM_STAR; }
","             { return TM_COMMA; }

[ \t\n\r]         { /* ignore white space */ }

"//".*          { /* ignore line comment */ }

.               { printf("Unrecognized character: '%s'\n", yytext); return -1; }

%%

