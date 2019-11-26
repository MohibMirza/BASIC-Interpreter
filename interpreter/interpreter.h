#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "arithmetic.h"
#include "command.h"
#include <map>
#include <stack>
#include <queue>

class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();

    void write(std::ostream& out);
    void run(std::map<size_t, Command*>::iterator it);
    NumericExpression* parse_numeric_expression(std::string line, int position);
    NumericExpression* parse_constant(std::string line, int position);
    NumericExpression* parse_variable_name(std::string line, int position);

private:
    void parse(std::istream& in);
    std::string getName(std::string line, int position);
    NumericExpression* getIndex(std::string line, int position);
    std::vector<Command*> commandline;
    std::map<size_t, Command*> commands; 
    // std::map<size_t, std::pair<Command*, std::string> > commandset;
    std::map<std::string, int> *variables; 
    std::map<std::string, std::map<int, int> > *arrays; 
    std::queue<NumericExpression*> *invalidNodes;
};

#endif