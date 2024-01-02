#include <stdio.h>
#include "ast.h"

#define YY_STDINIT
#define YYSTYPE double
extern YYSTYPE yylval;

extern int yyparse(void);
extern FILE *yyin;
void yyerror(const char *s) {
    printf("ERROR: %s\n", s);
}
static int max_id = 0;

char *typeString[22] = {
        "NODE_TYPE_ROOT",
        "NODE_TYPE_FOR",
        "NODE_TYPE_FILTER",
        "NODE_TYPE_RETURN",
        "NODE_TYPE_REMOVE",
        "NODE_TYPE_INSERT",
        "NODE_TYPE_UPDATE",
        "NODE_TYPE_VARIABLE",
        "NODE_TYPE_REFERENCE",
        "NODE_TYPE_DATA_SOURCE",
        "NODE_TYPE_ATTRIBUTE_ACCESS",
        "NODE_TYPE_OPERATOR_BINARY_OR",
        "NODE_TYPE_OPERATOR_BINARY_AND",
        "NODE_TYPE_OPERATOR_BINARY_EQ",
        "NODE_TYPE_OPERATOR_BINARY_NE",
        "NODE_TYPE_OPERATOR_BINARY_LT",
        "NODE_TYPE_OPERATOR_BINARY_GT",
        "NODE_TYPE_OPERATOR_BINARY_LE",
        "NODE_TYPE_OPERATOR_BINARY_GE",
        "NODE_TYPE_OPERATOR_BINARY_IN",
        "NODE_TYPE_OBJECT",
        "NODE_TYPE_OBJECT_ELEMENT"
};

static char* getAstNodeVal(struct AstNodeValue astNodeValue) {
    switch (astNodeValue.type)
    {
        case INT_TYPE: {
            int length = snprintf( NULL, 0, "%d", astNodeValue._int );
            char* str = malloc( length + 1 );
            snprintf( str, length + 1, "%d", astNodeValue._int);
            return str;
        }
        case BOOL_TYPE: {
            int length = snprintf( NULL, 0, "%d", astNodeValue._int );
            char* str = malloc( length + 1 );
            snprintf( str, length + 1, "%d", astNodeValue._int);
            return str;
        }
        case DOUBLE_TYPE: {
            int length = snprintf( NULL, 0, "%f", astNodeValue._double );
            char* str = malloc( length + 1 );
            snprintf( str, length + 1, "%f", astNodeValue._double);
            return str;
        }
        default:
            return astNodeValue._string;
    }
}

static char* getValue(struct AstNode *node) {
    switch (node->type) {
        case NODE_TYPE_VARIABLE:
            return getAstNodeVal(node->value);
        case NODE_TYPE_REFERENCE:
            return node->value._string;
        case NODE_TYPE_DATA_SOURCE:
            return node->value._string;
        case NODE_TYPE_ATTRIBUTE_ACCESS:
            return node->value._string;
        case NODE_TYPE_OBJECT_ELEMENT:
            return node->value._string;
        default:
            return "NULL";
    }
}

static void generateDotCode(struct AstNode *node, FILE *dotFile, int id) {
    if (node == NULL) {
        return;
    }
    fprintf(dotFile, "\t%d [ label=\"Type: %s Value: %s\" ];\n", id, typeString[node->type], getValue(node));
    for (size_t i = 0; i < node->children.children_count; i++) {
        struct AstNode *childNode = node->children.data[i];
        max_id++;
        fprintf(dotFile, "\t\t%d -> %d;\n", id, max_id);
        generateDotCode(childNode, dotFile, max_id);
    }
}


int main(int argc, char **argv) {
    init_ast();
    ++argv;
    --argc;
    char* filePath = "../test/test.txt";
    yyin = fopen(filePath, "r");
    if (!yyin) {
        printf("Can't open file %s", filePath);
        return 1;
    }
    FILE *dotFile = fopen("../test/ast.dot", "w+");
    if (dotFile == NULL) {
        perror("Failed to open DOT file");
        return 1;
    }
    // Parse
    yyparse();
    struct AstNode *root = get_ast_root();
    fprintf(dotFile, "digraph AstTree {\n");
    generateDotCode(root, dotFile, 0);
    fprintf(dotFile, "}\n");
    fclose(dotFile);
    return 0;
}

