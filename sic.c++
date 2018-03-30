#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

void length_text_record(string& text_record){
	
	int size = (text_record.size()-9)/2;
	stringstream ss;
	cout << text_record<< "With size" << size<<endl;
	ss << hex << size;
	string s = ss.str();
	if(s.size() == 1) s = "0"+s;
	text_record = text_record.substr(0,7)+s+text_record.substr(9,text_record.size()-9);
	cout << "changed text record" << text_record<<endl;
}

void write_the_text_record(string& text_record, string object_code, ofstream& out, long locctr){
	cout << "Writing to text record\n" ;
	if(text_record.size() + object_code.size() > 69){
		length_text_record(text_record);
		out << text_record <<endl;
				//initialize_T(text_record);
		text_record = "T";
		stringstream ss;
		ss << hex << locctr;
		string counter = ss.str();
		for (int i = 0; i < 6-counter.size(); ++i)
		{
			text_record += "0";
		}
		text_record += counter;
		text_record += "1E";

	}
	cout << "This -> " <<object_code << endl;
	text_record += object_code;
}

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
	cout << "OPTAB initialized succesfully" << endl;
	fields.push_back("18");fields.push_back("58");fields.push_back("90");fields.push_back("40");fields.push_back("B4");fields.push_back("28");fields.push_back("88");fields.push_back("A0");fields.push_back("24");fields.push_back("64");fields.push_back("9C");fields.push_back("C4");fields.push_back("C0");fields.push_back("F4");fields.push_back("3C");fields.push_back("30");fields.push_back("34");fields.push_back("38");fields.push_back("48");fields.push_back("00");fields.push_back("68");fields.push_back("50");fields.push_back("70");fields.push_back("08");fields.push_back("6C");fields.push_back("74");fields.push_back("04");fields.push_back("D0");fields.push_back("20");fields.push_back("60");fields.push_back("98");fields.push_back("C8");fields.push_back("44");fields.push_back("D8");fields.push_back("AC");fields.push_back("4C");fields.push_back("A4");fields.push_back("A8");fields.push_back("F0");fields.push_back("EC");fields.push_back("0C");fields.push_back("78");fields.push_back("54");fields.push_back("80");fields.push_back("D4");fields.push_back("14");fields.push_back("7C");fields.push_back("E8");fields.push_back("84");fields.push_back("10");fields.push_back("1C");fields.push_back("5C");fields.push_back("94");fields.push_back("B0");fields.push_back("E0");fields.push_back("F8");fields.push_back("2C");fields.push_back("B8");fields.push_back("DC");
	key.push_back("ADD");key.push_back("ADDF");key.push_back("ADDR");key.push_back("AND");key.push_back("CLEAR");key.push_back("COMP");key.push_back("COMPF");key.push_back("COMPFR");key.push_back("DIV");key.push_back("DIVF");key.push_back("DIVR");key.push_back("FIX");key.push_back("FLOAT");key.push_back("HIO");key.push_back("J");key.push_back("JEQ");key.push_back("JGT");key.push_back("JLT");key.push_back("JSUB");key.push_back("LDA");key.push_back("LDB");key.push_back("LDCH");key.push_back("LDF");key.push_back("LDL");key.push_back("LDS");key.push_back("LDT");key.push_back("LDX");key.push_back("LPS");key.push_back("MUL");key.push_back("MULF");key.push_back("MULR");key.push_back("NORM");key.push_back("OR");key.push_back("RD");key.push_back("RMO");key.push_back("RSUB");key.push_back("SHIFTL");key.push_back("SHIFTR");key.push_back("SIO");key.push_back("SSK");key.push_back("STA");key.push_back("STB");key.push_back("STCH");key.push_back("STF");key.push_back("STI");key.push_back("STL");key.push_back("STS");key.push_back("STSW");key.push_back("STT");key.push_back("STX");key.push_back("SUB");key.push_back("SUBF");key.push_back("SUBR");key.push_back("SVC");key.push_back("TD");key.push_back("TIO");key.push_back("TIX");key.push_back("TIXR");key.push_back("WD");

}


string parse(string s){
	cout << "given string is :: " << s;
	string res;
	stringstream ss;
	for (int i = 2; i < s.size()-1; ++i)
	{
		ss << hex << (int)s[i];
		res += ss.str();
		ss.str("");
	}
	cout <<  "Parsed string is :: "+res+"----";
	return res;
}


