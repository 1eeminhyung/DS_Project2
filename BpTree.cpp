#include "BpTree.h"

BpTree::~BpTree() {
	//Release the memory in the bpTree.
	//if the root has no child

	if(root == nullptr) return;
 	if(root->getMostLeftChild() == nullptr) {
		for(auto iter = root->getDataMap()->begin(); iter != root->getDataMap()->end(); iter++) {
			delete iter->second;
		}
		root->getDataMap()->clear();
		delete root;
		root = nullptr;
		return;
	}
	BpTreeNode* cur = root;
	while(cur->getMostLeftChild() != nullptr) {
		cur = cur->getMostLeftChild();
	}
	//first delete dataNode
	while(cur) {
		for(auto iter = cur->getDataMap()->begin(); iter != cur->getDataMap()->end(); iter++) {
			//delete FrequentPatternNode
			delete iter->second;
		}
		//delete dataNode
		BpTreeNode* deletenode = cur;

		//disconnecting parents from their children
		if(cur->getParent()->getMostLeftChild() != nullptr) {
			cur->getParent()->setMostLeftChild(nullptr);
			for(auto iter = cur->getParent()->getIndexMap()->begin(); iter != cur->getParent()->getIndexMap()->end(); iter++) {
				iter->second = nullptr;
			}
		}
		cur = cur->getNext();
		deletenode->getDataMap()->clear();
		delete deletenode;
	}

	cur = root;
	stack<BpTreeNode*> s;
	stack<BpTreeNode*> del;
	s.push(cur);

	while(!s.empty()) {
		BpTreeNode* temp = s.top();
		if(temp == nullptr) break;
		s.pop();
		del.push(temp);

		if(temp->getMostLeftChild()) {
			s.push(temp);
		}
		for(auto iter = temp->getIndexMap()->begin(); iter != temp->getIndexMap()->end(); iter++) {
			s.push(iter->second);
		}
	}

	while(!del.empty()) {
		BpTreeNode* deletenode = del.top();
		del.pop();
		deletenode->getIndexMap()->clear();
		delete deletenode;
	}
}


bool BpTree::Insert(int key, set<string> set) {	
	//If tree is empty, declare root
	if(root ==  nullptr) {
		root = new BpTreeDataNode;
		FrequentPatternNode* pN = new FrequentPatternNode;
		pN->InsertList(set);
		pN->setFrequency(key);
		root->insertDataMap(key, pN);
		return true;
	}

	//Move to the dataNode to be inserted.
	/*While circulating the index map of the root, 
	if the key value to be inserted is greater than the key value to be inserted in the mapIndex of the index node, 
	it goes down to the child node.*/
	BpTreeNode* p = root;
	while(p->getMostLeftChild() != nullptr) {
		auto iter = p->getIndexMap()->begin();
		for(int i=0; i<p->getIndexMap()->size(); i++, iter++) {
			if(key < iter->first) {
				//Move to their child.
				if(iter != p->getIndexMap()->begin()) {
					iter--;
					p = iter->second;
					break;
				}
				else {
					p = p->getMostLeftChild();
					break;
				}
			}
			/*If a key value larger than the existing key value of the index node comes in, 
			it is sent to the bottom right.*/
			else if(iter == --(p->getIndexMap()->end())) {
				p = iter->second;
				break;
			}
		}
	}

	/* Check if the Datanode you want to insert 
	has a Datanode with the Datanode you want to insert.*/
	auto it = p->getDataMap()->find(key);
	if(it != p->getDataMap()->end()) {
		it->second->InsertList(set);
	}
	else {
		FrequentPatternNode* pN = new FrequentPatternNode;
		pN->InsertList(set);
		pN->setFrequency(key);
		p->insertDataMap(key, pN);
	}

	if(excessDataNode(p)) {
		splitDataNode(p);
		return true;
	}
	return true;
}

