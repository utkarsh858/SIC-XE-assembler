#include <iostream>
#include <fstream>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream in;
	in.open("input.txt");
	ofstream out;
	out.open("intermediate_file");

	string x;
	long locctr;
	int starting_addr;
	queue<string> commands;
	long program_length ;
	//Pass 1
	vector <string> SYMTAB_name;
	vector <string> SYMTAB_address;
	vector <string> SYMTAB_error_flag;

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


		int size = args.size();

		cout << args[0] << args[1] <<endl;

		if(args[0][0] != '.'){  // comment handling
			if(args[size-2] == "START"){ // strting address
	
					starting_addr = atoi(args[size - 1]);
					locctr = starting_addr;
	
				} else locctr = 0;
	
			if (args[size-2] == "END" ){  //if
				
				ofstream << x;
				program_length = locctr;
			
			} // end opcode
			else {

				if(args.size() == 3){ //if LABEL field has a value
					// searching for the label
					int pos;
					if((pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), args[0])) != SYMTAB_name.end()){
						//found. set error flag
						SYMTAB_error_flag[pos] = "D";
					} else {
						//not found
						SYMTAB_name.push_back(args[0]);
						SYMTAB_address.push_back(to_string(locctr));
						SYMTAB_error_flag.push_back("");
					}
				}

			}

		}

	//Pass 2


	return 0;
}