#pragma once

#pragma once

#include <inttypes.h>
#include <stdio.h>

#define DEFAULT // define your structs sizes (in bytes). They depend on memory num_of_entries

#ifdef DEFAULT
#define DATA_STACK_SIZE 1024 // max num_of_entries - 65536
#define RET_STACK_SIZE 1024 // max num_of_entries - 65536
#define VAR_MAP_SIZE 1024 // max num_of_entries - 65536
#define HEAP_SIZE 1024 // max num_of_entries - 4294967296
#endif

#define NULL_BOOL_VALUE 0
#define NULL_INT_VALUE 0
#define NULL_FLT_VALUE 0
#define NULL_PTR_VALUE HEAP_SIZE
#define NULL_STR_VALUE (-1)


// bytecode commands
enum byte_code_commands {
    EXIT = 0,
    JMP = 1,
    JDEC = 2,
    JRET = 3,
    LOOP = 10,
    FINT = 11,
    FVAR = 12,
    JT = 20,
    CALL = 100,
    LIT = 110,
    VAR = 111,
    OFC = 112,
    RLIT = 120,
    RVAR = 121,
    ROFC = 122,
    BOOL = 200,
    INT = 201,
    FLT = 202,
    STR = 203,
    PTR = 204,
    VOID = 205,
    NOT_YET_DEFINED_TYPE = 206,
    PRINT = 700,
    ASSIGN = 701,
    GET_DATA = 710,
    SET_DATA = 711,
    GET_ADDR = 712,
    MALLOC = 713,
    AND = 720,
    OR = 721,
    NOT = 722,
    CONCAT = 725,
    SUBSTR = 726,
    LIKE = 727,
    LENGTH = 728,
    ABS = 735,
    SIN = 736,
    COS = 737,
    INC = 738,
    DEC = 739,
    LOG = 755,
    POW = 756,
    SUM = 757,
    SUB = 758,
    MUL = 759,
    DIV = 760,
    MOD = 761,
    MIN = 762,
    MAX = 763,
    LESS = 764,
    GREATER = 765,
    EQUAL = 766,
    PI = 775,
    E = 776,
    RANDOM = 777,
    MAIN = 999
};


// structs
struct var {
    enum byte_code_commands type;
    int32_t value;
};


struct data_stack {
    uint16_t num_of_entries;
    struct var *data;
};


struct ret_stack {
    uint16_t num_of_entries;
    uint32_t *data;
};


struct var_map {
    uint16_t num_of_entries;
    struct var *data;
};


struct var_map_map {
    uint8_t num_of_entries;
    struct var_map *data;
};


struct heap_entry {
    uint8_t num_of_links;
    enum byte_code_commands type;
    int32_t value; // null by default
};


struct heap {
    uint32_t num_of_entries;
    struct heap_entry *data; // index - addr, value - heap_entry
    uint32_t next_free_entry; // index of the next free addr in the data.
};


struct interpreter {
    struct data_stack *data_stack;
    struct ret_stack *ret_stack;
    struct var_map_map *var_map_map;
    struct heap *heap;
};


void interpret(struct interpreter *interpreter, int32_t *byte_code, uint32_t start);


// GLOBAL STRUCTS
const struct var NULL_DATA_STACK_ENTRY = {.type = NOT_YET_DEFINED_TYPE, .value = 0};
const uint32_t NULL_RET_STACK_ENTRY = 0;
const struct var NULL_VAR_MAP_ENTRY = {.type = NOT_YET_DEFINED_TYPE, .value = 0};
const struct heap_entry NULL_HEAP_ENTRY = {.num_of_links = 0, .type = NOT_YET_DEFINED_TYPE, .value = 0,};


// IMPLEMENTATION
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


