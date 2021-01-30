#include "malloc.h"

/**
* naive_malloc -  basic malloc
* @size: number of bytes required to allocate
* Return: a pointer to memory allocated on success, NULL otherwise
*/
void *naive_malloc(size_t size)
{
	static void *begin_h;
	static size_t chunks;
	void *u_mem = NULL, *last_bp = NULL, *ptr = NULL;
	size_t remaining_mem = 0, i = 0, mem_asked = 0, padding = 0;

	if (size == 0)
		return (NULL);

	if (!begin_h)
	{
		last_bp = sbrk(M_CHUNK);
		if (last_bp == ((void *) -1))
			return (NULL);
		begin_h = last_bp;
		memset(last_bp, 0, M_CHUNK);
		*((size_t *) ((char *) begin_h)) = M_CHUNK - HEADER_S;
	}

	for (i = 0, ptr = begin_h; i < chunks; i++)
		ptr = ((char *) ptr) + *((size_t *) ((char *) ptr));

	remaining_mem = *((size_t *) ((char *) ptr));
	mem_asked = HEADER_S + size;
	padding = (HEADER_S - (mem_asked % HEADER_S)) % HEADER_S;
	mem_asked += padding;

	while (((int) remaining_mem) - ((int) mem_asked) < 0)
	{
		last_bp = sbrk(M_CHUNK);
		if (last_bp == ((void *) -1))
			return (NULL);
		memset(last_bp, 0, M_CHUNK);
		remaining_mem += M_CHUNK;
	}

	*((size_t *) ((char *) ptr)) = mem_asked;
	u_mem = (void *) (((char *) ptr) + HEADER_S);
	*((size_t *) (((char *) ptr) + mem_asked)) = remaining_mem - mem_asked;
	chunks++;
	return (u_mem);
}
