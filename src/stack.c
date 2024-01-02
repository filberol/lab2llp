#include "stack.h"

#define STACK_INIT_CAPACITY 16

struct CommonStack* init_common_stack() {
    struct CommonStack* stack = malloc(sizeof(struct CommonStack));
    stack->data = calloc(STACK_INIT_CAPACITY, sizeof(void*));
    stack->capacity = STACK_INIT_CAPACITY;
    stack->stored_count = 0;
    return stack;
}

static void realloc_common_stack(struct CommonStack* stack) {
    void *new_data = calloc(stack->capacity * 2, sizeof(void*));
    memcpy(new_data, stack->data, stack->stored_count * sizeof(void*));
    free(stack->data);
    stack->data = new_data;
}

void push_common_stack(struct CommonStack* stack, void* data) {
    if (stack->capacity < stack->stored_count + 1) {
        realloc_common_stack(stack);
    }
    stack->data[stack->stored_count] = data;
    stack->stored_count++;
}

void* pop_common_stack(struct CommonStack* stack) {
    if (stack->stored_count <= 0) return NULL;
    stack->stored_count--;
    return stack->data[stack->stored_count];
}

void* peek_common_stack(struct CommonStack* stack) {
    if (stack->stored_count <= 0) return NULL;
    return stack->data[stack->stored_count - 1];
}

//хранит информацию о переменных в текущем скоупе
static void reallocStack(struct VariableStack *stack) {
    void *newData = calloc(stack->capacity * 2, sizeof(struct VariableNode));
    memcpy(newData, stack->data, stack->stored_count * sizeof(struct VariableNode));
    free(stack->data);
    stack->data = newData;
}

void add_variable_data(struct VariableStack *stack, struct VariableNode node) {
    if (stack->stored_count + 1 < stack->capacity) {
        reallocStack(stack);
    }
    stack->data[stack->stored_count] = node;
    stack->stored_count++;
}

struct VariableStack *init_variable_stack(uint32_t capacity) {
    void *data = calloc(capacity, sizeof(struct VariableNode));
    struct VariableStack *stack = malloc(sizeof(struct VariableStack));
    stack->stored_count = 0;
    stack->capacity = capacity;
    stack->data = data;
    return stack;
}

struct VariableNode *get_stack_variable(struct VariableStack *stack, char *name) {
    for (int i = 0; i < stack->stored_count; i++) {
        if (strcmp(name, stack->data[i].variable_name) == 0) {
            return &stack->data[i];
        }
    }
    return NULL;
}
