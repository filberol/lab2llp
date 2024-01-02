#include "../include/ast_node.h"

static void realloc_children_list(struct AstNode* parent) {
    struct AstNode** newData = calloc(parent->children.capacity * 2 + 2, sizeof(struct AstNode*));
    parent->children.capacity = parent->children.capacity * 2 + 2;
    memcpy(newData, parent->children.data, parent->children.children_count * sizeof(struct AstNode*));
    free(parent->children.data);
    parent->children.data = newData;
}

void add_ast_node(struct AstNode* parent, struct AstNode* children) {
    if(parent->children.capacity <= parent->children.children_count + 1) {
        realloc_children_list(parent);
    }
    parent->children.data[parent->children.children_count] = children;
    parent->children.children_count++;
}

// String utils for nodes
char *copy_string(const char *src) {
    char *dest = malloc(strlen(src));
    strcpy(dest, src);
    return dest;
}

char *copy_quoted_string(const char *src) {
    char *dest = malloc(strlen(src) - 2);
    memcpy(dest, src + 1, strlen(src) - 2); // NOLINT(bugprone-not-null-terminated-result)
    return dest;
}