#include "interpreter.h"
// #include "arithmetic.h"

using namespace std;

Interpreter::Interpreter(std::istream& in) {

    variables = new map<string, int>;
    arrays = new map<string, map<int, int>>;
    invalidNodes = new queue<NumericExpression*>;
    this->parse(in);
}

Interpreter::~Interpreter() {
	variables->clear();
	arrays->clear();
	delete variables;
    delete arrays;
    delete invalidNodes;
}

void Interpreter::parse(std::istream& in) {
    string line;
    while (getline(in, line)) {
    	string command;
        size_t line_number;
        stringstream stream(line);

        stream >> line_number;
        stream >> command;

        // cout << line_number << endl;

        if(command.compare("PRINT") == 0){ 
            // cout << "PRINTING" << endl;       	
            string temp, expression;
            stream >> expression;

            while(!stream.eof()){
                stream >> temp;
                expression += temp;
            }

            // cout << expression << endl;

            Command* print = new PrintCommand(parse_numeric_expression(expression, 0));
            commandline.push_back(print);
            commands.insert(pair<size_t, Command*>(line_number, print));

            // commandset.insert(make_pair(line_number, make_pair(print, "PRINT")));


        }else if(command.compare("LET") == 0){
            bool isArray = false;
            string temp, variableexpression, expression;
            NumericExpression *variable;
            NumericExpression* index1;

            stream >> temp;
            variableexpression += temp;
            for(int i = 0; i < (int)temp.size(); i++){
            	if(temp[i] == '['){
            		isArray = true;
            	}
            }

            // cout << isArray << endl;

            if(isArray){
                
                while(!stream.eof() && temp[temp.size()-1] != ']'){
                    stream >> temp;
                    if(temp[temp.size()-1] == ']'){
                        variableexpression += temp;
                        break;
                    }
                    variableexpression += temp;
                }

                variable = parse_variable_name(variableexpression, 0);
                index1 = getIndex(variableexpression, 0);
            }else{

                variable = parse_variable_name(variableexpression, 0);
                // cout << "Variable Expression: " << variableexpression << endl;

            }

            string name = getName(variableexpression, 0);

            stream >> expression;

            // cerr << expression << endl;

            while(!stream.eof()){
                stream >> temp;
                expression += temp;
            }

            NumericExpression* numexp = parse_numeric_expression(expression, 0);

            Command* letc;

            if(isArray){

                letc = new LetCommand(name, index1, variable, numexp, arrays);

            }else{

                letc = new LetCommand(name, variable, numexp, variables);

            }

            commandline.push_back(letc);
            commands.insert(pair<size_t, Command*>(line_number, letc));

            // commandset.insert(make_pair(line_number, make_pair(letc, "LET")));

        }else if(command.compare("IF") == 0){
            string temp, expression, linenumber;
            int split;
            NumericExpression* left;
            NumericExpression* right;
            NumericExpression* math;
            string op;

            stream >> expression;
            // cout << expression << endl;
            while(!stream.eof()){
                stream >> temp;
                // cout << temp << endl;
                if(temp.compare("THEN") == 0){
                    break;
                }
                expression += temp;
            }

            string leftside;
            string rightside;
            for(int i = 0; i < (int)expression.size(); i++){
                if(expression[i] == '>' || expression[i] == '<' || expression[i] == '='){
                    split = i+1;
                    op = expression[i];
                    while(split < (int)expression.size()){
                        	rightside += expression.at(split);
                        	split++;
                    }

                    break;
                }else{
                    leftside += expression.at(i);
                }
            }   

            // cerr << "Left: " << leftside << endl << "Right: " << rightside << endl;

            string func;
            stream >> func;
            while(!stream.eof()){
                stream >> temp;
                func += temp;
            }

            math = parse_numeric_expression(func, 0);

            left =  parse_numeric_expression(leftside, 0);

            right =  parse_numeric_expression(rightside, 0);

            BooleanVar* bool_exp = new BooleanVar(left, right, op);

            Command* ifer = new IfCommand(bool_exp, math);

            commandline.push_back(ifer);

            commands.insert(pair<size_t, Command*>(line_number, ifer));

            // commandset.insert(make_pair(line_number, make_pair(ifer, "IF")));
        }else if(command.compare("GOTO") == 0){

            int line;
            stream >> line;

            Command* c = new GoToCommand(line);
            commandline.push_back(c);
            commands.insert(pair<size_t, Command*>(line_number, c));

            // commandset.insert(make_pair(line_number, make_pair(c, "GOTO")));

        }else if(command.compare("GOSUB") == 0){

            int line;
            stream >> line;
            Command* c = new GoSubCommand(line);
            commandline.push_back(c);
            commands.insert(pair<size_t, Command*>(line_number, c));

            // commandset.insert(make_pair(line_number, make_pair(c, "GOSUB")));

        }else if(command.compare("RETURN") == 0){

            Command* c = new ReturnCommand();
            commandline.push_back(c);
            commands.insert(pair<size_t, Command*>(line_number, c));

            // commandset.insert(make_pair(line_number, make_pair(c, "RETURN")));


        }else if(command.compare("END") == 0){

            Command* c = new EndCommand();
            commandline.push_back(c);
            commands.insert(pair<size_t, Command*>(line_number, c));

            // commandset.insert(make_pair(line_number, make_pair(c, "END")));
        }
    }
}

