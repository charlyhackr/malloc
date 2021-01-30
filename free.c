#include "malloc.h"

/**
* _free - free a given heap pointer
* @ptr: pointer of the heap to be freed
* Return: Nothing
*/
void _free(void *ptr)
{
	void *header_ptr = NULL, *neighbor = NULL;
	size_t cur_chunk_h = 0;

	if (!ptr)
		return;

	/* Get the header ptr and the current size chunk*/
	header_ptr = CHAR_P(ptr) - HEADER_C;
	cur_chunk_h = GET_CURR_SIZE(header_ptr);
	/* Get the next chunk*/
	neighbor = CHAR_P(header_ptr) + cur_chunk_h - IS_PREV_FREE(cur_chunk_h);
	/* Check if the prev chunk was freed before*/
	if (IS_PREV_FREE(GET_CURR_SIZE(neighbor)))
		abort();
	/* Set the freed space in the next chunk*/
	((size_t *) neighbor)[0] += cur_chunk_h - IS_PREV_FREE(cur_chunk_h);
	((size_t *) neighbor)[1] += FLAG_FREE;
}
