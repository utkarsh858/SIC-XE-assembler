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

string removeSpaces(string word , string position){

    if(position == "Front"){
        for(int i =0;  i <word.length();i++){
            if(word[i] != ' '){
                return word;
            }else{
                word.erase(i);
            }
        }
        return word;

    }else if(position == "Back"){
        for(int i =word.length() - 1 ;  i >=0 ; i--){
            if(word[i] != ' '){
                return word;
            }else{
                word.erase(i);
            }
        }
        return word;
    }
}

string trim(string word) {

    if(word[0] == ' ') return removeSpaces(word, "Front");
    if(word[word.size()-1] == ' ') return removeSpaces(word , "Back");
    return word;
}

void initialize_OPTAB(vector<string>&  key, vector<string>&  fields){

	
	
}

int main(int argc, char const *argv[])
{
	ifstream in;
	in.open("input.txt");
	ofstream out;
	out.open("output.txt");

	string x;
	long locctr;
	int starting_addr;
	queue<string> commands;
	long program_length ;
	//Pass 1
	vector <string> SYMTAB_name;
	vector <string> SYMTAB_address;
	vector <string> SYMTAB_error_flag;
	vector <string> OPCODE_errors;
	vector <string> OPTAB_name;
	string text_record;

	initialize_OPTAB(OPTAB_name, OPTAB_code);

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

		//trimming failed
		// args[0] = trim(args[0]);
		// args[1] = trim(args[1]);
		// if(args.size() == 3)
			// args[0] = trim(args[0]);
		cout << trim("    ttt   ")<<"ff";
		cout << args[0] << "|"<< args[1] << "|"<<endl;

		if(args[0][0] != '.'){  // comment handling
			if(args[size-2] == "START"){ // strting address
	
					starting_addr = stoi(args[size - 1],nullptr,0);
					locctr = starting_addr;
	
				} else locctr = 0;
	
			if (args[size-2] == "END" ){  //if
				
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

				if((pos = find(OPTAB_name.begin(), OPTAB_name.end(), args[0])) != OPTAB_name.end()){ // searching for optab
					locctr += 3;   //have to see which type of opcode
				} else if(args[size-2] == "WORD"){
					locctr += 3;

				} else if(args[size-2] == "RESB"){
					locctr += stoi(args[size-1]);

				} else if(args[size-2] == "RESW"){
					locctr += 3 * stoi(args[size-1]);

				} else if(args[size-2] == "BYTE"){
					string s = args[size-1].substr(2, size-3);
					locctr += s.size()/2 ;
				} else {
					OPCODE_errors.push_back(args[size-2]+" Invalid OPCODE");
				}

			}

		}

	}//Pass 1 end
	program_length = locctr;
	cout <<"symtab  " << SYMTAB_name[0];
	//Pass 2

	while(x = commands.pop_front()){

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

		if(args[0][0] != '.'){  // comment handling
			if(args[size-2] == "START"){ // strting address
				//I dont know
			}
			out << "H" << args[0];
			for(int i=args[0].size();6;i++) out << " ";
			for (int i = args[2].size(); i < 6; ++i)
				out << 0;
			out << args[2];
			for (int i = stoi(locctr).size(); i < 6; ++i)
				out << 0;
			out << args[1] << endl;

			if(args[size-2] == "END"){
				// write last text record 
				out << text_record << endl;
				out << "E";
				vector<string> pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), args[size-1]);
				// I am doomed
			}


		}
	}

	return 0;
}