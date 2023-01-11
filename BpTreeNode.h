#ifndef BPTREENODE_H
#define BPTREENODE_H

#include "FrequentPatternNode.h"
#include "FPNode.h"

class BpTreeNode
{
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;
public:
	BpTreeNode() {
		pParent = nullptr;
		pMostLeftChild = nullptr;
	}
	~BpTreeNode() {
		
	}

	void setMostLeftChild(BpTreeNode* pN) { this->pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN) { this->pParent = pN; }

	BpTreeNode* getParent() { return this->pParent; }
	BpTreeNode* getMostLeftChild() { return this->pMostLeftChild; }

	virtual void setNext(BpTreeNode* pN) {}
	virtual void setPrev(BpTreeNode* pN) {}
	virtual BpTreeNode* getNext() { return NULL; }
	virtual BpTreeNode* getPrev() { return NULL; }

	virtual void insertDataMap(int n, FrequentPatternNode* pN) {}
	virtual void insertIndexMap(int n, BpTreeNode* pN) {}
	virtual void deleteMap(int n) {}

	virtual map<int, BpTreeNode*>* getIndexMap() { return {}; }
	virtual map<int, FrequentPatternNode*>* getDataMap() { return {}; }

};

#endif
