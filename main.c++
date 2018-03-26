#include <iostream>
#include <fstream>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream in;
	in.open("input.txt");
	string x;
	long locctr;
	int starting_addr;
	queue<string> commands;
	
	//Pass 1

	while (getline(in, x)){
		commands.push(x);

		//splitting the string
		vector<string> args;
		
		string delimiter = " ";
		string token = x.substr(0, x.find(delimiter));
		
		size_t pos = 0;
		while ((pos = x.find(delimiter)) != string::npos) {
		    token = x.substr(0, pos);
			args.push_back(token);		    
		    x.erase(0, pos + delimiter.length());
		}  


		// int size = args.size();

		// cout << args[0] << args[1] <<endl;
		// if(args[size-2] == "START"){

		// 	starting_addr = atoi(args[size - 1]);
		// 	locctr = starting_addr;

		// } else locctr = 0;

		// while (args[size-2] != "OPCODE"){

		// } // end opcode

	}

	//Pass 2


	return 0;
}