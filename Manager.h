#ifndef MANAGER_H
#define MANAGER_H

#include "FPGrowth.h"
#include "BpTree.h"
#include <fstream>
#include <iostream>

class Manager
{
private:
	char* cmd;
	FPGrowth* fpgrowth;
	BpTree* bptree;
	ofstream flog;
public:
	Manager(int threshold, int bpOrder);	//constructor
	~Manager();								//destructor

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	bool PRINT_RANGE(string item, int start, int end);
	bool PRINT_CONFIDENCE(string item, double rate);
	bool PRINT_BPTREE(string item, int min_frequency);


	void printErrorCode(int n);
	void printSuccessCode();

};

#endif
