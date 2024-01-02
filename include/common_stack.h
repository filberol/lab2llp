#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "./variable.h"

struct CommonStack {
    int32_t capacity;
    int32_t stored_count;
    void** data;
};

struct CommonStack* init_common_stack();

void* pop_common_stack(struct CommonStack* stack);
void push_common_stack(struct CommonStack* stack, void* data);
void* peek_common_stack(struct CommonStack* stack);

#endif //STACK_H