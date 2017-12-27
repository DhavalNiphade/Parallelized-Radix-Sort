#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <mpi.h>
#include <string>
#include "utilities.h"

using namespace std;

int main(int argc, char* argv[]){

	/* ------------------------------ PART 1 --------------------------------- */

	// Preprocessing

	string filename = argv[1];
	string mode;
	if(argv[2])
		mode = argv[2];

	// Get Type of File
	vector<string> globalNums = getFile(filename);
	char type = getType(globalNums);
	
	if(type=='i'){
		vector<int> sortedArray;
		sortIntegers(globalNums,argc,argv,sortedArray,mode);
	}
	else if(type=='s'){
		vector<string> sortedArray;
		sortStrings(globalNums,argc,argv,sortedArray,mode);
	}
	else{
		vector<double> sortedArray;
		sortDoubles(globalNums,argc,argv,sortedArray,mode);
	}

	return 0;
}

