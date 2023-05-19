#pragma once

#include "interpreter.h"


static const struct var NULL_DATA_STACK_ENTRY = {.value = 0};
static const uint32_t NULL_RET_STACK_ENTRY = 0;
static const struct var NULL_VAR_MAP_ENTRY = {.value = 0};
static const struct heap_entry NULL_HEAP_ENTRY = {.value = 0, .num_of_links = 0};


struct var data_stack_top(struct data_stack *stack) {
    if (stack->num_of_entries == 0) {
        // stack underflow
    }

    struct var top = stack->data[stack->num_of_entries - 1];
    return top;
}


struct var data_stack_pop(struct data_stack *stack) {
    if (stack->num_of_entries == 0) {
        // stack underflow
    }

    struct var top = stack->data[--stack->num_of_entries];
    stack->data[stack->num_of_entries] = NULL_DATA_STACK_ENTRY;
    return top;
}


void data_stack_push(struct data_stack *stack, enum byte_code_commands type, int32_t value) {
    if (stack->num_of_entries == DATA_STACK_SIZE) {
        // stack overflow
    }

    struct var var = {.type = type, .value = value};
    stack->data[stack->num_of_entries++] = var;
}


uint32_t ret_stack_top(struct ret_stack *stack) {
    if (stack->num_of_entries == 0) {
        // stack underflow
    }

    uint32_t top = stack->data[stack->num_of_entries - 1];
    return top;
}


uint32_t ret_stack_pop(struct ret_stack *stack) {
    if (stack->num_of_entries == 0) {
        // stack underflow
    }

    uint32_t top = stack->data[--stack->num_of_entries];
    stack->data[stack->num_of_entries] = NULL_RET_STACK_ENTRY;
    return top;
}


void ret_stack_push(struct ret_stack *stack, uint32_t top) {
    if (stack->num_of_entries == RET_STACK_SIZE) {
        // stack overflow
    }

    stack->data[stack->num_of_entries++] = top;
}


struct var var_map_get(uint32_t var_index, uint32_t map_index, struct var_map_map *var_map_map) {
    if (map_index >= var_map_map->num_of_entries) {
        // index out of bounds
    }

    if (var_index >= var_map_map->data[map_index].num_of_entries) {
        // index out of bounds
    }

    return var_map_map->data[map_index].data[var_index];
}


void var_map_set(enum byte_code_commands type, int32_t value, uint32_t var_index, uint32_t map_index,
                 struct var_map_map *var_map_map) {
    if (map_index >= var_map_map->num_of_entries) {
        // index out of bounds
    }

    if (var_index >= var_map_map->data[map_index].num_of_entries) {
        // index out of bounds
    }

    struct var curr_var_in_var_index = var_map_map->data[map_index].data[var_index];

    if (type != curr_var_in_var_index.type) {
        // different types
    }

    var_map_map->data[map_index].data[var_index].value = value;
}


void var_map_push(enum byte_code_commands type, int32_t value, struct var_map_map *var_map_map) {
    if (var_map_map->data[var_map_map->num_of_entries - 1].num_of_entries == VAR_MAP_SIZE) {
        // stack overflow
    }

    struct var var = {.type = type, .value = value};

    int32_t index_to_push = var_map_map->data[var_map_map->num_of_entries - 1].num_of_entries;

    var_map_map->data[var_map_map->num_of_entries - 1].data[index_to_push] = var;

    var_map_map->data[var_map_map->num_of_entries - 1].num_of_entries++;
}


struct heap_entry heap_get_by_addr(uint32_t addr, struct heap *heap) {
    if (addr >= heap->num_of_entries) {
        // index out of bounds
    }

    return heap->data[addr];
}


uint32_t heap_insert(void *value, enum byte_code_commands type, struct heap *heap) {
    if (heap->num_of_entries == HEAP_SIZE) {
        // stack overflow
    }

    struct heap_entry heap_entry = {.num_of_links = 1, .type = type, .value = value};

    heap->data[heap->num_of_entries++] = heap_entry;

    return heap->num_of_entries - 1;
}


void heap_dec_num_of_links(uint32_t addr, struct heap *heap) {
    if (heap->data[addr].num_of_links == 0) {
        return; // memory is free already
    }

    heap->data[addr].num_of_links--;

    if (heap->data[addr].num_of_links == 0) {
        heap->data[addr] = NULL_HEAP_ENTRY;
        heap->num_of_entries--;
        if (heap->next_free_entry > addr) {
            heap->next_free_entry = addr;
        }
    }
}


