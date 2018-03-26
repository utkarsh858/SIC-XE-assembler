#include <iostream>
#include <fstream>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
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
	
					starting_addr = stoi(args[size - 1],nullptr,0);
					locctr = starting_addr;
	
				} else locctr = 0;
	
			if (args[size-2] == "END" ){  //if
				
				out << x;
				program_length = locctr;
			
			} // end opcode
			else {
				vector<string>::iterator pos;

				if(args.size() == 3){ //if LABEL field has a value
					// searching for the label
					if((pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), args[0])) != SYMTAB_name.end()){
						//found. set error flag
						*pos = "D";
					} else {
						//not found
						SYMTAB_name.push_back(args[0]);
						SYMTAB_address.push_back(to_string(locctr));
						SYMTAB_error_flag.push_back("");
					}
				}

				if((pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), args[0])) != SYMTAB_name.end()){
					
				}

			}

		}

	}//Pass 1 end
	cout <<"symtab  " << SYMTAB_name[0];
	//Pass 2


	return 0;
}