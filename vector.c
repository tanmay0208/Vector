#include<stdio.h>

struct vector {
void *base; /**< Raw memory for items */
size_t size; /**< The number of inserted items */
size_t capacity; /**< The number of potential items before a resize */
size_t item_size; /**< The number of bytes occupied by an item */
} vector;

typedef struct vector;

extern vector *vector_create(size_t item_size, size_t capacity);
extern size_t vector_size(vector *v);
extern int vector_insert(vector *v, void *item, size_t index);
extern int vector_remove(vector *v, size_t index);


extern vector *vector_create(size_t item_size, size_t capacity)
{
  vector *v = malloc(sizeof *v);
  if (v != NULL) {
    v->base = NULL;
    v->size = 0;
    v->capacity = capacity;
    v->item_size = item_size;
    if (capacity > 0) {
      /* Allocate the default capacity */
      v->base = malloc(capacity * item_size);
      if (v->base == NULL) {
        /* Clean up rather than leaving a zombie */
        free(v);
        v = NULL;
      }
    }
  }
  return v;
}

extern size_t vector_size(vector *v)
{
  return v->size;
}


extern int vector_insert(vector *v, void *item, size_t index)
{
  void *src, *dst;
  if (index > v->size)
    return 0;
  if (v->size == v->capacity) {
    /* Resize to the next auto-growth amount */
    size_t new_capacity;
    if (!auto_capacity(v, &new_capacity) ||
      !vector_resize(v, new_capacity)){
	return 0;
    }
    v->capacity = new_capacity;
  }
  src = vector_at(v, index + 1);
  dst = vector_at(v, index);
  /* Make room for a new item */
  memmove(src, dst, (v->size - index) * v->item_size);
  /* Insert the new item */
  memcpy(dst, item, v->item_size);
  ++v->size;
  return 1;
}

extern int vector_remove(vector *v, size_t index)
{
  if (index >= v->size)
    return 0;
  else if (index == v->size - 1) {
    /* Special case: no copy when removing the last item */
    --v->size;
  }
  else {
    void *dst = vector_at(v, index);
    void *src = vector_at(v, index + 1);
    /* Fill in the vacated slot */
    memmove(dst, src, (v->size - index - 1) * v->item_size);
    --v->size;
  }
  return 1;
}