BpTreeNode* BpTree::searchDataNode(int n) {
	/* The int-type variable is entered as a factor, 
	moved to the data node where the key is located, and then the node is returned.*/
	BpTreeNode* p = root;
	while(p->getMostLeftChild() != nullptr) {
		auto iter = p->getIndexMap()->begin();
		for(int i=0; i<p->getIndexMap()->size(); i++, iter++) {
			if(n < iter->first) {
				//Move to their child.
				if(iter != p->getIndexMap()->begin()) {
					iter--;
					p = iter->second;
					break;
				}
				else {
					p = p->getMostLeftChild();
					break;
				}
			}
			/*If a key value larger than the existing key value of the index node comes in, 
			it is sent to the bottom right.*/
			else if(iter == --(p->getIndexMap()->end())) {
				p = iter->second;
				break;
			}
		}
	}
	return p;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	/* Since the maximum number of elements in the B+tree with bpOrder m is m-1, 
	split is performed if the number of elements exceeds m-1.*/

	int n = (pDataNode->getDataMap()->size()) / 2;
	int gotoParentnodekey = 0;	
	BpTreeNode* leftSplit = new BpTreeDataNode;

	//if splitDataNode is root node 	
	if(pDataNode->getParent() == nullptr) {
		BpTreeNode* newRoot = new BpTreeIndexNode;
		//Split pDataNode
		/* pDatanode is a datanode that is split to the right, 
		including a value to go up to the new root node,
		and leftsplit is a datanode that is split to the left.*/
		for(int i=0; i<n+1; i++) {
			auto iter = pDataNode->getDataMap()->begin();
			if(i<n) {
				leftSplit->insertDataMap(iter->first, iter->second);
				pDataNode->deleteMap(iter->first);
			}
			else if(i==n) {
				gotoParentnodekey = iter->first;
			}
		}

		/* Connect the child and the parent, 
		and connect the node split to the left and the node split to the right. */
		newRoot->insertIndexMap(gotoParentnodekey, pDataNode);
		newRoot->setMostLeftChild(leftSplit);
		leftSplit->setNext(pDataNode);
		pDataNode->setPrev(leftSplit);
		pDataNode->setParent(newRoot);
		leftSplit->setParent(newRoot);
		
		//Update root
		root = newRoot;
		return;
	}


	BpTreeNode* pData = pDataNode->getParent();	
	/* pDatanode is a datanode that is split to the right, 
	including a value to go up to the pDatanode's parent node,
	and leftsplit is a datanode that is split to the left.*/
	for(int i=0; i<n+1; i++) {	
		auto iter = pDataNode->getDataMap()->begin();
		if(i<n) {
			leftSplit->insertDataMap(iter->first, iter->second);
			pDataNode->deleteMap(iter->first);
		}
		else if (i==n) {
			gotoParentnodekey = iter->first;
		}
	}

	//Insert the gotoparentnodekey value and pDataanode into the indexmap of pData.
	pData->insertIndexMap(gotoParentnodekey, pDataNode);
	auto it = pData->getIndexMap()->find(gotoParentnodekey);
	//it variable returns the position inserted in pData.
	//Connect the pIndex node and the left split.
	if(it == pData->getIndexMap()->begin()) {		
		pData->setMostLeftChild(leftSplit);
	}
	else {
		(--it)->second = leftSplit;
	}

	//Connect the leftsplit node to the node that was connected to the pDatanode.
	if(pDataNode->getPrev() != nullptr) {
		pDataNode->getPrev()->setNext(leftSplit);
		leftSplit->setPrev(pDataNode->getPrev());
	}

	/* Connect the child and the parent, 
	and connect the node split to the left and the node split to the right. */
	leftSplit->setNext(pDataNode);
	pDataNode->setPrev(leftSplit);
	leftSplit->setParent(pData);
	pDataNode->setParent(pData);
	
	//if the number of elements of the inserted pData is greater than bpOrder-1, the pData is split again.
	if(excessIndexNode(pData)) {
		splitIndexNode(pData);
		return;
	}
	return;
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	/*The index node to be split is implemented separately 
	when it is the root node and when it is not*/

	BpTreeNode* leftSplit = new BpTreeIndexNode;	
	BpTreeNode* splitchildren;
	int n = (pIndexNode->getIndexMap()->size()) / 2;
	int gotoParentnodekey = 0;
	//when pIndexNode is root node
	if(pIndexNode->getParent() == nullptr) {	
		BpTreeNode* newRoot = new BpTreeIndexNode;

		//Connect mostleftChild of leftSplit.
		leftSplit->setMostLeftChild(pIndexNode->getMostLeftChild());
		pIndexNode->getMostLeftChild()->setParent(leftSplit);

		/*LeftSplit is a node that splits pIndexNode to the left, 
		and pIndexNode is a node that splits to the right except for the node 
		that goes to the newroot node using erase on the map.*/
		for(int i=0; i<n+1; i++) {
			auto iter = pIndexNode->getIndexMap()->begin();
			if(i<n) {
				leftSplit->insertIndexMap(iter->first, iter->second);
				pIndexNode->deleteMap(iter->first);
				iter->second->setParent(leftSplit);
			}
			else if(i==n) {
				gotoParentnodekey = iter->first;
				splitchildren = iter->second;
				pIndexNode->deleteMap(iter->first);
			}
		}

		//Connect newRoot and child nodes and link leftSplit and pDatanode in both directions
		newRoot->insertIndexMap(gotoParentnodekey, pIndexNode);			
		newRoot->setMostLeftChild(leftSplit);
		pIndexNode->setMostLeftChild(splitchildren);
		splitchildren->setParent(pIndexNode);

		//Setparent leftSplit & pIndexNode
		leftSplit->setParent(newRoot);
		pIndexNode->setParent(newRoot);
		root = newRoot;
		return;
	}

	//when pIndexNode is not root node	
	BpTreeNode* pIndex = pIndexNode->getParent();	

	//Connect mostleftChild of leftSplit.
	leftSplit->setMostLeftChild(pIndexNode->getMostLeftChild());
	pIndexNode->getMostLeftChild()->setParent(leftSplit);

	/*LeftSplit is a node that splits pIndexNode to the left, 
	and pIndexNode is a node that splits to the right except for the node 
	that goes to the parent node using erase on the map.*/
	for(int i=0; i<n+1; i++) {
		auto iter = pIndexNode->getIndexMap()->begin();
		if(i<n) {
			leftSplit->insertIndexMap(iter->first, iter->second);
			iter->second->setParent(leftSplit);
			pIndexNode->deleteMap(iter->first);
		}
		else if(i==n) {
			gotoParentnodekey = iter->first;
			splitchildren = iter->second;
			pIndexNode->deleteMap(iter->first);
		}
	}

	//Insert the gotoparentnodekey value and pDataanode into the indexmap of pIndex.
	//pIndex is parent of pIndexNode
	pIndex->insertIndexMap(gotoParentnodekey, pIndexNode);
	auto it = pIndex->getIndexMap()->find(gotoParentnodekey);
	//it variable returns the position inserted in pData.
	//Connect the pIndex node and the left split.
	if(it == pIndex->getIndexMap()->begin()) {
		pIndex->setMostLeftChild(leftSplit);
	}
	else {
		(--it)->second = leftSplit;
	}
	
	/* Connect the child and the parent, 
	and connect the node split to the left and the node split to the right. */
	pIndexNode->setMostLeftChild(splitchildren);
	splitchildren->setParent(pIndexNode);
	leftSplit->setParent(pIndex);
	pIndexNode->setParent(pIndex);

	//if the number of elements of the inserted pIndex is greater than bpOrder-1, the pData is split again.
	if(excessIndexNode(pIndex)) {
		splitIndexNode(pIndex);
		return;
	}
	return;
}
bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) {
		//order is equal to the number of elements
		return true;
	}
	else {
		return false;
	}	

}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1) return true; //order is equal to the number of elements 
	else return false;
}


