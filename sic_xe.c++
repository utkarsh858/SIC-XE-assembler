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

	vector <string> SYMTAB_name;
	vector <string> SYMTAB_address;
	vector <string> SYMTAB_error_flag;
	vector <string> OPCODE_errors;
	vector <string> OPTAB_name;
	vector <string> OPTAB_code;
	vector <string> OPTAB_type;

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
		cout << "=============================================================================" <<endl;
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

void initialize_OPTAB(){

	SYMTAB_name.push_back("A");	SYMTAB_name.push_back("X");	SYMTAB_name.push_back("L");	SYMTAB_name.push_back("B");	SYMTAB_name.push_back("S");	SYMTAB_name.push_back("T");	SYMTAB_name.push_back("F");
	SYMTAB_address.push_back("0");SYMTAB_address.push_back("1");SYMTAB_address.push_back("2");SYMTAB_address.push_back("3");SYMTAB_address.push_back("4");SYMTAB_address.push_back("5");SYMTAB_address.push_back("6");
	ifstream codes;
	codes.open("code.txt") ;
	string x;
	while(getline(codes, x)){
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
		OPTAB_name.push_back(args[0]);
		OPTAB_code.push_back(args[1]);
		OPTAB_type.push_back(args[2]);
	}

	// cout << "OPTAB initialized succesfully" << endl;
	codes.close();
}

void adjust_length(string& s, int l){
	if(s.size()<l)
		for (int i = 0; i <= l-s.size(); ++i)
			s= "0"+s;
	else s = s.substr(s.size()-l,l);

	
}

string parse(string s){
	// cout << "given string is :: " << s;
	string res;
	stringstream ss;
	for (int i = 2; i < s.size()-1; ++i)
	{
		ss << hex << (int)s[i];
		res += ss.str();
		ss.str("");
	}
	// cout <<  "Parsed string is :: "+res+"----";
	return res;
}

bool find_opcode(string opcode){
	// cout <<"Finding opcode" <<endl;
	if(opcode.find("+") != string::npos) opcode = opcode.substr(1,opcode.size()-1);
	return (find(OPTAB_name.begin(), OPTAB_name.end(), opcode) != OPTAB_name.end());
}

string find_diff(string a, string b){
	try{
		// cout <<a<<" " <<b;
	unsigned long i= (unsigned long)stoi(a,nullptr,16);
	unsigned long j= (unsigned long)stoi(b,nullptr,16);
	stringstream ss;
	ss<< hex<<(i-j);
	return ss.str();
	}catch(out_of_range){
		cout << "LOL";
		exit(1);
	}
}


