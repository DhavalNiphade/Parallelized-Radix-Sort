#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <limits>
#include <fstream>
#include <boost/mpi.hpp>

using namespace std;
namespace mpi = boost::mpi;

vector<string> getFile(){

	vector<string> tokens;
	string line;

	// Open file and get all tokens
	ifstream dataFile("numbers.txt");

	if(dataFile){
		while(getline(dataFile,line))
			tokens.push_back(line);
		dataFile.close();
	}
	else{
		cerr << "Exception opening/reading/closing file" << endl;
		return {};
	}

	return tokens;
}

char getType(vector<string>& tokens){
	char type;
	for(int i=0;i<tokens.size();i++){
		for(int j=0;j<tokens[i].size();j++){
			if(!isdigit(tokens[i][j]) && tokens[i][j]!='.'){
				type='s';
				return type;
			}
			else if(tokens[i][j]=='.')
				type='d';
		}
	}
	return type=='d' ? 'd' : 'i';
}

int main(int argc, char* argv[]){

	/* ------------------------------ PART 1 --------------------------------- */
	// Preprocessing
	vector<string> tokens = getFile();
	char type = getType(tokens);

	vector<int> numbers;
	//vector<double> numbers;
	vector<int> localNums;
	int localMax,globalMax;

	/*
	// Convert to required type
	if(type == 'i'){
		numbers.resize(tokens.size());
		transform(tokens.begin(), tokens.end(), numbers.begin() ,
				[](const string& s){return stoi(s);});
	}

	if(type == 'd'){
		numbers.resize(tokens.size());
		transform(tokens.begin(), tokens.end(), numbers.begin(),
				[](const string& s){return stod(s);});
	}
	*/

	MPI_Init(&argc,&argv);	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status *status;
	
	globalMax = *max_element(numbers.begin(),numbers.end());

	int sendSize = numbers.size()/size;
	int overFlow = numbers.size()%size;


	// Set appropriate sizes depending on divide
	if(overFlow>0){
		if (rank==size-1)
			localNums.resize(sendSize+overFlow);
		else
			localNums.resize(sendSize);
	}
	else
		localNums.resize(sendSize);
		

	// Start bucketing
	
	




	//mpi::environment env;
	//mpi::communicator world;

	//cout << "I am process " << world.rank() << "of "<< world.size() << endl;

	return 0;
}