bool BpTree::printConfidence(string item, double item_frequency, double min_confidence) {
	/*If the tree does not have a Frequency Pattern to output,
	the Pattern value is returned to false.*/
	bool pattern = false;
	double frequency = item_frequency * min_confidence;
	/*The min_frequency is a frequency that
	 may have a confidence value equal to or greater than the input association rate.*/
	int min_frequency = (int)frequency;

	//p is a data node having a minimum frequency value.
	BpTreeNode* p = searchDataNode(min_frequency);
	*fout << "FrequentPattern Frequency Confidence" << endl;

	while(p) {
		//nodeiter is an iterator that traverses a data map of p.
		for(auto nodeiter = p->getDataMap()->begin(); nodeiter != p->getDataMap()->end(); nodeiter++) {
			FrequentPatternNode* node = nodeiter->second;
			auto list = node->getList();
			//listiter is an iterator that traverses mulitmap of node
			for(auto listiter = list.begin(); listiter != list.end(); listiter++) {
				set<string> s = listiter->second;
				//setiter is an iterator that traverses set of listiter->second
				for(auto setiter = s.begin(); setiter != s.end(); setiter++) {
					if(*setiter == item) {
						/*If you find the same pattern as the input item, 
						calculate the pattern confidence, and if it is larger than min_confidence, 
						output the pattern, confidence value, and pattern.*/
						double patternConfidence = nodeiter->first / item_frequency;
						if(patternConfidence >= min_confidence) {
							printFrequentPatterns(s, item);
							*fout << nodeiter->first << " ";
							*fout << patternConfidence << endl;
							pattern  = true;
						}
					}
				}			
			}
		}
		p = p->getNext();
	}
	return pattern;
}

