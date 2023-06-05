#include <stdlib.h>

#define SWAP(a, b) {typeof(a) temp = a; a = b; b = temp;}

#define MIN_VAR(a, b, var) (a->var < b->var ? a : b)
#define MAX_VAR(a, b, var) (a->var > b->var ? a : b)

typedef struct BpqNode {
	int key;
	int degree;

	struct BpqNode *pChild;
	struct BpqNode *pSibling;
	struct BpqNode *pParent;
} BpqNode;

BpqNode *bpq_new(int key) {
	BpqNode *pResult = malloc(sizeof(BpqNode));
	pResult->key = key;
	pResult->degree = 0;
	pResult->pChild = pResult->pSibling = pResult->pParent = 0x0;

	return pResult;
}

/* Only merges two trees */
BpqNode* bpq_link(BpqNode *pA, BpqNode *pB) {
	pA->pParent = pB;
	pA->pSibling = pB->pChild;
	pB->pChild = pA;
	pB->degree++;

	return pB;
}

BpqNode *bpq_merge(BpqNode *pA, BpqNode *pB) {
	BpqNode *pStart = MIN_VAR(pA, pB, degree);
	BpqNode *pCurrent = pStart;

	if(pA->degree < pB->degree) {
		pCurrent = pA;
		pA = pA->pSibling;
	} else {
		pCurrent = pB;
		pB = pB->pSibling;
	}

	while(pA && pB) {
		if(pA->degree < pB->degree) {
			pCurrent->pSibling = pA;
			pA = pA->pSibling;
		} else {
			pCurrent->pSibling = pB;
			pB = pB->pSibling;
		}
	}

	pCurrent->pSibling = pA ? pA : pB;

	return pStart;
}

BpqNode *bpq_merge2(BpqNode *pA, BpqNode *pB) {
	BpqNode *pResult = NULL;
	BpqNode *pLast = NULL;

	while(pA && pB) {
		BpqNode *pNext = NULL;
		if(pA->degree > pB->degree) {
			pNext = pA;	
			pA = pA->pSibling;
		} else {
			pNext = pB;
			pB = pB->pSibling;
		}

		if(!pLast) {
			pLast = pNext;
			pResult = pNext;
		} else {
			pLast->pSibling = pNext;
		}
	}

	while(pA) {
		if(!pLast) {
			pLast = pA;
			pResult = pA;
		} else {
			pLast->pSibling = pA;
		}	
		pA = pA->pSibling;
	}

	while(pB) {
		if(!pLast) {
			pLast = pB;
			pResult = pB;
		} else {
			pLast->pSibling = pB;
		}	
		pB = pB->pSibling;
	}

	return pResult;
}


BpqNode* bpq_join(BpqNode *pA, BpqNode *pB) {
	/* makes new one */
	BpqNode *pNode = bpq_merge(pA, pB);
	
	if(pNode == NULL) return NULL;

	BpqNode *pPrev = NULL;
	BpqNode *pCurrent = pNode;
	BpqNode *pNext = pCurrent->pSibling;

	while(pNext) {
		/* move */
		if(pCurrent->degree != pNext->degree ||
		   (pNext->pSibling != NULL && pNext->pSibling->degree == pCurrent->degree)) {
			pPrev = pCurrent;
			pCurrent = pNext;
		} 
		
		else if(pCurrent->key <= pNext->key) {
			pCurrent->pSibling = pNext->pSibling;
			bpq_link(pNext, pCurrent);
		}

		else {
			if(pPrev == NULL) {
				pNode = pNext;
			} else {
                pPrev->pSibling = pNext;
            }
            bpq_link(pCurrent, pNext);
            pCurrent = pNext;
		}

		pNext = pCurrent->pSibling;
	}
	
	return pNode;
}

BpqNode * bpq_enqueue(BpqNode *pTree, BpqNode *pNode) {
	return bpq_join(pTree, pNode);
}

void bpq_find_min() {

}

void bpq_extract_min() {

}

#include <assert.h>

int merge_tests() {
	BpqNode *pA = bpq_new(15);
	BpqNode *pB = bpq_new(33);
	BpqNode *pC = bpq_join(pA, pB);

	BpqNode *pD = bpq_new(28);
	BpqNode *pE = bpq_new(41);
	BpqNode *pF = bpq_join(pD, pE);
	BpqNode *pG = bpq_join(pC, pF);
}

int main(int argc, char **pArgs) {

	merge_tests();

	return 0;
}
