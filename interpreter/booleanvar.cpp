#include "booleanvar.h"

using namespace std;

BooleanVar::BooleanVar(NumericExpression* left, NumericExpression* right, std::string symbol) : left(left), right(right), symbol(symbol){

}

BooleanVar::~BooleanVar(){
	delete this->left;
	delete this->right;
}

string BooleanVar::format() const{
	if(symbol == ">"){
		return right->format() + " < " + left->format();
	}
	return left->format() + " " + symbol + " " + right->format();
	
}

bool BooleanVar::evaluate() const{
	if(symbol.compare(">") == 0){
		return (left->evaluate() > right->evaluate());
	}else if(symbol.compare("<") == 0){
		return (left->evaluate() < right->evaluate());
	}else if(symbol.compare("=") == 0){
		return (left->evaluate() == right->evaluate());
	}
	return false;
}