bool BpTree::printFrequency(string item, int min_frequency) {
	/*If the tree does not have a Frequency Pattern to output,
	the Pattern value is returned to false.*/
	bool pattern = false;
	*fout << "FrequentPattern Frequency" << endl;

	//p is a data node having a minimum frequency value.
	BpTreeNode* p = searchDataNode(min_frequency);
	while(p) {
		//nodeiter is an iterator that traverses a data map of p.
		for(auto nodeiter = p->getDataMap()->begin(); nodeiter != p->getDataMap()->end(); nodeiter++) {
			FrequentPatternNode* node = nodeiter->second;
			auto list = node->getList();
			//listiter is an iterator that traverses mulitmap of node
			for(auto listiter = list.begin(); listiter != list.end(); listiter++) {
				set<string> s = listiter->second;
				//setiter is an iterator that traverses set of listiter->second
				for(auto setiter = s.begin(); setiter != s.end(); setiter++) {
					if(*(setiter) == item) {
						pattern = true;
						printFrequentPatterns(s, item);
						*fout << nodeiter->first << endl;
					}
				}
			}
		}
		p = p->getNext();
	}
	return pattern;
}

bool BpTree::printRange(string item, int min, int max) {
	/*If the tree does not have a Frequency Pattern to output,
	the Pattern value is returned to false.*/
	bool pattern = false;
	//p is a data node having a minimum frequency value.
	BpTreeNode* p = searchDataNode(min);
	*fout << "FrequentPattern Frequency" << endl;
	while(p) {

		for(auto nodeiter = p->getDataMap()->begin(); nodeiter != p->getDataMap()->end(); nodeiter++) {
			if(nodeiter->first > max) {
				return pattern;
			}
			FrequentPatternNode* node = nodeiter->second;
			auto list = node->getList();
			//listiter is an iterator that traverses mulitmap of node
			for(auto listiter = list.begin(); listiter != list.end(); listiter++) {
				set<string> s = listiter->second;
				//setiter is an iterator that traverses set of listiter->second
				for(auto setiter = s.begin(); setiter != s.end(); setiter++) {
					if(*(setiter) == item) {
						pattern = true;
						printFrequentPatterns(s, item);
						*fout << nodeiter->first << endl;
					}
				}
			}
		}
		p = p->getNext();
	}
	return pattern;
}

void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item) {
	/* This function is a function that receives a set factor and print the set.*/
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		*fout << temp;
		if (it == curPattern.end()) {
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}