void Interpreter::write(std::ostream& out) {
	/*for(int i = 0; i < (int)commandline.size(); i++){
        out << i+1 << " " << commandline[i]->format() << endl;
    }*/
    map<size_t, Command*>::iterator it = commands.begin();
    run(it);

    /*for(map<size_t, Command*>::iterator it = commands.begin(); it != commands.end(); ++it){
        // out << it->first << " " << it->second->format() << endl;
        int x = it->second->run();

    }*/

    for(int i = 0; i < (int)commandline.size(); i++){
        delete commandline[i];
    }
}

void Interpreter::run(map<size_t, Command*>::iterator it){

	// STORES ALL THE LINE NUMBERS THAT THE GOSUBS RESIDE ON SO IT CAN
	// BACKTRACK ONCE A GOSUB IS FOUND
    stack<int> gosubs;

    while(it != commands.end()){

    	// ALL COMMANDS HAVE A RUN FUNCTION THAT RETURNS AN INT
    	// IT IS USED FOR ERROR CHECKING OR LINE NUMBER DATA

        int x = it->second->run(invalidNodes);
        string identifier = it->second->to_string();

        if(x == -3){
			cout << "Error in Line " << it->first << ": Division by 0: " << invalidNodes->front()->handleError() << endl;

            return;
        }

        if(identifier.compare("IF") == 0){

            if(x == -9){
                ++it;
                continue;
            }

            it = commands.find(x);

            if(it == commands.end()){
                cout << "Error in Line " << it->first << ": IF jump to non-existent line " << x << "." << endl;
                return;
            }

            continue;

        }else if(identifier.compare("GOTO") == 0){

            it = commands.find(x);

            if(it == commands.end()){
                cout << "Error in Line " << it->first << ": GOTO to non-existent line " << x << "." << endl;
                return;
            }
            continue;

        }else if(identifier.compare("GOSUB") == 0){

            int lineno = it->first;
            it = commands.find(x);

            if(it == commands.end()){
                cout << "Error in Line " << lineno << ": GOSUB to non-existent line " << x << "." << endl;
                return;
            }

            gosubs.push(lineno);
            continue;

        }else if(identifier.compare("RETURN") == 0){

            if(gosubs.empty()){
                cout << "Error in Line " << it->first << ": No matching GOSUB for RETURN." << endl;
                return;
            }

            it = commands.find(gosubs.top());
            it++;
            gosubs.pop();

            continue;

        }else if(identifier.compare("END") == 0){
            return;
        }
        ++it;
    }
    return;
}

NumericExpression* Interpreter::parse_numeric_expression(std::string line, int position){
    // // cerr << "Parsing Inputs: " << line << " " << position << endl;
    if(isdigit(line[position]) || line[position] == '-'){
        // // cerr << "Found Constant!" << endl;
        return parse_constant(line, position);
    }

    if(line[position] >= 'A' && line[position] <= 'Z'){
        // // cerr << "Found Variable!" << endl;
        NumericExpression* name = parse_variable_name(line, position);
        return name;
    }

    if(line[position] == '('){
        // // cerr << "Found open bracket!" << endl;
        char op;
        position++;
        int nextpos = position;
        // // cerr << "Nextpos: " << nextpos << endl;
        int counter = 0;
        NumericExpression* left;
        NumericExpression* right;
        while(position < (int)line.size()){
            // INCREMENT DECREMENT PARANTHESIZES COUNTER
            if(line[position] == '('){
                counter++;
            }
 
            if(line[position] == ')'){
                counter--;
            }

            if(counter == 0 && (line[position] == '+' || line[position] == '-' || line[position] == '/' || line[position] == '*')){
                if(line[position] == '-' && line[position-1] == '('){
                	position++;
                	continue;
                }
                op = line[position];
                left = parse_numeric_expression(line, nextpos);
                right = parse_numeric_expression(line, (position + 1));
                break;

            }
            position++;
        }
        // HEAD RECURSION: ON THE WAY UP IT ALLOCATES OBJECTS
        if(op == '+'){
            return (new AdditionExpression(left, right));
        }else if(op == '-'){
            return (new SubtractionExpression(left, right));
        }else if(op == '*'){
            return (new MultiplicationExpression(left, right));
        }else if(op == '/'){
            return (new DivisionExpression(left, right, invalidNodes));
        }
    }
    return nullptr;
}

NumericExpression* Interpreter::parse_constant(string line, int position){

    int start = position;

    if(line[start] == '-'){
    	position++;
    }

    while(isdigit(line[position])){
        position++;
    }

    int end = position;
    int length = end - start;
    string c = line.substr(start, length);

    return (new ConstantExpression(c));
}

NumericExpression* Interpreter::parse_variable_name(string line, int position){
    int start = position;
    while(isalpha(line[position])){
        position++;
    }

    int end = position;
    int length = end - start;
    string name = line.substr(start, length);
    if(line[position] != '['){
        return (new IntegerVariable(name, variables));
    }else{
        position++;
        NumericExpression* index = parse_numeric_expression(line, position);
        position++;
        return (new ArrayVariable(name, index, arrays));
    }
}

string Interpreter::getName(string line, int position){
    int start = position;
    while(isalpha(line[position])){
        position++;
    }

    int end = position;
    int length = end - start;
    return line.substr(start, length);
}

NumericExpression* Interpreter::getIndex(string line, int position){
    while(isalpha(line[position])){
        position++;
    }
    position++;
    return parse_numeric_expression(line, position);
}