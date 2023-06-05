
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct FibNode {
	int key;
	int degree;
	int mark;
	struct FibNode *pParent, *pChild;
	struct FibNode *pLeft, *pRight;
} FibNode;

typedef struct FibHeap {
	FibNode *pMin;
	int n;
} FibHeap;

FibNode *fib_new_node(int key) {
	FibNode *pResult = malloc(sizeof(FibNode));
	pResult->key = key;
	pResult->pLeft = pResult->pRight = pResult;
	pResult->pParent = pResult->pChild = NULL;
	pResult->mark = pResult->degree = 0;
	return pResult;
}
// 1 5 6
// 3 5 4

FibNode *fib_join(FibNode *pA, FibNode *pB) {
	pB->pLeft->pRight = pA ? pA->pRight : pB;
	if(pA) {
		pA->pRight->pLeft = pB->pLeft;
		pA->pRight = pB;
	}
	pB->pLeft = pA ? pA : pB;

	return pA;
}

FibHeap* fib_insert(FibHeap* pHeap, FibNode *pNode) {
	/* concat */
	fib_join(pHeap->pMin, pNode);

	if(pHeap->pMin == NULL || pNode->key < pHeap->pMin->key) {
		pHeap->pMin = pNode;
	}

	pHeap->n++;

	return pHeap;
}

FibHeap *fib_new_heap() {
	FibHeap *pResult = malloc(sizeof(FibHeap));
	pResult->n = 0;
	pResult->pMin = NULL;
	return pResult;
}

FibHeap *fib_union(FibHeap *pA, FibHeap *pB) {
	FibHeap *pResult = fib_new_heap();

	pResult->pMin = pA->pMin;
	fib_join(pB->pMin, pResult->pMin);
	if(pA->pMin == NULL || 
	   (pB->pMin != NULL && pB->pMin->key < pA->pMin->key)) {
		pResult->pMin = pB->pMin;
	}

	pResult->n = pA->n + pB->n;

	return pResult;
}

#define SWAP(a, b) ({typeof(a) tmp = a; a = b; b = tmp;})

void fib_remove(FibNode *pNode) {
	pNode->pLeft->pRight = pNode->pRight;	
	pNode->pRight->pLeft = pNode->pLeft; 
}

void fib_remove_min(FibHeap *pHeap) {
	if(pHeap->pMin->pRight == pHeap->pMin) {
		pHeap->pMin = NULL;
	} else {
		FibNode *pNext = pHeap->pMin->pRight;
		fib_remove(pHeap->pMin);
		pHeap->pMin = pNext;
	}

	pHeap->n--;
}

int fib_max_degree(FibNode *pNode) {
	int max = pNode->degree;
	FibNode *pI = pNode->pRight;
	do {
		if(max < pI->degree) {
			max = pI->degree;
		}
		pI = pI->pRight;
	} while(pI != pNode);

	return max;
}

void fib_add_child(FibNode *pParent, FibNode *pChild) {
	pParent->pChild = pChild;
	pChild->pParent = pParent;
	pParent->degree++;
}

void fib_remove_siblings(FibNode *pNode) {
	pNode->pLeft = pNode->pRight = pNode;

}

void fib_heap_link(FibHeap *pHeap, FibNode *pX, FibNode *pY) {
	fib_remove(pY);

	fib_add_child(pX, pY);
	fib_remove_siblings(pX->pChild);
	
	pY->mark = 0;
}



void fib_consolidate(FibHeap *pHeap) {
	/* */
	size_t D = fib_max_degree(pHeap->pMin) + 1; 
	FibNode **A = malloc(sizeof(FibNode*) * D);
	memset(A, 0, sizeof(FibNode*) * D);

	FibNode *pI = pHeap->pMin;
	FibNode *pNext = pI->pRight;

	/* find 2 nodes with same degree but different key */
	do {
		FibNode *pX = pI;
		int d = pX->degree;

		/* if there is already a node with same degree */
		while(A[d] != NULL) {
			FibNode *pY = A[d];

			if(pX->key > pY->key) {
				SWAP(pX->key, pY->key);
			}

			/* Put 2 nodes with same degree as child and parent */
			fib_heap_link(pHeap, pX, pY);
			A[d++] = NULL;
		}

		A[d] = pX;
		pI = pNext;
		pNext = pI->pRight;
	} while(pI != pHeap->pMin);

	pHeap->pMin = NULL;

	for(int i = 0; i < D + 1; i++) {
		if(A[i] != NULL) {
			fib_insert(pHeap, A[i]);
		}
	}
}



FibNode* fib_extract_min(FibHeap *pHeap) {
	FibNode *pMin = pHeap->pMin;

	if(pMin != NULL) {
		FibNode *pI = pMin->pChild;

		/* Put every child of minimum into root list */
		while(pI != NULL) {
			fib_join(pHeap->pMin, pI);
			pHeap->n++;

			pI->pParent = NULL;
			pI = pI->pChild;
			pI->pParent->pChild = NULL;
		}

		/* remove the minimum node */
		FibNode *pNext = pMin->pRight;
		fib_remove_min(pHeap);

		if(pHeap->pMin != NULL){
			fib_consolidate(pHeap);
		} 

		pHeap->n--;
	}

	return pMin;
}

void print_list(FibNode *pNode) {
	FibNode *pI = pNode;
	do {
		printf("%i ", pI->key);
		if(pI->pRight->pLeft == pI)
			printf("<");
		printf("-> ");

		pI = pI->pRight;
	} while(pI != pNode);
	printf("\n");
}

int join_test() {
	FibNode *pA = fib_new_node(1);
	FibNode *pB = fib_new_node(4);

	print_list(fib_join(pA, pB));
	assert(pA != pB && pA->key != pB->key);
	assert(pA->pRight == pB && pB->pRight == pA &&
		   pA->pLeft == pB && pB->pLeft == pA);

	FibNode *pC = fib_new_node(6);
	print_list(fib_join(pA, pC));
	assert(pC->pRight == pB);

	return 0;
}

int heap_test() {
	FibHeap *pHeap = fib_new_heap();

	FibNode *pA = fib_new_node(1);
	FibNode *pB = fib_new_node(4);
	FibNode *pC = fib_new_node(6);

	assert(pHeap->pMin == NULL);

	fib_insert(pHeap, pA);
	assert(pHeap->pMin == pA && pHeap->pMin->pLeft == pHeap->pMin->pRight);

	fib_insert(pHeap, pB);
	fib_insert(pHeap, pC);

	print_list(pHeap->pMin);

	FibNode *pMin = fib_extract_min(pHeap);
	assert(pMin->key == 1);
	assert(pHeap->pMin->key == 4 &&
		   pHeap->pMin->pChild->key == 6);

	print_list(pHeap->pMin);

	return 0;
}

int tests() {
	int a = 5;
	int b = 3;
	SWAP(a, b);
	assert(a == 3 && b == 5);

	join_test();
	
	heap_test();

	return 0;
}

int main() {
	tests();

	return 0;
}
