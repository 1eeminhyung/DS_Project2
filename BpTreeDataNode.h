#ifndef BpTreeDataNode_H
#define BpTreeDataNode_H

#include "BpTreeNode.h"
#include <iostream>

class BpTreeDataNode : public BpTreeNode
{
private:
	map <int, FrequentPatternNode*> mapData;
	BpTreeNode* pNext;
	BpTreeNode* pPrev;
public:
	BpTreeDataNode() {
		pNext = nullptr;
		pPrev = nullptr;
	}
	~BpTreeDataNode() { }

	void setNext(BpTreeNode* pN) { this->pNext = pN; }
	void setPrev(BpTreeNode* pN) { this->pPrev = pN; }
	BpTreeNode* getNext() { return this->pNext; }
	BpTreeNode* getPrev() { return this->pPrev; }

	void insertDataMap(int n, FrequentPatternNode* pN) {
		mapData.insert(map<int, FrequentPatternNode*>::value_type(n, pN));
	}

	void deleteMap(int n) {
		mapData.erase(n);
	}
	map<int, FrequentPatternNode*>* getDataMap() { return &mapData; }
};

#endif