void var_map_set(enum byte_code_commands type, int32_t value,
                 uint32_t var_index, uint32_t map_index, struct var_map_map *var_map_map) {
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


struct heap_entry heap_get(uint32_t addr, struct heap *heap) {
    if (addr >= heap->num_of_entries) {
        // index out of bounds
    }

    return heap->data[addr];
}


void heap_set(uint32_t addr, enum byte_code_commands type, int32_t value, struct heap *heap) {
    if (type != heap->data[addr].type) {
        // different types
    }

    heap->data[addr].value = value;
}


// return addr at which value was inserted
uint32_t heap_push(enum byte_code_commands type, int32_t value, struct heap *heap) {
    if (heap->next_free_entry == HEAP_SIZE) {
        // stack overflow
    }

    struct heap_entry heap_entry = {.num_of_links = 1, .type = type, .value = value};

    heap->data[heap->next_free_entry] = heap_entry;

    heap->num_of_entries++;

    if (heap->num_of_entries == HEAP_SIZE) {
        heap->next_free_entry = HEAP_SIZE;
    } else {
        for (size_t i = heap->next_free_entry + 1; i < HEAP_SIZE; i++) {
            if (heap->data[i].num_of_links == 0) {
                heap->next_free_entry = i;
                break;
            }
        }
    }

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


int32_t get_fint_fvar(uint32_t *curr_command_addr, const int32_t *byte_code, struct interpreter *interpreter) {
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


int32_t get_int_bits(uint32_t bits, int32_t from, int32_t to) { // from and to includes
    return bits << (31 - from) >> (to + 31 - from);
}


struct var get_by_addr(int32_t addr, struct heap *heap, struct var_map_map *var_map_map) {
    int32_t is_in_var_map = get_int_bits(addr, 31, 31);

    if (!is_in_var_map) {
        struct heap_entry heap_entry = heap_get(addr, heap);
        return (struct var) {.type = heap_entry.type, .value = heap_entry.value};
    } else {
        int32_t map_index = get_int_bits(addr, 30, 16);
        int32_t var_index = get_int_bits(addr, 15, 0);
        return var_map_get(var_index, map_index, var_map_map);
    }
}


void set_by_addr(int32_t addr, int32_t value, enum byte_code_commands type,
                 struct heap *heap, struct var_map_map *var_map_map) {
    int32_t is_in_var_map = get_int_bits(addr, 31, 31);

    if (!is_in_var_map) {
        heap_set(addr, type, value, heap);
    } else {
        int32_t map_index = get_int_bits(addr, 30, 16);
        int32_t var_index = get_int_bits(addr, 15, 0);
        var_map_set(type, value, var_index, map_index, var_map_map);
    }
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



                // TODO remove var_map

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
                ret_stack_push(interpreter->ret_stack, curr_command_addr + 2);
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
                enum byte_code_commands type = byte_code[++curr_command_addr];
                int32_t value = byte_code[++curr_command_addr];
                data_stack_push(interpreter->data_stack, type, value);
                break;
            }
            case RVAR: {
                struct var var = var_map_get(byte_code[++curr_command_addr],
                                             interpreter->var_map_map->num_of_entries,
                                             interpreter->var_map_map);
                data_stack_push(interpreter->data_stack, var.type, var.value);
            }
            case ROFC: {
                break;
            }
            case BOOL: {
                var_map_push(BOOL, NULL_BOOL_VALUE, interpreter->var_map_map);
                break;
            }
            case INT: {
                var_map_push(INT, NULL_INT_VALUE, interpreter->var_map_map);
                break;
            }
            case FLT: {
                var_map_push(FLT, NULL_FLT_VALUE, interpreter->var_map_map);
                break;
            }
            case STR: {
                var_map_push(STR, NULL_STR_VALUE, interpreter->var_map_map);
                break;
            }
            case PTR: {
                var_map_push(PTR, NULL_PTR_VALUE, interpreter->var_map_map);
                break;
            }
            case VOID: {
                // can't meet
                break;
            }

            case PRINT: {
                int32_t value = data_stack_pop(interpreter->data_stack).value;
                int32_t addr = data_stack_pop(interpreter->data_stack).value;

            }
            case GET_DATA: {

            }
            case SET_DATA: {
                struct var var = data_stack_pop(interpreter->data_stack);

                if (var.type != PTR) {
                    // different types
                }

                int32_t addr = var.value;


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


int main() {
    struct var data_stack_data[DATA_STACK_SIZE];
    uint32_t ret_stack_data[RET_STACK_SIZE];
    struct var var_map_data[VAR_MAP_SIZE];
    struct var_map var_map_map_data[RET_STACK_SIZE];
    struct heap_entry heap_data[HEAP_SIZE];


    for (uint32_t i = 0; i < DATA_STACK_SIZE; i++) {
        data_stack_data[i] = NULL_DATA_STACK_ENTRY;
    }

    for (uint32_t i = 0; i < RET_STACK_SIZE; i++) {
        ret_stack_data[i] = NULL_RET_STACK_ENTRY;
    }

    for (uint32_t i = 0; i < VAR_MAP_SIZE; i++) {
        var_map_data[i] = NULL_VAR_MAP_ENTRY;
    }

    for (uint32_t i = 0; i < RET_STACK_SIZE; i++) {
        var_map_map_data[i].data = var_map_data;
    }

    for (uint32_t i = 0; i < HEAP_SIZE; i++) {
        heap_data[i] = NULL_HEAP_ENTRY;
    }


    struct data_stack my_data_stack = {.num_of_entries = 0, .data = data_stack_data};
    struct ret_stack my_ret_stack = {.num_of_entries = 0, .data = ret_stack_data};
    struct var_map_map my_var_map_map = {.num_of_entries = 1, .data = var_map_map_data};
    struct heap my_heap = {.num_of_entries = 0, .data = heap_data, .next_free_entry = 0};
    struct interpreter my_interpreter = {
            .data_stack = &my_data_stack,
            .ret_stack = &my_ret_stack,
            .var_map_map = &my_var_map_map,
            .heap = &my_heap
    };

    uint32_t main_program_start = 21;

    int32_t byte_code[] = {
            201,
            2,
            203,
            203,
            10,
            11,
            1,
            11,
            10,
            11,
            1,
            0,
            110,
            201,
            10,
            700,
            2,
            -6,
            3,
            999,
            110,
            203,
            31,
            110,
            203,
            38,
            100,
            0,
            112,
            700,
            0,
            6,
            113,
            119,
            101,
            114,
            116,
            121,
            3,
            1,
            2,
            3
    };

//    interpret(&my_interpreter, byte_code, main_program_start);

    get_by_addr(UINT32_MAX, &my_heap);

    return 0;
}
