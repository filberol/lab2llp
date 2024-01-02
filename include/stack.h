#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct CommonStack {
    uint32_t capacity;
    uint32_t stored_count;
    void** data;
};

struct CommonStack* init_common_stack();

void* pop_common_stack(struct CommonStack* stack);
void push_common_stack(struct CommonStack* stack, void* data);
void* peek_common_stack(struct CommonStack* stack);

struct VariableNode {
    char* variable_name;
    void* value;
};

struct VariableStack {
    uint32_t capacity;
    uint32_t stored_count;
    struct VariableNode* data;
};

struct VariableStack* init_variable_stack(uint32_t capacity);

void add_variable_data(struct VariableStack* stack, struct VariableNode node);
struct VariableNode* get_stack_variable(struct VariableStack* stack, char* name);

#endif //STACK_H