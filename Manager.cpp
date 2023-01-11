#include "Manager.h"

Manager::Manager(int threshold, int bpOrder) {
	fpgrowth = new FPGrowth(&flog, threshold);
	bptree = new BpTree(&flog, bpOrder);
	flog.open("log.txt", ios::app);
	cmd = new char[256];
}

Manager::~Manager() { 
	if(flog.is_open()) {
		flog.close();
	}
}	

void Manager::run(const char* command)
{
	ifstream fin;
	fin.open(command);
	if(!fin) {
		flog << "File Open Error" << endl;
		return;
	}

	while (!fin.eof()) {	
		bool result;
		fin.getline(cmd, 256);
		/*Execute the corresponding command for each string 
		by breaking the string on a tab-by-tab basis.*/
		
		char* ptr = strtok(cmd, "\t");
		//Exception handling
		if(ptr == nullptr) continue;
		if(ptr[0] == '/' && ptr[1] == '/') continue;

		if(strcmp(ptr, "LOAD") == 0) {			
			/*If the action fails, the result is returned to false
			If the result is false, print an error code, else print Success code.
			Also, print error code if data already exists in the data structure*/
			flog << "======LOAD==========" << endl;	
			try {
				if(!fpgrowth->getTree()->getChildren().empty()) throw 100;								
				result = LOAD();
				if(!result) throw 100;
				printSuccessCode();
			}		
			catch (int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "BTLOAD") == 0) {
			/*If the action fails, the result is returned to false
			If the result is false, print an error code, else print Success code.
			Also, print error code if data already exists in the data structure*/
			flog << "======BTLOAD========" << endl;
			try {
				if(bptree->getRoot() != nullptr) throw 200;				
				result = BTLOAD(); 
				if(!result) throw 200;
				printSuccessCode();
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}			
		}
		else if(strcmp(ptr, "PRINT_ITEMLIST") == 0) {
			/*If the header table is empty, the result is returned as a value of false.*/
			flog << "=====PRINT_ITEMLIST===" << endl;
			try {
				result = PRINT_ITEMLIST();
				if(!result) throw 300;			
				flog << "======================" << endl;				
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "PRINT_FPTREE") == 0) {
			/*If fpTree is empty, result is returned as a value of false.*/
			flog << "======PRINT_FPTREE======" << endl;
			try {
				result = PRINT_FPTREE();
				if(!result) throw 400;			
				flog << "=========================" << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "PRINT_BPTREE") == 0) {
			/*If there is no FrequencyPatternt or if the BpTree is empty, 
			the result value is returned as false.	
			Also, print an error code if the type of the factor is different */
			flog << "======PRINT_BPTREE=======" << endl;
			try {
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 500;
				if(isdigit(ptr[0]) != 0) throw 500;		//Must be in string format but in numeric format

				string item(ptr);
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 500;					
				if(isdigit(ptr[0]) == 0) throw 500;		//Must be in numeric format but in string format

				int frequency = atoi(ptr);
				result = PRINT_BPTREE(item, frequency);
				if(!result) throw 500;			
				flog << "========================" << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}			
		}
		else if(strcmp(ptr, "PRINT_CONFIDENCE") == 0) {
			/*If there is no pattern to output or bpTree is empty, an error code is print. 
			In addition, if both factors are not input or the format is different,
			 an error code is print.*/
			flog << "=====PRINT_CONFIDENCE=====" << endl;
			try {
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 600;
				if(isdigit(ptr[0]) != 0) throw 600;		//Must be in string format but in numeric format
									
				string item(ptr);
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 600;
				if(isdigit(ptr[0]) == 0) throw 600;		//Must be in numeric format but in string format
					
				double rate = atof(ptr);
				result = PRINT_CONFIDENCE(item, rate);
				if(result != true) throw 600;
				flog << "=========================" << endl;		
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "PRINT_RANGE") == 0) {
			/*If there is no pattern to output or bpTree is empty, an error code is output.
			In addition, if all three factors are not input or the format is different, 
			an error code is output.*/
			flog << "=======PRINT_RANGE=======" << endl;
			try {
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 700;
				if(isdigit(ptr[0]) != 0) throw 700;		//Must be in string format but in numeric format
				
				string item(ptr);
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 700;
				if(isdigit(ptr[0]) == 0) throw 700;	//Must be in numeric format but in string format

				int minRange = atoi(ptr);
				ptr = strtok(nullptr, "\t");
				if(ptr == nullptr) throw 700;
				if(isdigit(ptr[0]) == 0) throw 700;	//Must be in numeric format but in string format
					
				int maxRange = atoi(ptr);
				result = PRINT_RANGE(item, minRange, maxRange);
				if(!result) throw 700; 			
				flog << "===========================" << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "EXIT") == 0) {
			flog << "=======EXIT==========" << endl;
			delete bptree;
			delete fpgrowth;
			printSuccessCode();
			fin.close();
			flog.close();
			delete[] cmd;
			return;
		}
	}	
	return;
}