static int32_t get_fint_fvar(uint32_t *curr_command_addr, const int32_t *byte_code, struct interpreter *interpreter) {
    int32_t value;

    if (byte_code[++(*curr_command_addr)] == FINT) {
        value = byte_code[++(*curr_command_addr)];
    } else {
        uint32_t var_index = byte_code[++(*curr_command_addr)];
        uint32_t map_index = interpreter->var_map_map->num_of_entries - 1;
        value = var_map_get(var_index, map_index, interpreter->var_map_map).value;
    }

    return value;
}


void interpret(struct interpreter *interpreter, int32_t *byte_code, uint32_t start) {
    uint32_t curr_command_addr = start;

    while (byte_code[curr_command_addr] != EXIT) {
        switch (byte_code[curr_command_addr]) {
            case EXIT: {
                // exit(0);
                break;
            }
            case JMP: {
                curr_command_addr = byte_code[curr_command_addr + 1];
                break;
            }
            case JDEC: {
                curr_command_addr++; // now curr_command_addr points to offset relative to current addr
                uint32_t temp = curr_command_addr;
                curr_command_addr += byte_code[curr_command_addr];
                uint32_t counter = --byte_code[curr_command_addr];
                if (!counter) {
                    curr_command_addr = temp;
                }
                break;
            }
            case JRET: {
                curr_command_addr = ret_stack_pop(interpreter->ret_stack);
                break;
            }
            case LOOP: {
                int32_t lower_bound = get_fint_fvar(&curr_command_addr, byte_code, interpreter);
                int32_t upper_bound = get_fint_fvar(&curr_command_addr, byte_code, interpreter);
                int32_t step = get_fint_fvar(&curr_command_addr, byte_code, interpreter);
                int32_t counter = (upper_bound - lower_bound) / step;
                byte_code[++curr_command_addr] = counter;
                break;
            }
            case FINT: {
                // can't meet
                break;
            }
            case FVAR: {
                // can't meet
                break;
            }
            case JT: {
                struct var var = data_stack_pop(interpreter->data_stack);
                if (var.type != BOOL) {
                    // in condition was call of func that returns not bool value
                }

                if (var.value == 0) {
                    curr_command_addr += byte_code[curr_command_addr + 1];
                } else {
                    curr_command_addr++;
                }

                break;
            }
            case CALL: {
                curr_command_addr = byte_code[curr_command_addr + 1];
                break;
            }
            case LIT: {
                enum byte_code_commands type = byte_code[++curr_command_addr];
                int32_t value = byte_code[++curr_command_addr];
                data_stack_push(interpreter->data_stack, type, value);
                break;
            }
            case VAR: {
                struct var var = var_map_get(byte_code[++curr_command_addr],
                                             interpreter->var_map_map->num_of_entries,
                                             interpreter->var_map_map);
                data_stack_push(interpreter->data_stack, var.type, var.value);
            }
            case OFC: {
                break;
            }
            case RLIT: {

            }
            case RVAR: {

            }
            case ROFC: {

            }
            case BOOL: {

            }
            case INT: {

            }
            case FLT: {

            }
            case STR: {

            }
            case PTR: {

            }
            case VOID: {

            }


            case PRINT: {

            }
            case ASSIGN: {

            }
            case GET_DATA: {

            }
            case SET_DATA: {

            }
            case GET_ADDR: {

            }
            case MALLOC: {

            }
            case AND: {

            }
            case OR: {

            }
            case NOT: {

            }
            case CONCAT: {

            }
            case SUBSTR: {

            }
            case LIKE: {

            }
            case LENGTH: {

            }
            case ABS: {

            }
            case SIN: {

            }
            case COS: {

            }
            case INC: {

            }
            case DEC: {

            }
            case LOG: {

            }
            case POW: {

            }
            case SUM: {

            }
            case SUB: {

            }
            case MUL: {

            }
            case DIV: {

            }
            case MOD: {

            }
            case MIN: {

            }
            case MAX: {

            }
            case LESS: {

            }
            case GREATER: {

            }
            case EQUAL: {

            }
            case PI: {

            }
            case E: {

            }
            case RANDOM: {

            }
            case MAIN: {
                break;
            }
            default: {

            }
        }

        curr_command_addr++;
    }
}