string make_my_code(string opcode, string operand,long locctr,string B){
	//special cases
	if(opcode == "RSUB") return "4F0000";

	stringstream ss;

	cout <<"making the code"<<endl;
	bool indirect = false;
	bool immediate = false;
	bool index = false;
	bool extended = false;
	// cout << "pop1" <<endl;

	// cout << "pop2" <<endl;

	string operand_addr;

	//making query string
	if(operand.find("@") != string::npos){ //indirect
		operand = operand.substr(1,operand.size());
		indirect = true;
	} else if(operand.find("#") != string::npos){// immediate
		operand = operand.substr(1,operand.size());
		immediate = true;
	} else { //direct
		if(operand.find(",X") != string::npos){
			operand = operand.substr(0,operand.size()-2);
			index = true;
		}
	} 
	if(opcode.find("+") != string::npos){
		opcode = opcode.substr(1,opcode.size());
		extended = true;
		locctr++; //since only 3 has been increased originally
	}

	int pos = find(OPTAB_name.begin(), OPTAB_name.end(), opcode) - OPTAB_name.begin();
	string opcode_addr = *(OPTAB_code.begin() + pos);
	string format = *(pos+OPTAB_type.begin());

	ss.str("");
	ss << hex << locctr;
	cout << "locctr "<<ss.str();
	long pc = locctr + stoi(format);

	// cout << "pop3" <<endl;

	//some variations defined for changing the value of n,i,x,b,p,e
	int ni_tweak = 0 | (int)(immediate) | ((int)indirect << 1);
	if(!immediate && !indirect) ni_tweak = 3;
	int middle = 0 | ((int)index << 3) ;
	bool number_operand = (int)operand[0] <= 57 && (int)operand[0] >= 48;
	int cutout = (extended)? 5:3;
	middle |= (int)extended;
	cout << middle;
	// cout << "pop4 " << "format : "<< format<<endl;
	cout << " ni_tweak: "<<ni_tweak;
	cout << " format " << format <<endl;
	if(format == "3"){
	if(immediate && number_operand ){
		//immediate handling
			// cout << "Hari bol!!" <<endl;
			ss.str("");
			ss << hex << stoi(operand);
			operand_addr = ss.str();
			cout << operand_addr << "iii"<<endl;
			cout << middle << "ooo"<<endl;
			adjust_length(operand_addr, cutout);

			ss.str("");
			ss << hex << middle;

			operand_addr = ss.str()+operand_addr;
			
			//opcode adjustment
			ss.str("");
			int p = stoi(opcode_addr,nullptr,16);
			ss << dec << p <<endl;
			long temp = (stoi(ss.str()) | ni_tweak);
			ss.str("");
			ss << hex << temp;
			opcode_addr = ss.str();
			adjust_length(opcode_addr,2);
			cout << "Shout!"+opcode_addr<<endl;

	} else 

	// if(indirect || direct || (immediate && !number_operand))
	{
			pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand) - SYMTAB_name.begin();
			operand_addr = *(SYMTAB_address.begin() + pos);

			bool use_base = false;
			if(!extended){
			// have to take complement also !!!!
			ss.str("");ss << hex << pc;
			// cout <<"fighting maya 1"<<endl;
			cout << "label addr"<<operand_addr << "pc"<<ss.str()<<endl;
			string disp = find_diff(operand_addr, ss.str());
			
			//now have to check whether the displacement is less than 12 bits or not
			// cout <<"fighting maya 2"<<"displacement is"<< operand_addr<<endl;
			cout << "displacement is" << disp <<endl;
			if(disp.size() > 3)
			{
				cout <<"How???" <<endl;
				//disp[disp.size()-4] .. disp[0]
				if(disp[0] != 'f'){
					cout << "positive checking";
					for(int i= disp.size()-4; i>=0;i--) if(disp[i]!='0') {use_base = true;break;}
				}
				else{
					cout << "complement checking";
					for(int i= disp.size()-4; i>=0;i--) if(disp[i]!='f') {use_base = true;break;}
				}

			}
			adjust_length(disp, 3);
			// cout <<"fighting maya 4"<<endl;

			//if have to use base
			cout << use_base <<endl;
			if(use_base){
				// cout << "Have to use base." <<endl;
				middle |= 4;
				operand_addr = find_diff(operand_addr, B);
			} else 
			{middle |= 2;
			operand_addr = disp;
			}

			} //!extended end
			//cutout 
			adjust_length(operand_addr, cutout);
			cout <<"middle"<<middle<<endl;		
			ss.str("");
			ss << hex << middle;	
			operand_addr = ss.str()+operand_addr;
			
			// cout <<"fighting maya 3   "<< opcode_addr<<endl;
			// cout << opcode_addr +"string";
					//now for the opcode
			ss.str("");
			int p = stoi(opcode_addr,nullptr,16);
			ss << dec << p <<endl;
			long temp = (stoi(ss.str()) | ni_tweak);
			ss.str("");
			ss << hex << temp;
			opcode_addr = ss.str();
			if(opcode_addr.size() == 1) opcode_addr = "0"+opcode_addr;
			// cout << "fighting maya 5"<<endl;
			cout << opcode_addr.size() ;
			cout << " OPcode_addr "<< opcode_addr<<" OPerand addr "<< operand_addr <<endl;
		}
	} else if(format == "2"){
		if(operand.size() == 3)
			{
				int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand.substr(0,1)) - SYMTAB_name.begin();
				string reg1 = *(pos + SYMTAB_address.begin());
				int pos2 = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand.substr(2,1)) - SYMTAB_name.begin();
				string reg2 = *(pos + SYMTAB_address.begin());
				operand_addr = reg1 + reg2;
				
			}else{
				int pos = find(SYMTAB_name.begin(), SYMTAB_name.end(), operand.substr(0,1)) - SYMTAB_name.begin();
				string reg1 = *(pos + SYMTAB_address.begin());
				operand_addr = reg1 + "0";
			}
	} else{
		operand_addr = "0000";
	}
	// cout << "pop5" <<endl;
	string res = (opcode_addr + operand_addr);
	cout << "Manufactured code-----"+res;
	
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

	string text_record;
	string base_register;
	bool text_record_printer = true;

	initialize_OPTAB();

	string opcode = "";
	string operand = "";
	string label = "";

	//Pass 1
	while (getline(in, x)){
		// cout << "Got this::: " << x;
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
			cout << locctr << endl;
		if(args[0][0] != '.'){  // comment handling
			if(opcode == "START"){ // strting address

				starting_addr = stoi(operand,nullptr,16);
				

				locctr = starting_addr;
				base_register = starting_addr;
				// cout << "5" << endl;

			} else 
			if(opcode == "BASE")
				{
					//ignore
				} else
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
						
						stringstream ss ;
						ss << hex << locctr;
						SYMTAB_address.push_back(ss.str());
						SYMTAB_error_flag.push_back("");
					}
					// cout << "7"<<endl;

				}

				if(find_opcode(opcode)){ // searching for optab
									// cout << "mast\n";
 
					if(opcode.find("+") != string::npos){
						opcode = opcode.substr(1,opcode.size());
						locctr++; //since only 3 has been increased originally
					}

					int pos = find(OPTAB_name.begin(), OPTAB_name.end(), opcode) - OPTAB_name.begin();
					string format = *(pos+OPTAB_type.begin());

					locctr += stoi(format);   //have to see which type of opcode
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
					cout << "15"<<endl;

				}
				else
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
				else if(opcode == "BASE")
				{
					int pos = find(SYMTAB_name.begin() ,SYMTAB_name.end() , operand) - SYMTAB_name.begin();
					base_register = *(pos + SYMTAB_address.begin()); 
					cout << "BASE set ! " << base_register <<endl;
				} else


				if(find_opcode(opcode)){
					if(text_record_printer = false){
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

					cout << "PMT incharge"<<endl;
					string query = (opcode.find("+")!= string::npos)?opcode.substr(1,opcode.size()-1):opcode;
					int pos = find(OPTAB_name.begin(), OPTAB_name.end(), query) - OPTAB_name.begin();
					string format = *(pos+OPTAB_type.begin());
					string code = make_my_code(opcode, operand, locctr, base_register);
					write_the_text_record(text_record ,code , out, locctr);
					if(opcode.find("+") != string::npos) locctr++;
					if(format == "3") locctr+=3;
					else if(format == "2") locctr += 2;
					else if(format == "1") locctr += 1;



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
							cout << "18"<<endl;
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