bool Manager::LOAD()
{
	//open market.txt
	ifstream fload("market.txt");
	if(!fload) return false;
	

	char* buf = new char[256];
	list<pair<int, string>> temp;

	/*First, by reading market.txt, each item value and frequency value 
	are stored in the list and a header table is generated.*/
	while(!fload.eof()) {
		fload.getline(buf, 256);
		char* ptr = strtok(buf, "\t");
		if(ptr == nullptr) continue;
		if(ptr == " ") continue;		//Expection handling

		list<pair<int, string>>::iterator iter;
		while(ptr != nullptr) {			
			if(ptr == " ") continue;
			for(iter = temp.begin(); iter != temp.end(); iter++) {
				if(iter->second == ptr) {
					iter->first += 1;
					break;
				}
			}
			if(iter == temp.end()) {
				string s(ptr);
				temp.push_back(make_pair(1, s));
			}
			ptr = strtok(nullptr, "\t");
		}
	}
	fload.close();
	//Create HeaderTable
	for(const auto& elem : temp) {
		fpgrowth->createTable(elem.second, elem.first);
	}

	/*Read market.txt again and save one line in the list to build an fpTree.*/
	ifstream _fload("market.txt");
	while(!_fload.eof()) {
		_fload.getline(buf, 256);
		list<string> item_array;
		char* ptr = strtok(buf, "\t");

		while(ptr != nullptr) {
			if(ptr == " ") continue;
			string s(ptr);
			item_array.push_back(s);
			ptr = strtok(nullptr, "\t");
		}
		fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), item_array);
	}
	delete[] buf;
	_fload.close();
	return true;
}

bool Manager::BTLOAD() {

	/*In the vector, the factor type is declared as a pair type int,
	set<string> type, and the first value of the file is stored as an integer type, 
	and the remaining product information is stored as a set<string> type to construct a bpTree.*/
	ifstream fbtload("result.txt");
	if(!fbtload) return false;
	vector<pair<int, set<string>>> temp;
	char* buf = new char[256];

	while(!fbtload.eof()) {
		fbtload.getline(buf, 256);
		char* ptr = strtok(buf, "\t");
		set<string> item_set;
		if(ptr == nullptr) continue;
		if(ptr == " ") continue;

		int frequency = atoi(ptr);			
		ptr = strtok(nullptr, "\t");
		while(ptr != nullptr) {		
			item_set.insert(ptr);
			ptr = strtok(nullptr, "\t");
		}
		temp.push_back(make_pair(frequency, item_set));
	}
	fbtload.close();
	//construct BpTree
	for(const auto& elem : temp) {
		bptree->Insert(elem.first, elem.second);
	}
	
	delete[] buf;
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	//if HeadTable is Empty return false
	if(fpgrowth->getTree()->getChildren().empty()) return false;
	if(fpgrowth->getHeaderTable()->getindexTable().empty()) return false;
	/*Products stored in the header table are output in descending order based on frequency. 
	It also prints products with a frequency smaller than the threshold.*/
	fpgrowth->printList();
	return true;
}

bool Manager::PRINT_FPTREE() {
	//if fpTree is empty return false
	if(fpgrowth->getTree()->getChildren().empty()) return false;
	/*The FP-Tree information of the FP-growth is output. 
	It is sorted in ascending order based on frequency and outputs products with threshold or higher.*/
	fpgrowth->printTree();
	return true;
}

bool Manager::PRINT_BPTREE(string item, int min_frequency) {
	//If there is no item pattern corresponding to bpTree, it returns false.
	bool nofattern = false;
	/*It searches in bpTree based on the minimum frequency, 
	satisfies the minimum frequency of Frequenct Patterns stored in the bpTree, 
	and moves by outputting a pattern containing the input product.*/
	nofattern = bptree->printFrequency(item, min_frequency);

	return nofattern;
}

bool Manager::PRINT_CONFIDENCE(string item, double rate) {
	//If there is no item pattern corresponding to bpTree, it returns false.
	bool pattern = false;
	//Returns the number of frequency of that item from the header table.
	double item_frequency = fpgrowth->item_frequency(item);

	//Calling a function that outputs a pattern with a confidence value above the association rate with the entered product
	pattern = bptree->printConfidence(item, item_frequency, rate);
	return pattern;
}

bool Manager::PRINT_RANGE(string item, int start, int end) {
	//If there is no item pattern corresponding to bpTree, it returns false.
	bool pattern = false;
	//Calling a function that navigates from minimum to maximum frequency
	pattern = bptree->printRange(item, start, end);
	return pattern;
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "ERROR " << n << endl;
	flog << "=======================" << endl;
	return;
}

void Manager::printSuccessCode() {					//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl;
	return;
}

