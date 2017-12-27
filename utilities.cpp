#include "utilities.h"

vector<string> getFile(string filename){

	vector<string> tokens;
	string line;

	// Open file and get all tokens
	ifstream dataFile(filename);

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
			if(!isdigit(tokens[i][j]) && tokens[i][j]!='.' && tokens[i][0]!='-'){
				type='s';
				return type;
			}
			else if(tokens[i][j]=='.')
				type='d';
		}
	}
	return type=='d' ? 'd' : 'i';
}

