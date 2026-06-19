#include "codexion.h"

int	main(void)
{
	t_heap	heap;
	t_coder	c1, c2, c3, c4;
	t_coder	*out;

	// 1. Inicializamos el heap a mano
	heap.capacity = 10;
	heap.size = 0;
	heap.data = malloc(sizeof(t_coder *) * heap.capacity);

	// 2. Creamos 4 programadores con deadlines completamente desordenados
	c1.id = 1; c1.deadline = 500;
	c2.id = 2; c2.deadline = 100; // ¡Este debería salir el primero!
	c3.id = 3; c3.deadline = 300;
	c4.id = 4; c4.deadline = 50;  // Ah no, ¡este es el más urgente!

	printf("--- Insertando en el Heap (Modo EDF) ---\n");
	heap_push(&heap, &c1, "edf");
	heap_push(&heap, &c2, "edf");
	heap_push(&heap, &c3, "edf");
	heap_push(&heap, &c4, "edf");

	printf("--- Extrayendo del Heap ---\n");
	while (heap.size > 0)
	{
		out = heap_pop(&heap, "edf");
		printf("Coder ID: %d | Deadline: %ld\n", out->id, out->deadline);
	}

	free(heap.data);
	return (0);
}