#include "utilities.h"

void sortDoubles(vector<string>& globalNums, int argc, char* argv[], vector<double>& sortedArray, string mode="-r"){

	/* ----------------------------------------- Part 1 --------------------------------------------------------- */
	vector<double> localNums;	
	int rank, size;
	map<double,vector<double>> localHash;

	// Start MPI
	MPI_Init(&argc,&argv);	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status *status;

	if(size > globalNums.size()){
		cerr << "# processes exceed the number of doubles to be sorted" << endl;
		return;
	}

	
	clock_t startTime = clock();

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
			localNums[j]=stod(globalNums[i]);
	}
	else{
		for(int i=rank*perProcessSize,j=0 ; i<globalNums.size() ; i++, j++)
			localNums[j]=stod(globalNums[i]);
	}
	

	/* ------------------------------ PART 2 --------------------------------- */


	// Setup hash table
	for(int i=-9 ; i<10 ; i++) 
		localHash[i]={};
	

	string localMax="";
	// Get maximum local element
	for(int i=0 ; i<localNums.size() ; i++){
		if(to_string(localNums[i]).size() > localMax.size())
			localMax = to_string(localNums[i]);
	}
	
	double n=0.000010;
	double m=0.000001;
	for(int i=0 ; i<localMax.size() ; i++){
		for(int j=0 ; j<localNums.size() ; j++){
			//double mod = localNums[j]%n;
			double mod = fmod(localNums[j],n);
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
		MPI_Send(&localNums[0],localNums.size(),MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(rank==0){
		localNums.resize(globalNums.size());
		for(int i=size-2 ; i>0 ; i--){
			MPI_Recv(&localNums[(i)*perProcessSize], perProcessSize, MPI_DOUBLE,(i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		MPI_Recv(&localNums[(size-1)*perProcessSize], outlierSize, MPI_DOUBLE, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		
	}
	
	priority_queue<double,vector<double>,greater<double>> minHeap;

	if(rank==0){

		//MIN HEAP IMPLEMENTATION
		for(int i=0 ; i<localNums.size() ; i++)
			minHeap.push(localNums[i]);

		if(mode == "-v"){
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
		cout << "Time taken = " << double(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl;

	}
	
	MPI_Finalize();
	return;
}

