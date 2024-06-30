#ifndef _LZSTACK_H_
#define _LZSTACK_H_

#include <stdlib.h>

typedef struct _lzstack_allocator_
{
    void *(*lzstack_alloc)(size_t byes, int dynamic);
    void (*lzstack_dealloc)(void *ptr, int dynamic);
} LZStackAllocator;

typedef struct _lzstack_node_
{
    void *value;
    struct _lzstack_node_ *previous;
} LZStackNode;

typedef struct _lzstack_
{
    struct _lzstack_node_ *nodes;
    struct _lzstack_allocator_ *allocator;
} LZStack;

struct _lzstack_ *lzstack_create(struct _lzstack_allocator_ *allocator);
void lzstack_destroy(struct _lzstack_ *stack);

void *lzstack_peek(struct _lzstack_ *stack, struct _lzstack_node_ **out_node);
int lzstack_push(void *value, struct _lzstack_ *stack, struct _lzstack_node_ **out_node);
void *lzstack_pop(struct _lzstack_ *stack);

int lzstack_count(struct _lzstack_ *stack);

#endif