int main(int argc, char const *argv[])
{
	ifstream in;
	in.open("input.txt");
	ofstream out;
	out.open("output.txt");

	string x;
	long locctr = 0;
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
	bool text_record_printer = true;
	initialize_OPTAB(OPTAB_name, OPTAB_code);

	string opcode = "";
	string operand = "";
	string label = "";

	//Pass 1
	while (getline(in, x)){
		cout << "Got this::: " << x;
		if(x != "")
			commands.push(x);
		else 
			continue;
		//splitting the string
		vector<string> args;

		string delimiter = " ";
		string token = x.substr(0, x.find(delimiter));
		// cout << "1" <<endl;
		size_t pos = 0;
		while ((pos = x.find(delimiter)) != string::npos) {
			token = x.substr(0, pos);
			args.push_back(token);		    
			x.erase(0, pos + delimiter.length());
		}  
		args.push_back(x.substr(0, x.size()));
		// cout << "2" <<endl;

		int size = args.size();

		for (int i = 0; i < args.size(); ++i)
		{
			cout << "args:" << i <<":" <<args[i] <<endl;
		}

		if(size == 3){
			opcode = args[1];
			label = args[0];
			operand = args[2];
		} else if(size == 2){
			opcode = args[0];
			operand = args[1];

		} else if(size == 1){
			opcode = args[0];

		}
		// cout << "3" <<endl;

		//trimming failed
		// args[0] = trim(args[0]);
		// args[1] = trim(args[1]);
		// if(args.size() == 3)
			// args[0] = trim(args[0]);
			cout << label << "|"<< opcode << "|"<< operand <<endl;
			// cout << "4" << endl;

		if(args[0][0] != '.'){  // comment handling
			if(opcode == "START"){ // strting address

				starting_addr = stoi(operand,nullptr,16);
				

				locctr = starting_addr;
				// cout << "5" << endl;

			} ;

			if (opcode == "END" ){  //if
				
				program_length = locctr;
				// cout << "6" << endl;

			} // end opcode
			else {
				vector<string>::iterator pos;

				if(label != ""){ //if LABEL field has a value
					// searching for the label
					// cout << "LABEL found" << endl;
					if((pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), label)) != SYMTAB_name.end()){
						//found. set error flag
						OPCODE_errors.push_back("OOPs duplicate label -> "+*pos);
					} else {
						//not found
						SYMTAB_name.push_back(label);
						cout << locctr << endl;
						stringstream ss ;
						ss << hex << locctr;
						SYMTAB_address.push_back(ss.str());
						SYMTAB_error_flag.push_back("");
					}
					// cout << "7"<<endl;

				}

				if(find(OPTAB_name.begin(), OPTAB_name.end(), opcode) != OPTAB_name.end()){ // searching for optab
					// cout << "mast\n";
					locctr += 3;   //have to see which type of opcode
				} else if(opcode == "WORD"){
					locctr += 3;

				} else if(opcode == "RESB"){
					locctr += stoi(operand);

				} else if(opcode == "RESW"){
					locctr += 3 * stoi(operand);

				} else if(opcode == "BYTE"){
					//------------------------------------------------------------------
					if(operand[0] == 'X')
						locctr += (operand.size()-3)/2;
					else
						locctr += (operand.size()-3);
				} else {
					OPCODE_errors.push_back(opcode+" Invalid OPCODE");
				}

			}
			// cout << "9" <<endl;

		}

	operand = "";
	opcode = "";
	label = "";
	// cout << "10"<<endl;

	}//Pass 1 end


















	cout << "Pass 1 finished!\n";
	cout << "SYMTAB has been maufactured as follows:\n";
	for (vector<string>::iterator i = SYMTAB_name.begin(); i < SYMTAB_name.end(); ++i)
	{
		int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), *i) - SYMTAB_name.begin();
		cout << *(SYMTAB_name.begin()+pos);
		cout << "\t" << *(SYMTAB_address.begin()+pos) << endl;
	}
	in.close();
	program_length = locctr;
	locctr = 0 ;
	// cout << "11"<<endl;
















	//Pass 2
	while(!commands.empty()){
		x = commands.front();
		commands.pop();
		//splitting the string
		vector<string> args;
		string object_code = "";
		string delimiter = " ";
		string token = x.substr(0, x.find(delimiter));
		string opcode_num;
		size_t pos = 0;
		// cout << "12"<<endl;

		while ((pos = x.find(delimiter)) != string::npos) {
			token = x.substr(0, pos);
			args.push_back(token);		    
			x.erase(0, pos + delimiter.length());
		}  
		args.push_back(x.substr(0, x.size()));

		// cout << "13"<<endl;

		int size = args.size();

		if(size == 3){
			opcode = args[1];
			label = args[0];
			operand = args[2];
		} else if(size == 2){
			opcode = args[0];
			operand = args[1];

		} else if(size == 1){
			opcode = args[0];

		}

		cout << args[0];
		// cout << "14"<<endl;


		if(args[0][0] != '.'){  // comment handling
			if(opcode == "START"){ // starting address

				locctr = starting_addr;


					
				out << "H" << label;
				for(int i=label.size();i < 6;i++) out << " ";
					for (int i = operand.size(); i < 6; ++i)
						out << 0;
					out << operand;
					for (int i = to_string(locctr).size(); i < 6; ++i)
						out << 0;
					out << locctr << endl;

				//new text record initialized
					text_record+="T";
					for (int i = operand.size(); i < 6; ++i) text_record += "0";
						text_record += operand;
					text_record += "1E";
					// cout << "15"<<endl;

				}
				// cout << "14.1" <<endl;
				if(opcode == "END"){
				// write last text record 
					length_text_record(text_record);
					out << text_record << endl;
					out << "E";
					int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) - SYMTAB_name.begin();
					string program_start = *(SYMTAB_address.begin() + pos) ;
					for (int i = program_start.size(); i < 6 ; ++i)
						out << "0";
					out << program_start << endl;
					// cout << "16"<<endl;

				}
				// cout << "14.2" <<endl;


				if((find(OPTAB_name.begin(),OPTAB_name.end(), opcode)) != OPTAB_name.end()){
					pos = find(OPTAB_name.begin(),OPTAB_name.end(), opcode) - OPTAB_name.begin();
					opcode_num = *(pos + OPTAB_code.begin());
					string operand_address;
				//found
					if(operand != ""){
						
						if(text_record_printer == false)
						{
							length_text_record(text_record);
							out << text_record << endl;
							text_record = "T";
							stringstream ss;
							ss << hex << locctr;
							string counter = ss.str();
							for (int i = 0; i < 6-counter.size(); ++i)
							{
								text_record += "0";
							}
							text_record += counter;
							text_record += "1E";
							text_record_printer = true;
						}

						bool indexaddr = false;
						if(operand.find(",X") != string::npos){
							cout << "HEY !!\n";
						//index registering
							indexaddr = true;
						operand = operand.substr(0,operand.size()-2);
						}
						

						if(find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) != SYMTAB_name.end()){

							int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) - SYMTAB_name.begin();
							operand_address = *(SYMTAB_address.begin()+pos);

						}
						else {
							operand_address = "0";
							OPCODE_errors.push_back("Undefined symbol"+opcode);
						}
						//assemble the object code instruction
						cout << "ee" << operand;
						if(indexaddr){
							cout << "Krsna";
							stringstream ss;
							long temp = stoi(operand_address,nullptr,16);
							ss << dec << temp;
							temp = stoi(ss.str());
							cout << temp <<endl;
							ss.str("");
							cout << (long)pow(2,15) <<endl;
							ss << hex << (temp | (long)pow(2,15));
							operand_address = ss.str();
							cout << "Remember the operand_address ? " << operand_address; 
						}

					} else {
						//have to initialize tet record
						if(text_record_printer == false)
						{
							length_text_record(text_record);
							out << text_record << endl;
							text_record = "T";
							stringstream ss;
							ss << hex << locctr;
							string counter = ss.str();
							for (int i = 0; i < 6-counter.size(); ++i)
							{
								text_record += "0";
							}
							text_record += counter;
							text_record += "1E";
							text_record_printer = true;
						}

						operand_address = "0000";							
					}
						
					object_code += opcode_num;
					object_code += operand_address;
						// cout << "14.3" <<endl;


						// cout << "17"<<endl;

					write_the_text_record(text_record, object_code, out, locctr);
					locctr += 3;

				} else if( opcode == "WORD"){
				// constant to object code

						//converting to hexadecimal
							int num_dec = stoi(operand);
							std::stringstream ss;
						ss<< std::hex << num_dec; // int decimal_value
						std::string num_hex ( ss.str() );
						for(int i = num_hex.size();i < 6 ;i++) 
							object_code += "0";
						object_code += num_hex;
	
							write_the_text_record(text_record, object_code, out, locctr);
							// cout << "18"<<endl;
						locctr += 3;

				} else if(opcode == "BYTE"){
							string record;
							if(operand[0] == 'X'){
								record = operand.substr(2,operand.size()-3);
								// cout << "The operand is"+operand+"The record is ......"+record;
							}else if(operand[0] == 'C'){
								record = parse(operand);
							}
							object_code = record;
							string s = operand.substr(2, size-3);
							// cout << "19"<<endl;
							write_the_text_record(text_record, object_code, out, locctr);

							if(operand[0] == 'X'){
								locctr += (operand.size()-3)/2 ;
							} else {
								locctr += operand.size()-3;	
							}

						} else if(opcode == "RESB"){
							locctr += stoi(operand);
							text_record_printer = false;
						}else if(opcode == "RESW"){
							text_record_printer = false;
							locctr += stoi(operand) * 3;
						}
			// cout << "14.4" <<endl;

		}
			// cout << "14.5" <<endl;

		operand = "";
		opcode = "";
		label = "";
	}
out.close();
return 0;
}