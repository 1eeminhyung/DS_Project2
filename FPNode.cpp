#include "FPNode.h"

FPNode::FPNode()	//constructor
{
	parent = nullptr;
	next = nullptr;
	frequency = 0;
}


FPNode::~FPNode()//destructor
{
	this->children.clear();
}

FPNode* FPNode::getChildrenNode(string item) {
	map<string, FPNode*>::iterator it = children.find(item);
	if (it == children.end()) return nullptr;	//cannot find childernNode

	//find childrenNode
	FPNode* findNode = it->second;
	return findNode;
}

string FPNode::getitem(FPNode* node) {
	/*Returns an item equal to the node value by traversing children.
	Return "" if the corresponding item was not found. */
	string item = "";
	for(auto iter = children.begin(); iter != children.end(); iter++) {
		if(iter->second == node) {
			item = iter->first;
			return item;
		}
	}
	return item;
}