#include <string>
#include "arithmetic.h"

class BooleanVar{
public:
	BooleanVar(NumericExpression* left, NumericExpression* right, std::string symbol);
	virtual ~BooleanVar();
	virtual std::string format() const;
	virtual bool evaluate() const;

private:
	NumericExpression* left;
	NumericExpression* right;
	std::string symbol;
};

/*1 PRINT ((   X    +   5)      + 2)
2 PRINT (5+3)
3 PRINT (7 - ((5+3) + (2 / 7)))
4 LET X[(3-(2*(5+2)))]  (5+7)
5 LET X [  (     5+          7)       ] 2
6 LET X 5
7 PRINT ( 2 + 5)
8 PRINT ((2+5)  + 2)*/