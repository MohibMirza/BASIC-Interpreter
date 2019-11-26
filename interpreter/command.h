#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <sstream>
#include <iostream>
#include "arithmetic.h"
#include "booleanvar.h"

class Command {
public:
	Command();
	virtual ~Command();
    virtual std::string format() const = 0;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const = 0;
    virtual std::string to_string() const = 0;
};

class PrintCommand : public Command{
public:
	PrintCommand(NumericExpression* value);
	~PrintCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;

private:
	NumericExpression* value;
};

class IFCommand : public Command{
public:
	IFCommand();
	~IFCommand();

	virtual std::string format() const;
	virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
	virtual std::string to_string() const;
	
};

class LetCommand : public Command{
public:
	// LetCommand(NumericExpression* variable, NumericExpression* value);
	LetCommand(std::string varname, NumericExpression* variable, NumericExpression* value, std::map<std::string, int> *variables);
	LetCommand(std::string varname, NumericExpression* index, NumericExpression* variable, NumericExpression* value, std::map<std::string, std::map<int, int> > *arrays);	
	~LetCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;

private:
	std::string varname;
	NumericExpression* index;
	NumericExpression* variable;
	NumericExpression* value;
	bool isArray;
	std::map<std::string, int> *variables;
	std::map<std::string, std::map<int, int> > *arrays;

};

class IfCommand : public Command{
public:
	IfCommand(BooleanVar* booly, NumericExpression* nexp);
	~IfCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;

private:
	BooleanVar* booly;
	NumericExpression* nexp;
};

class GoToCommand : public Command{
public:
	GoToCommand(int linenumber);
	~GoToCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;

private:
	int linenumber;
};

class GoSubCommand : public Command{
public:
	GoSubCommand(int linenumber);
	~GoSubCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;

private:
	int linenumber;
};

class ReturnCommand : public Command{
public:
	ReturnCommand();
	~ReturnCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;
};

class EndCommand : public Command{
public:
	EndCommand();
	~EndCommand();

    virtual std::string format() const;
    virtual int run(std::queue<NumericExpression*> *invalidNodes) const;
    virtual std::string to_string() const;
};

#endif
