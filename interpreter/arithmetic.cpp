#include "arithmetic.h"

using namespace std;

string NumericExpression::handleError() {
    return "";
}

IntegerVariable::IntegerVariable(std::string name, std::map<string, int> *variables) : name(name), variables(variables){

}

IntegerVariable::~IntegerVariable(){

}

string IntegerVariable::format() const{
	return name;
}

int IntegerVariable::evaluate(){
    /*map<string, IntegerVariable*>::iterator it;

    it = variables->find(name);
    if(it != variables->end()){
        return it->second->evaluate();
    }*/
    map<string, int>::iterator it;

    it = variables->find(name);

    if(it != variables->end()){
        return it->second;
    }
    return 0;
}

ArrayVariable:: ArrayVariable(std::string name, NumericExpression* index, std::map<string, map<int, int>> *arrays) : name(name), index(index), arrays(arrays){

}

ArrayVariable::~ArrayVariable(){

}

string ArrayVariable::format() const{
	return name + "[" + this->index->format() + "]";
}

int ArrayVariable::evaluate(){

    map<string, map<int, int> >::iterator it;

    it = arrays->find(name);
    if(it != arrays->end()){
        map<int, int>::iterator it2;

        it2 = it->second.find(index->evaluate());

        if(it2 != it->second.end()){
            return it2->second;
        }

    }

    return 0;
}

AdditionExpression::AdditionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

AdditionExpression::~AdditionExpression() {
    delete this->left;
    delete this->right;
}

string AdditionExpression::format() const {
    return "(" + this->left->format() + " + " + this->right->format() + ")";
}

int AdditionExpression::evaluate(){
    return this->left->evaluate() + this->right->evaluate();
}

SubtractionExpression::SubtractionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

SubtractionExpression::~SubtractionExpression() {
    delete this->left;
    delete this->right;
}

string SubtractionExpression::format() const {
    return "(" + this->left->format() + " - " + this->right->format() + ")";
}

int SubtractionExpression::evaluate(){
    return left->evaluate() - right->evaluate();
}

MultiplicationExpression::MultiplicationExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

MultiplicationExpression::~MultiplicationExpression() {
    delete this->left;
    delete this->right;
}

string MultiplicationExpression::format() const {
    return "(" + this->left->format() + " * " + this->right->format() + ")";
}

int MultiplicationExpression::evaluate(){
    return left->evaluate() * right->evaluate();
}

DivisionExpression::DivisionExpression(NumericExpression* left, NumericExpression* right, queue<NumericExpression*> *invalidNodes) : left(left), right(right), invalidNodes(invalidNodes) {

}

DivisionExpression::~DivisionExpression() {
    delete this->left;
    delete this->right;
}

string DivisionExpression::format() const {
    return "(" + this->left->format() + " / " + this->right->format() + ")";
}

int DivisionExpression::evaluate(){

    bool isValid = !(right->evaluate() == 0);

    if(isValid){
    	// cout << "LEFT: " << left->evaluate() << " RIGHT " << right->evaluate() << endl;
        return ((int)(left->evaluate() / right->evaluate()));
    }
    // cout << "Invalid Node" << endl;
    if(invalidNodes->empty()){
   		invalidNodes->push(this);
	}
    return 0;
}

string DivisionExpression::handleError(){
    return left->format() + " = " + to_string(left->evaluate()) +  ", " + right->format() + " = " + to_string(right->evaluate()) + ".";
}

ConstantExpression::ConstantExpression(string val) {
	value = val;
}

ConstantExpression::~ConstantExpression() {}

string ConstantExpression::format() const {
    return value;
}

int ConstantExpression::evaluate(){
    stringstream s(value);
    int x = 0;
    s >> x;
    return x;   
}

