#include "sc_alloc.h"

typedef struct Sc_Memory_Heap {
	uint8_t sc_g_buffers[SC_HEAP_MEMORY_SIZE];
	uint8_t sc_g_buffers_maps[SC_MEMORY_SIZE_];
	uint8_t sc_l_buffers[SC_HEAP_MEMORY_SIZE];

	uint8_t* sc_l_buffers_pos;
	size_t sc_l_buffers_alloc_num;
	uint8_t* sc_g_buffers_cache_pos;
	size_t sc_g_buffers_cache_size;
}Sc_Memory_Heap;

static Sc_Memory_Heap sc_mem_heap = { 0 };

void* sc_maps_to_minimum_alloc____(size_t size) {
	if (!sc_mem_heap.sc_l_buffers_pos)
		sc_mem_heap.sc_l_buffers_pos = &sc_mem_heap.sc_l_buffers[0];
	uint8_t* now = sc_mem_heap.sc_l_buffers_pos;
	size_t l = (size_t)&sc_mem_heap.sc_l_buffers[SC_HEAP_MEMORY_SIZE] - (size_t)now;
	if (l < SC_MEMORY_MINIMUM_MAPS_SIZE) {
		return NULL;
	}
	size += sizeof(void*);
	if (l < size) return NULL;
	uint8_t* e = &now[size - 1];

	memcpy(now, &e, sizeof(void*));
	sc_mem_heap.sc_l_buffers_pos = &now[size];
	sc_mem_heap.sc_l_buffers_alloc_num++;
	return now + sizeof(void*);
}

void* sc_minimum_alloc____(size_t size) {
	if (size > SC_MEMORY_MINIMUM_SIZE) return NULL;

	if (!sc_mem_heap.sc_l_buffers_pos)
		sc_mem_heap.sc_l_buffers_pos = &sc_mem_heap.sc_l_buffers[0];
	uint8_t* now = sc_mem_heap.sc_l_buffers_pos;
	size_t l = (size_t)&sc_mem_heap.sc_l_buffers[SC_HEAP_MEMORY_SIZE] - (size_t)now;
	size += sizeof(void*);
	if (l < size) return NULL;
	uint8_t* e = &now[size - 1];

	memcpy(now, &e, sizeof(void*));
	//printf("%p %p\n", now, now + sizeof(void*));

	sc_mem_heap.sc_l_buffers_pos = &now[size];
	sc_mem_heap.sc_l_buffers_alloc_num++;
	return now + sizeof(void*);
}

