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

typedef struct BpqTree {
	BpqNode *pRoot;
} BpqTree;

BpqTree *bpq_tree_new(BpqNode *pRoot) {
	BpqTree *pResult = malloc(sizeof(BpqTree));
	pResult->pRoot = pRoot;
	return pResult;
}

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

BpqNode *bpq_find_min_prev(BpqNode *pNode) {
	BpqNode *pPrev = NULL;
	BpqNode *pMin = pNode;

	BpqNode *pPrevI = NULL;
	BpqNode *pI = pNode->pSibling;
	while(pI != NULL) {
		if(pI->key < pMin->key) {
			pMin = pI;
			pPrev = pPrevI;
		}

		pPrevI = pI;
		pI = pI->pSibling;
	}

	return pPrev;
}

void bpq_remove_child(BpqNode *pParent) {
	pParent->pChild = pParent->pChild->pSibling;
}

void bpq_remove_next(BpqNode *pNode) {
	if(pNode->pParent &&
	   pNode->pParent->pChild == pNode) {
		bpq_remove_child(pNode);	
	} else {
		pNode->pSibling = pNode->pSibling->pSibling;
	}
}

BpqNode *bpq_reverse_children_list(BpqNode *pRoot) {
	BpqNode *pI = pRoot->pChild;
	BpqNode *pPrev = NULL;

	while(pI != NULL) {
		BpqNode *pNext = pI->pChild;
		pI->pParent = pI->pChild;
		pI->pChild = pPrev;

		pPrev = pI;
		pI = pNext;
	}

	return pPrev ? pPrev : pRoot->pChild;
}

BpqNode *bpq_extract_min(BpqTree *pTree) {
	BpqNode *pMinPrev = bpq_find_min_prev(pTree->pRoot);
	BpqNode *pMin = NULL;

	if(pMinPrev) {
		pMin = pMinPrev->pSibling;
		bpq_remove_next(pMinPrev);
	} else {
		pMin = pTree->pRoot;
		pTree->pRoot = pTree->pRoot->pSibling;
	}

	BpqTree *pH = bpq_tree_new(bpq_reverse_children_list(pMin));

	pTree->pRoot = bpq_merge(pTree->pRoot, pH->pRoot);

	return pMinPrev->pSibling;
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

	assert(pA->pChild == pD &&
		   pD->pSibling == pB &&
		   pD->pChild == pE);

	return 0;
}

int main(int argc, char **pArgs) {
	merge_tests();

	return 0;
}
