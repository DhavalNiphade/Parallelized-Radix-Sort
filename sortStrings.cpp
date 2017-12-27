#include "utilities.h"

void sortStrings(vector<string>& globalWords, int argc, char* argv[], vector<string>& sortedArray, string mode= "-r"){

	vector<string> localWords;	
	int rank, size;
	map<char,vector<string>> localHash;

	clock_t startTime = clock();

	// Start MPI
	MPI_Init(&argc,&argv);	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status *status;

	if(size > globalWords.size()){
		cerr << "# processes exceed the number of strings to sort" << endl;
		return;
	}


	int perProcessSize = globalWords.size()/size;
	int overFlow = globalWords.size()%size;
	int sendSize;
	int outlierSize=0 ;


	// Set appropriate sizes depending on division result in the previous step
	if(overFlow>0){
		if (rank==size-1){
			localWords.resize(perProcessSize+overFlow);
			sendSize = perProcessSize + overFlow;			
		}
		else{
			localWords.resize(perProcessSize);
			sendSize = perProcessSize;
		}
		outlierSize = perProcessSize + overFlow;
	}
	else{
		localWords.resize(perProcessSize);
		sendSize = perProcessSize;
	}	


	// Distribute numbers amongst processes
	if(rank < size-1){
		for(int i=rank*perProcessSize,j=0 ; i<(rank+1)*perProcessSize ; i++, j++)
			localWords[j]=globalWords[i];
	}
	else{
		for(int i=rank*perProcessSize,j=0 ; i<globalWords.size() ; i++, j++)
			localWords[j]=globalWords[i];
	}



	// Setup hash table
	string allChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	for(int i=0 ; i<allChars.size() ; i++) 
		localHash[i]={};

	// Get maximum local element
	string localMax = *max_element(localWords.begin(),localWords.end(),[](const string& a, const string& b){return a.size() < b.size();});

	// TODO : Change this to a function
	// Start bucketing
	for(int i=localMax.size()-1 ; i>=0 ; i--){
		for(int j=0 ; j<localWords.size() ; j++){
			char curr = i >= localWords[i].size() ? 'a' : localWords[j][i];
			localHash[curr].push_back(localWords[j]); 
		}
		fillNums(localWords,localHash);
		clearMap(localHash);
	}

	// Assemble sorted buckets to root process
	if(rank!=0){
		for(int i=0 ; i<localWords.size() ; i++)
			MPI_Send(localWords[i].c_str(),localWords[i].size(),MPI_CHAR,0,i,MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		//localWords.resize(globalWords.size());
		MPI_Status statusX;
		int counter=0, recv_count=0;
		for(int i=size-2 ; i>0 ; i--){		
			for(int j=0 ; j<perProcessSize ; j++){
				MPI_Probe(i,j,MPI_COMM_WORLD,&statusX);
				MPI_Get_count(&statusX,MPI_CHAR,&recv_count);
				char* buffer = (char*)malloc(sizeof(char)*recv_count);
				MPI_Recv(buffer,recv_count,MPI_CHAR,i,j,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				string temp (buffer);
				localWords.push_back(temp);
				delete [] buffer;
			}
		}

		for(int i=0 ; i<outlierSize; i++){
			MPI_Probe(size-1,i,MPI_COMM_WORLD,&statusX);
			MPI_Get_count(&statusX,MPI_CHAR,&recv_count);
			char* buffer = (char*)malloc(sizeof(char)*recv_count);
			MPI_Recv(buffer,recv_count,MPI_CHAR,size-1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			string temp(buffer);
			localWords.push_back(temp);
			delete [] buffer;
		}	
	}

	if(rank==0){

		sort(localWords.begin(),localWords.end());
		if(mode=="-v")
			print(sortedArray);
		if(mode=="-vr"){		
			copy(localWords.begin(),localWords.end(),back_inserter(sortedArray));
			print(sortedArray);
		}
		if(mode=="-r")			
			copy(localWords.begin(),localWords.end(),back_inserter(sortedArray));
		
		cout << endl;
		cout << "Time taken = " << float(clock()-startTime)/CLOCKS_PER_SEC << "s" << endl;
	}

	MPI_Finalize();

	return;
}

