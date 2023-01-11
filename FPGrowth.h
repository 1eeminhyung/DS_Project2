#ifndef FPGROWTH_H
#define FPGROWTH_H


#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

class FPGrowth
{
private:
	int threshold;
	FPNode* fpTree;
	HeaderTable* table;
	map<set<string>, int> frequenctPatterns;
	ofstream* fout;
public:
	FPGrowth(ofstream* fout, int threshold = 3) {	//ofstream* fout
		this->threshold = threshold;
		fpTree = new FPNode;  
		table = new HeaderTable;
		this->fout = fout;
	}
	~FPGrowth();
	
	void createTable(string item, int frequency) { this->table->insertTable(item, frequency); }
	void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array);
	void connectNode(HeaderTable* table, string item, FPNode* node);
	
	void frequenctPatternSetting() {
		table->ascendingIndexTable();	//sort ascending
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
	bool contains_single_path(FPNode* pNode);
	int item_frequency(string item) { return this->table->find_frequency(item); }
	FPNode* getTree() { return this->fpTree; }
	HeaderTable* getHeaderTable() { return this->table; }

	bool printList();
	bool printTree();
	void saveFrequentPatterns();
};


#endif