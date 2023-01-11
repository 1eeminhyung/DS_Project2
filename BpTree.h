#ifndef BPTREE_H
#define BPTREE_H

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>

class BpTree {
private:
	BpTreeNode* root;
	int			order;		// m children
	ofstream* fout;
public:
	BpTree(ofstream *fout, int order = 3) {
		root = nullptr;
		this->order = order;
		this->fout = fout;
	}
	~BpTree();
	// essential
	
	bool		Insert(int key, set<string> set);
	bool		excessDataNode(BpTreeNode* pDataNode);
	bool		excessIndexNode(BpTreeNode* pIndexNode);
	void		splitDataNode(BpTreeNode* pDataNode);
	void		splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return this->root; }
	BpTreeNode* searchDataNode(int n);

	void	printFrequentPatterns(set<string> pFrequentPattern, string item);
	bool	printFrequency(string item, int min_frequency);
	bool	printConfidence(string item, double item_frequency, double min_confidence);
	bool 	printRange(string item, int min, int max);
};

#endif