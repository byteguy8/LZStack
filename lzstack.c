#include "lzstack.h"

// private interface
static void *_lzstack_alloc_(size_t bytes, int dynamic, struct _lzstack_allocator_ *allocator);
static void _lzstack_dealloc_(void *ptr, int dynamic, struct _lzstack_allocator_ *allocator);

// private implementation
static void *_lzstack_alloc_(size_t bytes, int dynamic, struct _lzstack_allocator_ *allocator)
{
    return allocator ? allocator->lzstack_alloc(bytes, dynamic) : malloc(bytes);
}

static void _lzstack_dealloc_(void *ptr, int dynamic, struct _lzstack_allocator_ *allocator)
{
    if (!ptr)
        return;

    if (allocator)
        allocator->lzstack_dealloc(ptr, dynamic);
    else
        free(ptr);
}

// public implementation
struct _lzstack_ *lzstack_create(struct _lzstack_allocator_ *allocator)
{
    struct _lzstack_ *stack = (struct _lzstack_ *)_lzstack_alloc_(sizeof(struct _lzstack_), 0, allocator);

    if (!stack)
        return NULL;

    stack->nodes = NULL;
    stack->allocator = allocator;

    return stack;
}

void lzstack_destroy(struct _lzstack_ *stack)
{
    if (!stack)
        return;

    struct _lzstack_node_ *node = stack->nodes;

    while (node)
    {
        struct _lzstack_node_ *previous = node->previous;

        node->value = NULL;
        node->previous = NULL;

        _lzstack_dealloc_(node, 0, stack->allocator);

        node = previous;
    }

    struct _lzstack_allocator_ *allocator = stack->allocator;

    _lzstack_dealloc_(stack, 0, allocator);
}

void *lzstack_peek(struct _lzstack_ *stack, struct _lzstack_node_ **out_node)
{
    struct _lzstack_node_ *node = stack->nodes;

    if (!node)
        return NULL;

    if (out_node)
        *out_node = node;

    return node->value;
}

int lzstack_push(void *value, struct _lzstack_ *stack, struct _lzstack_node_ **out_node)
{
    struct _lzstack_node_ *node = (struct _lzstack_node_ *)_lzstack_alloc_(sizeof(struct _lzstack_node_), 0, stack->allocator);

    if (!node)
        return 1;

    node->value = value;
    node->previous = stack->nodes;

    stack->nodes = node;

    if (out_node)
        *out_node = node;

    return 0;
}

void *lzstack_pop(struct _lzstack_ *stack)
{
    struct _lzstack_node_ *node = stack->nodes;

    if (!node)
        return NULL;

    stack->nodes = node->previous;

    void *value = node->value;

    node->value = NULL;
    node->previous = NULL;

    _lzstack_dealloc_(node, 0, stack->allocator);

    return value;
}

int lzstack_count(struct _lzstack_ *stack)
{
    int count = 0;

    struct _lzstack_node_ *node = stack->nodes;

    while (node)
    {
        struct _lzstack_node_ *previous = node->previous;

        count++;

        node = previous;
    }

    return count;
}