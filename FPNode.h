#ifndef FPNODE_H
#define FPNODE_H

#include <cstring>
#include <string>
#include <map>
#include <cmath>

using namespace std;

class FPNode
{
private:
	int frequency;
	FPNode* parent;
	FPNode* next;
	map<string, FPNode*> children;
public:
	FPNode();
	~FPNode();
	void setParent(FPNode* node) { this->parent = node; }
	void setNext(FPNode* node) { this->next = node; }
	void pushchildren(string item, FPNode* node) { children.insert(map<string, FPNode*>::value_type(item, node)); }
	void updateFrequency(int frequency) { this->frequency += frequency; }

	int getFrequency() { return this->frequency; }
	FPNode* getParent() { return this->parent; }
	FPNode* getNext() { return this->next; }
	FPNode* getChildrenNode(string item);
	string getitem(FPNode* node);
	map<string, FPNode*> getChildren() { return children; }
};


#endif
