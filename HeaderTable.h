#ifndef HEADERYABLE_H
#define HEADERYABLE_H

#include "FPNode.h"
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

class HeaderTable
{
private:
	list<pair<int, string>> indexTable;
	map<string, FPNode*> dataTable;
	ofstream* fout;
public:
	HeaderTable() { }
	~HeaderTable();

	void insertTable(string item, int frequency);
	list<pair<int, string>> getindexTable() { return indexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }

	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); }
	void ascendingIndexTable() { indexTable.sort(); }
	int find_frequency(string item);
	void PrintIndexTable();
};


#endif

