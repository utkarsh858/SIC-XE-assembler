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

key.push_back("ADD");key.push_back("ADDF");key.push_back("ADDR");key.push_back("AND");key.push_back("CLEAR");key.push_back("COMP");key.push_back("COMPF");key.push_back("COMPFR");key.push_back("DIV");key.push_back("DIVF");key.push_back("DIVR");key.push_back("FIX");key.push_back("FLOAT");key.push_back("HIO");key.push_back("J");key.push_back("JEQ");key.push_back("JGT");key.push_back("JLT");key.push_back("JSUB");key.push_back("LDA");key.push_back("LDB");key.push_back("LDCH");key.push_back("LDF");key.push_back("LDL");key.push_back("LDS");key.push_back("LDT");key.push_back("LDX");key.push_back("LPS");key.push_back("MUL");key.push_back("MULF");key.push_back("MULR");key.push_back("NORM");key.push_back("OR");key.push_back("RD");key.push_back("RMO");key.push_back("RSUB");key.push_back("SHIFTL");key.push_back("SHIFTR");key.push_back("SIO");key.push_back("SSK");key.push_back("STA");key.push_back("STB");key.push_back("STCH");key.push_back("STF");key.push_back("STI");key.push_back("STL");key.push_back("STS");key.push_back("STSW");key.push_back("STT");key.push_back("STX");key.push_back("SUB");key.push_back("SUBF");key.push_back("SUBR");key.push_back("SVC");key.push_back("TD")key.push_back("TIO");key.push_back("TIX");key.push_back("TIXR");key.push_back("WD");
fields.push_back("18");fields.push_back("58");fields.push_back("90");fields.push_back("40");fields.push_back("B4");fields.push_back("28");fields.push_back("88");fields.push_back("A0");fields.push_back("24");fields.push_back("64");fields.push_back("9C");fields.push_back("C4");fields.push_back("C0");fields.push_back("F4");fields.push_back("3C");fields.push_back("30");fields.push_back("34");fields.push_back("38");fields.push_back("48");fields.push_back("00");fields.push_back("68");fields.push_back("50");fields.push_back("70");fields.push_back("08");fields.push_back("6C");fields.push_back("74");fields.push_back("04");fields.push_back("D0");fields.push_back("20");fields.push_back("60");fields.push_back("98");fields.push_back("C8");fields.push_back("44");fields.push_back("D8");fields.push_back("AC");fields.push_back("4C");fields.push_back("A4");fields.push_back("A8");fields.push_back("F0");fields.push_back("EC");fields.push_back("0C");fields.push_back("78");fields.push_back("54");fields.push_back("80");fields.push_back("D4");fields.push_back("14");fields.push_back("7C");fields.push_back("E8");fields.push_back("84");fields.push_back("10");fields.push_back("1C");fields.push_back("5C");fields.push_back("94");fields.push_back("B0");fields.push_back("E0");fields.push_back("F8");fields.push_back("2C");fields.push_back("B8");fields.push_back("DC");

}

