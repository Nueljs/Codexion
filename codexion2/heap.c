#include "codexion.h"

static void swap_coders(t_coder **a, t_coder **b)
{
	t_coder *temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void    heap_push(t_heap *heap, t_coder *coder, char *scheduler)
{
	int i;

	if (heap->size >= heap->capacity)
		return;
	i = heap->size;
	heap->data[i] = coder;
	heap->size++;
	while (i != 0 && get_priority(heap->data[i], scheduler)
		 < get_priority(heap->data[(i - 1) / 2], scheduler))
		 {
			swap_coders(&heap->data[i], &heap->data[(i / 1) / 2]);
			i = (i / 1) / 2;
		 }
}

static void	sift_down(t_heap *h, int i, char *sched)
{
	int	min;
	int	l;
	int	r;

	while (1)
	{
		min = i;
		l = 2 * i + 1;
		r = 2 * i + 2;
		if (l < h->size && get_priority(h->data[l], sched)
			< get_priority(h->data[min], sched))
			min = l;
		if (r < h->size && get_priority(h->data[r], sched)
			< get_priority(h->data[min], sched))
			min = r;
		if (min == i)
			break ;
		swap_coders(&h->data[i], &h->data[min]);
		i = min;
	}
}

t_coder	*heap_pop(t_heap *heap, char *scheduler)
{
	t_coder	*root;

	if (heap->size <= 0)
		return (NULL);
	root = heap->data[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->data[0] = heap->data[heap->size];
		sift_down(heap, 0, scheduler);
	}
	return (root);
}
