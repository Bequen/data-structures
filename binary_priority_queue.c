
#include <limits.h>
#include <math.h>
#include <stdio.h>

typedef int Key;
typedef unsigned int Idx;

#define LOWEST_KEY INT_MIN

/* Swaps VALUES!!!! */
#define SWAP(a, b) ({int tmp = a; a = b; b = tmp;})

typedef struct BinaryPriorityQueue {
	unsigned int heapSize;
	unsigned int allocSize;
	Key *pData;
} BinaryPriorityQueue;

Idx get_parent(Idx idx) {
	return ceil(idx / 2.0) - 1;
}

Idx get_left(Idx idx) {
	return 2 * idx + 1;
}

Idx get_right(Idx idx) {
	return 2 * idx + 2;
}

/* Incrementuje klic na indexu */
void heap_inc_key(BinaryPriorityQueue *pQueue, unsigned int idx, Key key) {
	pQueue->pData[idx] = key;
	while(idx > 0 && pQueue->pData[get_parent(idx)] < pQueue->pData[idx]) {
		Idx parent = get_parent(idx);
		Key tmp = pQueue->pData[idx];
		pQueue->pData[idx] = pQueue->pData[parent];
		pQueue->pData[parent] = tmp;

		idx = parent;
	}
}

void heap_insert(BinaryPriorityQueue *pQueue, Key key) {
	if(pQueue->allocSize <= pQueue->heapSize + 1) {
		pQueue->pData = realloc(pQueue->pData, sizeof(Key) * (pQueue->allocSize ? pQueue->allocSize : 1) * 2);
		pQueue->allocSize = pQueue->allocSize ? pQueue->allocSize : 1;
	}
	pQueue->pData[pQueue->heapSize++] = LOWEST_KEY;
	heap_inc_key(pQueue, pQueue->heapSize -1, key);
}

void max_heapify(BinaryPriorityQueue *pQueue, Idx idx) {
	Idx left = get_left(idx);
	Idx right = get_right(idx);

	Idx largest = idx;

	if(left < pQueue->heapSize && 
	   pQueue->pData[left] > pQueue->pData[largest]) {
		largest = left;
	}

	if(pQueue->pData[right] > pQueue->pData[largest]) {
		largest = right;
	}

	/* if something changed */
	if(largest != idx) {
		SWAP(pQueue->pData[idx], pQueue->pData[largest]);
		max_heapify(pQueue, largest);
	}
}

Key heap_remove_max(BinaryPriorityQueue *pQueue) {
	if(pQueue->heapSize < 1) {
		return -1;
	}

	Key max = pQueue->pData[0];
	pQueue->pData[0] = pQueue->pData[pQueue->heapSize-1];
	pQueue->heapSize--;
	max_heapify(pQueue, 0);
	return max;
}

void heap_remove_min(BinaryPriorityQueue *pQueue) {
	
}

void heap_print(BinaryPriorityQueue *pQueue) {
	for(int i = 0; i < pQueue->heapSize; i++) {
		printf("%i ", pQueue->pData[i]);
	}
	printf("\n");
}

int
main() {
	BinaryPriorityQueue queue = {
		.heapSize = 1,
		.allocSize = 128,
		.pData = (Key*)malloc(sizeof(Key) * 128)
	};

	queue.pData[0] = 2;
	heap_insert(&queue, 3);
	heap_insert(&queue, 5);
	heap_insert(&queue, 4);
	heap_insert(&queue, 6);

	heap_print(&queue);
	
	printf("Max: %i\n", heap_remove_max(&queue));

	heap_print(&queue);

	return 0;
}