string parse(string s){
	string res;
	for (int i = 2; i < s.size-1; ++i)
	{
		s += stoi((int)s[i]);
	}
	return s;
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
	vector <string> OPTAB_code;
	string text_record;

	initialize_OPTAB(OPTAB_name, OPTAB_code);

	while (getline(in, x)){
		commands.push(x);

		//splitting the string
		vector<string> args;
		string opcode = NULL;
		string operand = NULL;
		string label = NULL;

		string delimiter = " ";
		string token = x.substr(0, x.find(delimiter));
		
		size_t pos = 0;
		while ((pos = x.find(delimiter)) != string::npos) {
		    token = x.substr(0, pos);
			args.push_back(token);		    
		    x.erase(0, pos + delimiter.length());
		}  


		int size = args.size();

		if(args.size == 3){
			opcode = args[1];
			label = args[0];
			operand = args[2];
		} else if(args.size == 2){
			opcode = args[1];
			operand = args[2];

		} else if(args.size == 1){
			label = args[0];

		}

		//trimming failed
		// args[0] = trim(args[0]);
		// args[1] = trim(args[1]);
		// if(args.size() == 3)
			// args[0] = trim(args[0]);
		if(args.size() > 0){
		cout << label << "|"<< opcode << "|"<< operand <<endl;

		if(args[0][0] != '.'){  // comment handling
			if(opcode == "START"){ // strting address
	
					starting_addr = stoi(operand,nullptr,0);
					locctr = starting_addr;
	
				} else locctr = 0;
	
			if (opcode == "END" ){  //if
				
				program_length = locctr;
			
			} // end opcode
			else {
				vector<string>::iterator pos;

				if(args.size() == 3){ //if LABEL field has a value
					// searching for the label
					if((pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), label)) != SYMTAB_name.end()){
						//found. set error flag
						*pos = "D";
					} else {
						//not found
						SYMTAB_name.push_back(label);
						SYMTAB_address.push_back(to_string(locctr));
						SYMTAB_error_flag.push_back("");
					}
				}

				if((pos = find(OPTAB_name.begin(), OPTAB_name.end(), args[0])) != OPTAB_name.end()){ // searching for optab
					locctr += 3;   //have to see which type of opcode
				} else if(opcode == "WORD"){
					locctr += 3;

				} else if(opcode == "RESB"){
					locctr += stoi(operand);

				} else if(opcode == "RESW"){
					locctr += 3 * stoi(operand);

				} else if(opcode == "BYTE"){
					string s = operand.substr(2, size-3);
					locctr += s.size()/2 ;
				} else {
					OPCODE_errors.push_back(opcode+" Invalid OPCODE");
				}

			}

		}
	} // no command

		operand = NULL;
		opcode = NULL;
		label = NULL;
	}//Pass 1 end
	program_length = locctr;
	//Pass 2
	locctr = 0 ;
	while(x = commands.pop_front()){
		//splitting the string
		vector<string> args;
		
		string delimiter = " ";
		string token = x.substr(0, x.find(delimiter));
		string opcode_num;
		size_t pos = 0;
		while ((pos = x.find(delimiter)) != string::npos) {
		    token = x.substr(0, pos);
			args.push_back(token);		    
		    x.erase(0, pos + delimiter.length());
		}  


		int size = args.size();

		if(args.size == 3){
			opcode = args[1];
			label = args[0];
			operand = args[2];
		} else if(args.size == 2){
			opcode = args[1];
			operand = args[2];

		} else if(args.size == 1){
			label = args[0];

		}


		if(args.size() > 0 ){

		if(args[0][0] != '.'){  // comment handling
			if(opcode == "START"){ // strting address
				starting_addr = stoi(operand,nullptr,0);
				locctr = starting_addr;
	

					//lets initialize a new text record
				out << "H" << label;
				for(int i=label.size();6;i++) out << " ";
				for (int i = operand.size(); i < 6; ++i)
					out << 0;
				out << operand;
				for (int i = stoi(locctr).size(); i < 6; ++i)
					out << 0;
				out << locctr << endl;
	
				text_record+="T";
				for (int i = operand.size(); i < 6; ++i) text_record += "0";
				text_record += operand;
				text_record += "1E";

			}

			if(opcode == "END"){
				// write last text record 
				out << text_record << endl;
				out << "E";
				int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) - SYMTAB_name.begin();
				string program_start = *(SYMTAB_address.begin() + pos) ;
				for (int i = program_start.size(); i <= 6 ; ++i)
					out << "0";
				out << program_start << endl;
			}

			if((find(OPCODE_name.begin(),OPCODE_name.end(), opcode)) != OPCODE_name.end()){
				string operand_address;
				//found
				locctr += 3;
				if(operand != NULL){
					if(find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) != SYMTAB_name.end()){

						int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) - SYMTAB_name.begin();
						operand_address = *(SYMTAB_address.begin()+pos);

					}
					else {
						operand_address = "0";
						OPCODE_errors.push("Undefined symbol"+opcode);
					}
					//assemble the object code instruction
					text_record += opcode_num;
					text_record += operand_address;
				}
			} else if(opcode == "BYTE" || opcode == "WORD"){
				// constant to object code
				if(opcode == "WORD") {
					locctr += 3;

					//converting to hexadecimal
					int num_dec= stoi(operand);
					std::stringstream ss;
					ss<< std::hex << num_dec; // int decimal_value
					std::string num_hex ( ss.str() );
					for(int i = num_hex.size();i <= 6 ;i++) 
						text_record += "0";
					text_record += num_hex;

				}
				else{
					string record;
					if(operand[0] == 'X'){
						record = parse(operand);
					}else if(operand[0] == 'C'){
						record = operand.substr(2,size-3);
					}

					string s = operand.substr(2, size-3);
					locctr += s.size()/2 ;
					

				} else if(opcode == "RESB"){
					locctr += stoi(operand);
				}else if(opcode == "RESW"){
					locctr += stoi(operand) * 3;
				}

			}

			if(text_record.size() + object_code.size() > 69){
				out << text_record <<endl;
				//initialize_T(text_record);
				text_record = "T";
				text_record += locctr;
				text_record += "1E";
			
			}
			text_record += object_code;



		}
		}

		operand = NULL;
		opcode = NULL;
		label = NULL;
	}

	return 0;
}