#include "FPGrowth.h"

FPGrowth::~FPGrowth() {
	//Release the memory in the Fpgrowth.
	table->ascendingIndexTable();
	list<pair<int, string>> li = table->getindexTable();

	for(auto iter = li.begin(); iter != li.end(); iter++) {
		if(iter->first < threshold) {
			delete table->getNode(iter->second);
			continue;
		}
		else {
			FPNode* p = table->getNode(iter->second);
			while(p) {
				FPNode* deletenode = p;
				p = p->getNext();
				delete deletenode;
			}
		}
	}
	delete fpTree;
	delete table;
}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array) {

	table->descendingIndexTable();
	vector<pair<int, string>> ordered_item;
	int temp = 0;
	//declare ordered_item
	/*The ordered_item does not store a value smaller than the threshold in the item_array, 
	but if it is all stored, it is sorted in ascending order using the sort function.*/
	for(auto iter = item_array.begin(); iter != item_array.end(); iter++) {
		temp = table->find_frequency(*iter);
		//Exception handling when item does not exist in indexTable or value is less than threshold
		if(temp == 0) continue;			
		else if(temp < threshold) continue;
		else {
			ordered_item.push_back(make_pair(temp, *iter));
		}
	}
	sort(ordered_item.begin(), ordered_item.end(), greater<pair<int, string>>());


	/*Put the ordered_item in order, update the parent node of the node and perform it recursively every time after time.
	If there is no corresponding childrenode, create a new node, while if there is, update the frequency.
	Whenever a node is newly created, the header table and the nodes are connected.
	*/
	FPNode* p = root;
	for(int i=0; i<ordered_item.size(); i++) {
		if(p->getChildrenNode(ordered_item[i].second) != nullptr) {
			p = p->getChildrenNode(ordered_item[i].second);
			p->updateFrequency(1);
			continue;
		}
		else {
			//insert in children managed by map class
			FPNode* New = new FPNode;
			New->updateFrequency(1);
			New->setParent(p);
			p->pushchildren(ordered_item[i].second, New);
			connectNode(table, ordered_item[i].second, New);		//Connect New node
			p = p->getChildrenNode(ordered_item[i].second);			//Update FPNode p, p indicate parent
			continue;		
		}
	}
}

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	/*Move until the next node of the table is nullptr and
	 connect it to the node received by the factor.*/
	if(table->getNode(item)->getNext() == nullptr) {
		table->getNode(item)->setNext(node);
		return;
	}

	FPNode* connect = table->getNode(item)->getNext();
	while(connect) {
		if(connect->getNext() == nullptr) {
			connect->setNext(node);
			return;
		}
		connect = connect->getNext();
	}
}

bool FPGrowth::contains_single_path(FPNode* pNode) {
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

	return {};
}



void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequentPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {
	/*Print a header table sorted in descending order. 
	It also print a value smaller than the threshold.*/
	list<pair<int, string>> li = table->getindexTable();
	for(auto iter = li.begin(); iter != li.end(); iter++) {
		*fout << iter->second << " " << iter->first << endl;
	}
	return true;
}

bool FPGrowth::printTree() {
	/* After arranging the header tables in ascending order, the paths of the FP-tree are output in ascending order. */
	table->ascendingIndexTable();
	*fout << "{StandardItem, frequency} (path_Item, Frequency)" << endl;
	list<pair<int, string>> li;
	li = table->getindexTable();
	
	/*The node variable moves to next as a pointer to the data table and 
	outputs until there are no nodes connected to the corresponding product.
	The paths of the fpTree connected to the product are output to the parent node connected to the root node.*/
	for(auto iter = li.begin(); iter != li.end(); iter++) {
		if(iter->first < threshold) continue;
		else {
			*fout << "{" << iter->second << ", " << iter->first << "}" << endl;

			/* p refers to the parent of the node variable, and pp refers to the parent of p.
			The reason for implementing it in this way is that the item of p can be accessed by the map of p, */
			FPNode* node = table->getNode(iter->second);
			FPNode* p = nullptr;
			FPNode* pp = nullptr;

			node = node->getNext();			
			while(node) {
				p = node->getParent();
				pp = p->getParent();
				*fout << "(" << iter->second << ", " << node->getFrequency() << ")";
				while(pp) {
					*fout << "(" << pp->getitem(p) << ", " << p->getFrequency() << ")";
					p = pp;
					pp = pp->getParent();
				}
				node = node->getNext();
				*fout << endl;
			}
		}
	}
	return true;
}


void FPGrowth::saveFrequentPatterns(){

}
