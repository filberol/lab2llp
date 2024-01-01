%option yylineno
%option noyywrap nounput batch

%{
#include "../include/ast.h"
#include "./grammar.h"
#define YY_NO_INPUT 1
%}

%%
"FOR" { return T_FOR; }
"FILTER" { return T_FILTER; }
"RETURN" { return T_RETURN; }

"{" { return T_OBJECT_OPEN; }
"}" { return T_OBJECT_CLOSE; }

"IN" { return T_IN; }
"REMOVE" { return T_REMOVE; }
"INSERT" { return T_INSERT; }
"UPDATE" { return T_UPDATE; }

"TRUE" { return T_TRUE; }
"FALSE" { return T_FALSE; }

"==" { return T_EQ; }
"!=" { return T_NE; }
">=" { return T_GE; }
">" { return T_GT; }
"<=" { return T_LE; }
"<" { return T_LT; }
"," { return T_COMMA;
"(" { return T_OPEN; }
")" { return T_CLOSE; }
":" { return T_COLON; }
"!" { return T_NOT; }
"&&" { return T_AND; }
"||" { return T_OR; }

(\"[^\"]*\") {
  /* quoted string */
  yylval.s_val = yytext;
  printf("%s", yytext);
  return T_QUOTED_STRING;
}

(($?|_+)[a-zA-Z]+[_a-zA-Z0-9]*) {
  /* unquoted string */
  yylval.s_val = yytext;
  return T_STRING;
}

(0|[1-9][0-9]*) {
  /* a numeric integer value, base 10 (decimal) */
  yylval.node = createNodeValueInt(atoi(yytext));
  return T_INTEGER;
}

((0|[1-9][0-9]*)(\.[0-9]+)?|\.[0-9]+)([eE][\-\+]?[0-9]+)? {
  /* a numeric double value, base 10 (decimal) */
  double value = atof(yytext);
  yylval.node = createNodeValueDouble(value);
  return T_DOUBLE;
}

[ \t\r]+ {
  /* whitespace is ignored */
}

. {
  /* anything else is returned as it is */
  return (int) yytext[0];
}
%%
