#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <queue>

class NumericExpression {
public:
    virtual ~NumericExpression() {}
    virtual std::string format() const = 0;
    virtual int evaluate() = 0;
    virtual std::string handleError();

private:
    bool valid;
    NumericExpression* invalidNode;
};

class IntegerVariable : public NumericExpression {
public:
	IntegerVariable(std::string name ,std::map<std::string, int> *variables);
	~IntegerVariable();

	virtual std::string format() const;
    virtual int evaluate();
    /*void setValue(int val);
    int getValue() const;*/

private:
	std::string name;
    std::map<std::string, int> *variables; 
};

class ArrayVariable : public NumericExpression {
public:
	ArrayVariable(std::string name, NumericExpression* index, std::map<std::string, std::map<int, int> > *arrays);
	~ArrayVariable();

	virtual std::string format() const;
    virtual int evaluate();

private:
	std::string name;
	NumericExpression* index;
    std::map<std::string, std::map<int, int> > *arrays; 
};

class AdditionExpression : public NumericExpression {
public:
    AdditionExpression(NumericExpression* left, NumericExpression* right);
    ~AdditionExpression();

    virtual std::string format() const;
    virtual int evaluate();

private:
    NumericExpression* left;
    NumericExpression* right;
};

class SubtractionExpression : public NumericExpression {
public:
    SubtractionExpression(NumericExpression* left, NumericExpression* right);
    ~SubtractionExpression();

    virtual std::string format() const;
    virtual int evaluate();

private:
    NumericExpression* left;
    NumericExpression* right;
};

class MultiplicationExpression : public NumericExpression {
public:
    MultiplicationExpression(NumericExpression* left, NumericExpression* right);
    ~MultiplicationExpression();

    virtual std::string format() const;
    virtual int evaluate();

private:
    NumericExpression* left;
    NumericExpression* right;
};

class DivisionExpression : public NumericExpression {
public:
    DivisionExpression(NumericExpression* left, NumericExpression* right, std::queue<NumericExpression*> *invalidNodes);
    ~DivisionExpression();

    virtual std::string format() const;
    virtual int evaluate();
    virtual std::string handleError();

private:
    NumericExpression* left;
    NumericExpression* right;
    std::queue<NumericExpression*> *invalidNodes;
};

class ConstantExpression : public NumericExpression {
public:
    ConstantExpression(std::string str);
    ~ConstantExpression();

    virtual std::string format() const;
    virtual int evaluate();

private:
	std::string value;
	int integervalue;
};

#endif
