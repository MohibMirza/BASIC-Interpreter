#include "command.h"

using namespace std;

Command::Command() {}

Command::~Command() {}

PrintCommand::PrintCommand(NumericExpression* value) : value(value){

}

PrintCommand::~PrintCommand() {
	if(value != nullptr){
		delete value;
	}
}

string PrintCommand::format() const{
	return "PRINT " + value->format();
}

int PrintCommand::run(std::queue<NumericExpression*> *invalidNodes) const{

	int s = value->evaluate();

	if(invalidNodes->empty()){
		cout << s << endl;
		return -2;
	}
	return -3;
}

string PrintCommand::to_string() const{
	return "PRINT";
}

LetCommand::LetCommand(string varname, NumericExpression* variable, NumericExpression* value, map<string, int> *variables) : varname(varname), variable(variable), value(value), variables(variables) {
	isArray = false;
}

LetCommand::LetCommand(string varname, NumericExpression* index, NumericExpression* variable, NumericExpression* value, map<string, map<int, int>> *arrays) : varname(varname), index(index), variable(variable), value(value), arrays(arrays) {
	isArray = true;
}

LetCommand::~LetCommand(){
	if(index != nullptr){
		delete index;
	}
	if(variable != nullptr){
		delete variable;
	}
	if(value != nullptr){
		delete value;
	}
}

string LetCommand::format() const{
	return "LET " + variable->format() + " " + value->format();
}

int LetCommand::run(std::queue<NumericExpression*> *invalidNodes) const{

	if(isArray){

		int x = index->evaluate();
		int y = value->evaluate();

		if(invalidNodes->empty()){
			(*arrays)[varname][x] = y;
		}else{
			return -3;
		}
	}else{

		if(invalidNodes->empty()){
			(*variables)[varname] = value->evaluate();
		}else{
			return -3;
		}
	}
	return -6;
}

string LetCommand::to_string() const{
	return "LET";
}

IfCommand::IfCommand(BooleanVar* booly, NumericExpression* nexp) : booly(booly), nexp(nexp) {

}

IfCommand::~IfCommand(){
	if(booly != nullptr){
		delete booly;
	}

	if(nexp != nullptr){
		delete nexp;
	}
}

string IfCommand::format() const{

	return "IF [" + booly->format() + "] THEN <" + nexp->format()+ ">";
}

int IfCommand::run(std::queue<NumericExpression*> *invalidNodes) const{

	bool condition = booly->evaluate();
	int x = nexp->evaluate();

	if(!invalidNodes->empty()){
		return -3;
	}

	if(condition){
		return x;
	}

	return -9;
}

string IfCommand::to_string() const{
	return "IF";
}

GoToCommand::GoToCommand(int linenumber) : linenumber(linenumber){

}

GoToCommand::~GoToCommand(){

}

string GoToCommand::format() const{
	ostringstream s;
	s << "GOTO " << "<" << linenumber << ">";
	string ss = string(s.str());
	return ss;
}

int GoToCommand::run(std::queue<NumericExpression*> *invalidNodes) const{
	return linenumber;
}

string GoToCommand::to_string() const{
	return "GOTO";
}	

GoSubCommand::GoSubCommand(int linenumber) : linenumber(linenumber){

}

GoSubCommand::~GoSubCommand(){

}

string GoSubCommand::format() const{
	ostringstream s;
	s << "GOSUB " << "<" << linenumber << ">";
	string ss = string(s.str());
	return ss;
}

int GoSubCommand::run(std::queue<NumericExpression*> *invalidNodes) const{
	return linenumber;
}

string GoSubCommand::to_string() const{
	return "GOSUB";
}

ReturnCommand::ReturnCommand(){

}

ReturnCommand::~ReturnCommand(){

}

int ReturnCommand::run(std::queue<NumericExpression*> *invalidNodes) const{
	return -2;
}

string ReturnCommand::format() const{
	return "RETURN";
}

string ReturnCommand::to_string() const{
	return "RETURN";
}

EndCommand::EndCommand(){

}

EndCommand::~EndCommand(){

}

string EndCommand::format() const{
	return "END";
}

int EndCommand::run(std::queue<NumericExpression*> *invalidNodes) const{
	return -2;
}

string EndCommand::to_string() const{
	return "END";
}