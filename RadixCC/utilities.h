#ifndef UTILITY_H
#define UTILITY_H


#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <mpi.h>
#include <queue>

using namespace std;

vector<string> getFile(string filename);

char getType(vector<string>& tokens);

template<class T, class A>
void print(vector<T,A> const& container){
	for(auto it : container)
		cout << it << " ";
	cout << endl;
	return;
}

template<class K, class V, class A>
void clearMap(map<K,V,A>& container){
	for(auto& x: container)
		x.second.clear();
	return;
}

template<class One, class Two, class A>
void fillNums(vector<One,A>& nums, const map<Two,vector<One>>& hash){
	nums.clear();
	for(auto x : hash){
		if(x.second.size() > 0){
			for(int i=0 ; i<x.second.size() ; i++){
				nums.push_back(x.second[i]);
			}
		}
	}
	return;
}


void sortDoubles(vector<string>& globalNums, int argc, char* argv[],vector<double>& sortedArray, string mode);

void sortIntegers(vector<string>& globalNums, int argc, char* argv[],vector<int>& sortedArray, string mode);

void sortStrings(vector<string>& globalNums, int argc, char* argv[],vector<string>& sortedArray, string mode);

#endif

