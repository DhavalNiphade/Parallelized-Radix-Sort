#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>

using namespace std;

int main(){

	auto start = std::chrono::steady_clock::now();

	clock_t startTime = clock();
	stringstream ss;
	string filename = "../intsLarge.txt";
	string line;
	vector<double> array;
	ifstream dataFile(filename);
	if(dataFile){
		while(getline(dataFile,line)){
			ss << line;
			double num;
			ss >> num;
			ss.clear();
			array.push_back(num);
		}
	}
	sort(array.begin(),array.end());
	//for(auto& x : array)
		//cout << x << " ";

	auto end = std::chrono::steady_clock::now();
	auto taken = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	cout << taken << "ms" << endl;

	cout << "Time taken = " << (float)(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl;



	return 0;

}
