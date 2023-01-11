#include "HeaderTable.h"
#include <functional>

using namespace std;

HeaderTable::~HeaderTable() {
	
}

void HeaderTable::insertTable(string item, int frequency) {
	//Exception handling
	if(item == " " || frequency == 0) return;
	//Construct indexTable
	indexTable.push_back(make_pair(frequency, item));

	//Map does't allow duplication
	FPNode* New = new FPNode;
	New->updateFrequency(frequency);
	dataTable.insert(make_pair(item, New));
	return;
}
int HeaderTable::find_frequency(string item){
	int frequency = 0;
	/*Search the list and find the frequency of the item value you want to search for and return it.*/
	for(auto iter = indexTable.begin(); iter != indexTable.end(); iter++) {
		if(iter->second == item) {
			frequency = iter->first;
			break;
		}
	}
	return frequency;
}

//PrintIndexTable
void HeaderTable::PrintIndexTable() {
	for(auto iter = indexTable.begin(); iter != indexTable.end(); iter++) {
		cout << iter->second << " " << iter->first << endl;
	}
}
