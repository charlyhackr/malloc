#include "malloc.h"

/**
 * allocate_memory - get bigger the heap memory
 * Return: the last pointer break
 */
void *allocate_memory()
{
	void *last_bp = NULL;

	/* Get bigger the heap memory*/
	last_bp = sbrk(M_CHUNK);
	/* if no able to get bigger the heap return NULL*/
	if (last_bp == VOID_P(-1))
		return (NULL);
	/* Clean the new memory of the heap*/
	memset(last_bp, 0, M_CHUNK);

	return (last_bp);
}

/**
 * get_user_ptr - get a free chunk to the user
 * @heap: free pointer of the heap
 * @mem_asked: size of memory requested by the user
 * Return: void pointer with the allocated memory requested by the user
 */
void *get_user_ptr(void *heap, size_t mem_asked)
{
	size_t remaining_mem = 0, c_mem = 0, pre_chunk_h = 0, cur_chunk_h = 0;
	void *u_mem = NULL, *neighbor = NULL, *new_neighbor = NULL;

	pre_chunk_h = GET_PREV_SIZE(heap), cur_chunk_h = GET_CURR_SIZE(heap);
	/* If there are a chunck freed before, use it*/
	if (!IS_LAST_HEADER(pre_chunk_h))
	{
		c_mem = cur_chunk_h - IS_PREV_FREE(cur_chunk_h);
		neighbor = CHAR_P(heap) + c_mem;
		if (mem_asked + HEADER_C + HEADER_S <= c_mem &&
				PADDING((c_mem - (mem_asked + HEADER_C))) == 0)
		{
			new_neighbor = CHAR_P(heap) + mem_asked;
			PREV_SIZE(neighbor) -= mem_asked, PREV_SIZE(new_neighbor) = 0;
			CURR_SIZE(new_neighbor) = c_mem - mem_asked;
			CURR_SIZE(heap) = mem_asked + IS_PREV_FREE(cur_chunk_h);
		}
		else
			PREV_SIZE(neighbor) -= c_mem, CURR_SIZE(neighbor) -= FLAG_FREE;
		u_mem = VOID_P((CHAR_P(heap) + HEADER_C));
		return (u_mem);
	}
	/* Check if there are enough heap memory for allocate the requested memory*/
	remaining_mem = cur_chunk_h - IS_PREV_FREE(cur_chunk_h);
	while (remaining_mem < mem_asked)
	{
		/* Try to get bigger the heap*/
		if (!allocate_memory())
			return (NULL);
		/* Add the new heap memory*/
		remaining_mem += M_CHUNK;
	}
	/* Get the ptr to retrieve the user and set the headers of the user chunks*/
	PREV_SIZE(heap) -= FLAG_LAST_HEADER;
	CURR_SIZE(heap) = mem_asked + IS_PREV_FREE(cur_chunk_h);
	u_mem = VOID_P((CHAR_P(heap) + HEADER_C));
	/* Set the end header of the chain allocated*/
	PREV_SIZE((CHAR_P(heap) + mem_asked)) = FLAG_LAST_HEADER;
	CURR_SIZE((CHAR_P(heap) + mem_asked)) = remaining_mem - mem_asked;
	return (u_mem);
}

/**
 * lookfor_free_memory - look for a freed chunk in the memory
 * @heap: first pointert of the heap
 * @mem_asked: size of memory requested by the user
 * Return: a void pointer to the free chunk found
 */
void *lookfor_free_memory(void *heap, size_t mem_asked)
{
	size_t pre_chunk_h = 0, cur_chunk_h = 0;
	void *pre_heap = NULL, *pre_pre_heap = NULL;

	/* Look for unallocate memory*/
	pre_chunk_h = GET_PREV_SIZE(heap), cur_chunk_h = GET_CURR_SIZE(heap);
	while (!IS_LAST_HEADER(pre_chunk_h))
	{
		/*If exist contiguos free chunks, merge them*/
		if (pre_heap && IS_PREV_FREE(GET_CURR_SIZE(pre_heap)) &&
				IS_PREV_FREE(cur_chunk_h))
		{
			pre_pre_heap = CHAR_P(pre_heap) - GET_PREV_SIZE(pre_heap);
			CURR_SIZE(pre_pre_heap) += pre_chunk_h;
			PREV_SIZE(heap) += GET_PREV_SIZE(pre_heap);
			heap = pre_pre_heap, cur_chunk_h = GET_CURR_SIZE(heap);
		}
		/* If find a freed chunk with enough memory, no get memory in the endchain*/
		else if (IS_PREV_FREE(cur_chunk_h) && mem_asked <= pre_chunk_h)
		{
			heap = CHAR_P(heap) - pre_chunk_h + IS_LAST_HEADER(pre_chunk_h);
			break;
		}
		/* If the prev chunk is free, so fix the chunk size to get the next chunk*/
		pre_heap = heap;
		heap = CHAR_P(heap) + cur_chunk_h - IS_PREV_FREE(cur_chunk_h);
		pre_chunk_h = GET_PREV_SIZE(heap), cur_chunk_h = GET_CURR_SIZE(heap);
	}
	/*If exist a freed chunk before the end allocate, update the end allocate*/
	if (IS_LAST_HEADER(pre_chunk_h) && IS_PREV_FREE(cur_chunk_h))
	{
		if (pre_heap && IS_PREV_FREE(GET_CURR_SIZE(pre_heap)))
		{
			pre_pre_heap = CHAR_P(pre_heap) - GET_PREV_SIZE(pre_heap);
			CURR_SIZE(pre_pre_heap) += pre_chunk_h - IS_LAST_HEADER(pre_chunk_h);
			PREV_SIZE(heap) += GET_PREV_SIZE(pre_heap), pre_heap = pre_pre_heap;
		}
		PREV_SIZE(pre_heap) += FLAG_LAST_HEADER;
		CURR_SIZE(pre_heap) += GET_CURR_SIZE(heap) - FLAG_FREE, heap = pre_heap;
	}
	return (heap);
}

/**
 * _malloc - malloc functio functionn
 * @size: number of bytes required to allocate
 * Return: a pointer to memory allocated on success, NULL otherwise
 */
void *_malloc(size_t size)
{
	static void *begin_h;
	void *u_mem = NULL, *heap = NULL;
	size_t mem_asked = 0;

	/* If size is 0, No allocate memory*/
	if (size == 0)
		return (NULL);
	/* Get the first pointer for the heap memory*/
	if (!begin_h)
	{
		begin_h = allocate_memory();
		/* If no pointer to heap memory return NULL*/
		if (!begin_h)
			return (NULL);
		/* Inicialize the final header with the remaining heap memory*/
		PREV_SIZE(begin_h) = 1, CURR_SIZE(begin_h) = M_CHUNK - HEADER_C;
	}
	/* Align the requested memory*/
	mem_asked = HEADER_C + size;
	mem_asked += PADDING(mem_asked);
	/* Look for the unallocate memory*/
	heap = lookfor_free_memory(begin_h, mem_asked);
	/* Allocate user requested memory*/
	u_mem = get_user_ptr(heap, mem_asked);

	return (u_mem);
}
