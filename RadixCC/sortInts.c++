#include "utilities.h"

void sortIntegers(vector<string>& globalNums, int argc, char* argv[],vector<int>& sortedArray, string mode="-r"){

	/* ----------------------------------------- Part 1 --------------------------------------------------------- */
	vector<int> localNums;
	int rank, size;
	map<int,vector<int>> localHash;
	
	clock_t startTime = clock();

	// Start MPI
	MPI_Init(&argc,&argv);	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status *status;
	
	if(size > globalNums.size()){
		cerr << "# processes exceed number of integers " << endl;
		return;
	}

	int perProcessSize = globalNums.size()/size;
	int overFlow = globalNums.size()%size;
	int sendSize;
	int outlierSize=0 ;

	// Set appropriate sizes depending on division result in the previous step
	if(overFlow>0){
		if (rank==size-1){
			localNums.resize(perProcessSize+overFlow);
			sendSize = perProcessSize + overFlow;
		}
		else{
			localNums.resize(perProcessSize);
			sendSize = perProcessSize;
		}
		outlierSize = perProcessSize + overFlow;
	}
	else{
		localNums.resize(perProcessSize);
		sendSize = perProcessSize;
	}	


	// Distribute numbers amongst processes
	if(rank < size-1){
		for(int i=rank*perProcessSize,j=0 ; i<(rank+1)*perProcessSize ; i++, j++)
			localNums[j]=stoi(globalNums[i]);
	}
	else{
		for(int i=rank*perProcessSize,j=0 ; i<globalNums.size() ; i++, j++)
			localNums[j]=stoi(globalNums[i]);
	}
	


	/* ------------------------------ PART 2 --------------------------------- */

	// Setup hash table
	for(int i=-9 ; i<10 ; i++) 
		localHash[i]={};
	
	
	// Get maximum local element
	int localMax = *max_element(localNums.begin(),localNums.end());
	

	// TODO : Change this to a function
	// Start bucketing
	int n=10,m=1;
	for(int i=0 ; i<to_string(localMax).size() ; i++){
		for(int j=0 ; j<localNums.size() ; j++){
			int mod = localNums[j]%n;
			int div = mod/m;
			localHash[div].push_back(localNums[j]); 
		}

		n*=10;
		m*=10;
		fillNums(localNums,localHash);
		clearMap(localHash);
	}


	/* ------------------------------ PART 3 --------------------------------- */

	// Assemble sorted buckets to root process
	if(rank!=0){
		MPI_Send(&localNums[0],localNums.size(),MPI_INT,0,0,MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(rank==0){
		localNums.resize(globalNums.size());
		for(int i=size-2 ; i>0 ; i--){
			MPI_Recv(&localNums[(i)*perProcessSize], perProcessSize, MPI_INT,(i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		MPI_Recv(&localNums[(size-1)*perProcessSize], outlierSize, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		
	}
		
	priority_queue<int,vector<int>,greater<int>> minHeap;

	if(rank==0){

		// MIN HEAP IMPLEMENTATION
		for(int i=0 ; i<localNums.size() ; i++)
			minHeap.push(localNums[i]);

		if(mode=="-v"){
			while(!minHeap.empty()){
				cout << minHeap.top() << " ";
				minHeap.pop();
			}
		}

		if(mode=="-vr"){
			while(!minHeap.empty()){
				cout << minHeap.top() << " ";
				sortedArray.push_back(minHeap.top());
				minHeap.pop();
			}
		}

		if(mode=="-r"){
			while(!minHeap.empty()){
				sortedArray.push_back(minHeap.top());
				minHeap.pop();
			}
		}
		cout << endl;
		cout << "Time taken for execution = " << float(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl;
	}
	
	MPI_Finalize();
	return;
}