void* sc_maps_alloc____(size_t size) {
	//void * buf = sc_minimum_alloc____(size);
	//if (buf) return buf;
	//if (!size) return NULL;

	size_t a = 0;
	uint8_t* first = NULL;

	size += sizeof(void*);
	size_t li = 0;
	size_t lj = 0;
	first = &sc_mem_heap.sc_g_buffers[0];
	if (sc_mem_heap.sc_g_buffers_cache_size >= size) {
		size_t f =
			sc_mem_heap.sc_g_buffers_cache_pos
			-
			&sc_mem_heap.sc_g_buffers[0];
		size_t ii = f / CHAR_BIT;
		size_t jj = f % CHAR_BIT;
		for (size_t i = ii; i < SC_MEMORY_SIZE_; ++i) {
			for (size_t j = jj; j < CHAR_BIT; ++j) {
				if (a == size)break;
				sc_mem_heap.sc_g_buffers_maps[i]
					|= (1 << j);
				a++;
			}
			if (a == size)break;

			jj = 0;
		}
		uint8_t* block = &sc_mem_heap.sc_g_buffers_cache_pos[size - 1];
		memcpy(sc_mem_heap.sc_g_buffers_cache_pos, &block, sizeof(void*));
		first = sc_mem_heap.sc_g_buffers_cache_pos;

		if (sc_mem_heap.sc_g_buffers_cache_size - size > CHAR_BIT) {
			sc_mem_heap.sc_g_buffers_cache_size = sc_mem_heap.sc_g_buffers_cache_size - size;
			sc_mem_heap.sc_g_buffers_cache_pos = &block[1];
		}
		else {
			sc_mem_heap.sc_g_buffers_cache_size = 0;
			sc_mem_heap.sc_g_buffers_cache_pos = NULL;
		}
		return first + sizeof(void*);
	}
	for (size_t i = 0; i < SC_MEMORY_SIZE_; ++i) {
		uint8_t byte_ = sc_mem_heap.sc_g_buffers_maps[i];
		//first = NULL;
		/*
		* 0000 0000
		*/

		for (size_t j = 0; j < CHAR_BIT; ++j) {
			if (a == size) break;
			if (byte_ & (1 << j)) {
				if (sc_mem_heap.sc_g_buffers_cache_size < a) {
					sc_mem_heap.sc_g_buffers_cache_size = a;
					sc_mem_heap.sc_g_buffers_cache_pos = first;
				}
				a = 0;
				//li = i;
				//lj = j;
				size_t ii = i;
				size_t jj = j + 1;
				if (jj >= CHAR_BIT) {
					ii++;
					if (ii >= SC_MEMORY_SIZE_) return NULL;
					jj = 0;
				}
				first = &sc_mem_heap.sc_g_buffers[ii * CHAR_BIT + jj];
				li = ii;
				lj = jj;
				continue;
			}
			a++;
		}
		if (a == size) break;
	}
	if (a != size) {
		return sc_maps_to_minimum_alloc____(size - sizeof(void*));
	}
	if (!first) return NULL;

	uint8_t* block = &first[size - 1];
	/*
	* [end adress] [alloc]
	*/
	size_t b = 0;
	for (size_t i = li; i < SC_MEMORY_SIZE_; ++i) {
		for (size_t j = lj; j < CHAR_BIT; ++j) {
			if (b == size)break;
			sc_mem_heap.sc_g_buffers_maps[i] |= 1 << j;
			b++;
		}
		lj = 0;
		if (b == size)break;
	}
	memcpy(first, &block, sizeof(void*));

	return first + sizeof(void*);
}

void* sc_alloc____(size_t size) {
	if (!size) return NULL;
	void* buf = sc_minimum_alloc____(size);
	if (!buf) buf = sc_maps_alloc____(size);
	return buf;
}

void* sc_free____(void* block) {
	if (block) {
		uint8_t* buf = block;
		uint8_t* first_pos = buf - sizeof(void*);
		uint8_t* end_pos;
		memcpy(&end_pos, first_pos, sizeof(void*));
		if (first_pos <= &sc_mem_heap.sc_g_buffers[SC_HEAP_MEMORY_SIZE - 1]) {
			uint8_t* t = &sc_mem_heap.sc_g_buffers[0];
			size_t f = (size_t)first_pos - (size_t)t;
			size_t e = (size_t)end_pos - (size_t)first_pos + 1;
			size_t ii = f / CHAR_BIT;
			size_t jj = f % CHAR_BIT;
			size_t idx = 0;
			for (uint8_t* s = first_pos; s != end_pos; ++s) *s = 0;
			*end_pos = 0;
			/*
			* 0110 1010
			*
			*/
			for (size_t i = ii; i < SC_MEMORY_SIZE_; ++i) {
				for (size_t j = jj; j < CHAR_BIT; ++j) {
					if (e == idx) break;
					sc_mem_heap.sc_g_buffers_maps[i] =
						sc_mem_heap.sc_g_buffers_maps[i]
						& ~(1 << j)
						;
					idx++;
				}
				if (e == idx) break;
				jj = 0;
			}
			if (sc_mem_heap.sc_g_buffers_cache_size < e) {
				sc_mem_heap.sc_g_buffers_cache_size = e;
				sc_mem_heap.sc_g_buffers_cache_pos = first_pos;
			}
		}
		else {
			for (uint8_t* s = first_pos; s != end_pos; ++s) *s = 0;

			*end_pos = 0;
			if (sc_mem_heap.sc_l_buffers_alloc_num) {
				sc_mem_heap.sc_l_buffers_alloc_num--;
			}
			if (!sc_mem_heap.sc_l_buffers_alloc_num) {
				sc_mem_heap.sc_l_buffers_pos = &sc_mem_heap.sc_l_buffers[0];
			}
		}
	}
	return NULL;
}