#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>

#include "test_lexer.cpp"

using namespace std;

bool checkStatement();
bool checkAssign();
bool checkDeclarative();

bool checkExpression();
bool checkExpressionPrime();
bool checkTerm();
bool checkTermPrime();
bool checkFactor();

bool checkIdentifier();
bool checkType();

// Acknowledge if this is a statement or not
// DEFAULT: false
bool duringStatement = false;
bool isLeftMost, isRightMost;

Token tmp;
int prevToken, nextToken;
// A vector to hold the tokens
vector<Token> temp, tokens;
vector<vector<Token>> tVec;
// Token token;
int token_ptr;

// Error Check
template<int N>
  class Error {
  public:
    explicit Error(const std::string s) : message{s} { }

    std::string get_message() const { return message; }
    void put(std::ostream& os) const { os << message; }

  private:
    std::string message;
  };

using Lexical_error = Error<1>;
using Syntax_error = Error<2>;
using Runtime_error = Error<3>;

template<int N>
  std::ostream& operator<<(std::ostream& os, const Error<N>& e)
  {
    e.put(os);
    return os;
  }

// Auxiliry functions
void advance(int i = 1)
{
    switch (i)
    {   
        case -2 :
            token_ptr = temp.size() + 1;
            return;

        case 0 :
            token_ptr = 0;
            prevToken = (token_ptr < 1 ? 0 : prevToken - 1);
            nextToken = (token_ptr > temp.size() - 2 ? temp.size() - 1 : nextToken + 1);
            tmp.setPrio();
            temp[prevToken].setPrio();
            temp[nextToken].setPrio();
            return;
    

        case 1 :
    
            temp[token_ptr] = tmp;

            if (token_ptr < temp.size() - 1)
            {
            prevToken = token_ptr;
            token_ptr += 1;
            tmp = temp[token_ptr];
            nextToken = (token_ptr < temp.size() - 1 ? token_ptr + 1 : nextToken);
            tmp.setPrio();
            temp[prevToken].setPrio();
            temp[nextToken].setPrio();
            return;
            }
            else
            {
                token_ptr++;
            }
        
        default :
            return;
    }
    int k = temp.size() - i;
    
    if (token_ptr > k - 2 || token_ptr < i - 1)
    {
        return;
    }

    if (token_ptr >= i)
    {
        prevToken += i;
    }
    if (token_ptr <= k)
    {
        nextToken += i;
    }
    token_ptr += i;
    tmp = temp[token_ptr];

}
void moveup()
{
    if (token_ptr < temp.size())
    {
        prevToken = token_ptr;
        token_ptr += 1;
        tmp = temp[token_ptr];
        nextToken = (token_ptr < temp.size() - 1 ? token_ptr + 1 : nextToken);
    }
};

void backdown()
{
    if (token_ptr > 0)
    {
        nextToken = token_ptr;
        token_ptr -= 1;
        tmp = temp[token_ptr];
        prevToken = (token_ptr > 0 ? token_ptr - 1 : prevToken);
    }
};

// Check if a string is numeric
bool isNumeric(string &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

    // Money sign to mark the end of the file
    Token moneySign;
    moneySign.lexemeNum = 4;
    moneySign.lexeme = '$';
    moneySign.lexemeName = "SEPERATOR";

    // Read the file
    cout << "\nPlease enter the name of the file (no need .txt): ";
    getline(cin, fileName);
    fileName = fileName + ".txt";

    // Open the file
    infile.open(fileName.c_str());

    // Error shooting if the file cannot be opened
    if (infile.fail())
    {
        cout << "\n** ERROR - the file \"" << fileName << "\" cannot be found!\n\n";
        exit(1);
    }

    

    // Determine each token in the line
    while (getline(infile, line))
    {
        temp = lexer(line);       
        if (temp.size() == 0) // empty line skip
        {
            temp = lexer(line);
        }
        

        tVec.push_back(temp);
        cout << "\ntVec's size = " << tVec.size() << "\n";
        cout << "\nToken Line : ";
        for (Token tk : temp)
        {
            cout << tk.lexeme;
        }   
        
    }   
    temp.clear();
    temp.push_back(moneySign);
    cout << "\ntVec's size = " << tVec.size() << "\n";
    tVec.push_back(temp);
    cout << "\ntVec's size = " << tVec.size() << "\n";
    cout << "\nToken Line : ";
    for (Token tk : temp)
    {
        cout << tk.lexeme;
    }   

    // Close the file
    infile.close();
    infile.clear();
    
    for (auto tLine : tVec) 
    { 
        //tokens.insert(tokens.end(), temp.begin(), temp.end());
        temp = tLine;
        int line_size = temp.size();
        token_ptr = 0;
        cout << "\n";
        bool ass = false;           // statement assigned yes or no
        int insidetrack = 0;        // parentheses balance
        advance(0);
        // Parse the tree (line)
        while (token_ptr < line_size)
        {
            // Skip comment section
            while (temp[token_ptr].lexemeNum == 6)
            {
                advance();
                
            };
            if (token_ptr == line_size) {advance(-2); break;}
            cout << "Token ptr = " << token_ptr << "\n";
            cout << "Token size = " << line_size << "\n";
            tmp = temp[token_ptr];
            tmp.index = token_ptr;
            
            isLeftMost = (token_ptr == 0 ? true : false);
            
            isRightMost = (token_ptr == line_size - 1 ? true : false);

            cout << "TOKEN_PTR = " << token_ptr << endl;
            cout << "TOKEN SIZE = " << line_size << endl;
            cout << "PREV TOKEN IS   :   " << temp[prevToken].lexeme << "    WITH PRIORITY : " << temp[prevToken].prio << endl;
            cout << "CURRENT TOKEN IS   :   " << temp[token_ptr].lexeme << "    WITH PRIORITY : " << temp[token_ptr].prio << endl;
            cout << "NEXT TOKEN IS   :   " << temp[nextToken].lexeme << "    WITH PRIORITY : " << temp[nextToken].prio << endl;
            cout << "assigned is currently  " << boolalpha << ass << endl;

            // Display the token
            cout << left << setw(5) << "Token: " << setw(15) << tmp.lexemeName
                << setw(5) << "Lexeme: " << tmp.lexeme << endl;

            cout << "===========BEFORE THE LOOP===========" << endl;
            cout << "duringStatement is " << (duringStatement ? "true" : "false") << endl;

            if (isLeftMost || isRightMost || tmp.prio < 0)
            {
                if (!tmp.isTerminal()) 
                {
                    cout << "Syntax Error : non-terminal token could not be resolved\n";
                    throw Syntax_error("Syntax Error : non-terminal token could not be resolved\n"); 
                }
            }

            if (isalnum(tmp.lexeme[0]))
            {
                tmp.setTerminal(true);
            }

            switch (tmp.prio)   // Token's priority based on type
            {
                case 10 : // alnum; lowest priority
                    cout << "switch case 10 entered \n";
                    tmp.setTerminal(true);
                    if (!ass)
                    {
                        tmp.assignExpr("<Statement> -> <Assign>", 0);
                        ass = true;
                        if (temp[nextToken].prio == 0 )
                        {
                            tmp.assignExpr("<Assign> ->  <Identifier>  = <Expression> ;", 1);
                            ass = true;
                        }
                    }
                    else
                    {
                        cout << "switch case inside case 10\n";
                        tmp.assignExpr("<Expression> -> <Term> <Expression Prime>", 1);
                        switch (temp[nextToken].prio)
                        {
                            case 0 :
                                //throw Syntax_error("Syntax Error : multiple = signs in the same line\n");
                                tmp.assignExpr("<ID> -> "+tmp.lexeme, 6);
                                advance();
                                continue;

                            case 1 : // next is $
                                cout << "next token is suspected end of file\n";
                                tmp.assignExpr("<ID> -> "+tmp.lexeme, 6);
                                advance();
                                continue;

                            case 2 : // right parentheses
                                if (insidetrack < 1)
                                {
                                    throw Syntax_error("Syntax Error : no left parentheses before right! \n"); 
                                }
                                //insidetrack--;
                                break;
                            
                            case 3 : // left parentheses

                                //insidetrack++;
                                break;

                            case 4 : // multdiv
                                tmp.assignExpr("<Term>  -> <Factor> <Term Prime>", 2);
                                advance();
                                continue;
                                //break;

                            case 5 : // addsub
                                cout << "addsub subcase 5 entered \n";
                                tmp.assignExpr("<Term>  -> <Factor> <Term Prime>", 2);
                                advance();
                                continue;
                                //break;
                                
                            case 6 :
                                cout << "next token is expected end of line ;\n";
                                tmp.assignExpr("<ID> -> "+tmp.lexeme, 6);
                                advance();
                                continue;

                            default :
                                if (nextToken != token_ptr)
                                {
                                    throw Syntax_error("Syntax Error : consecutive ID tokens encountered without operators \n"); 
                                }
                                break;
                        }
                    }
                    
                    cout << "switch case 10 exiting \n";
                    advance();
                    continue;
                    
                case 0 : // = sign
                    if (isLeftMost || isRightMost)
                    {
                        cout << "Syntax Error : equals sign at the beginning or end of a line is forbidden \n";
                        throw Syntax_error("Syntax Error : non-terminal token could not be resolved\n"); 
                    }
                    if (ass && temp[prevToken].prio == 0)
                    {
                        cout << "Multiple EQUALITY/COMPARISON OPERATORS in the same line is disallowed";
                        throw Syntax_error("Multiple  = < > >= <= in the same line\n");
                    }
                    advance();
                    continue;
                    
                case 1 : // $
                    cout << "token is suspected end of file\n";
                    advance();
                    break;

                case 2 : // close parentheses
                    if (isLeftMost)
                    {
                        throw Syntax_error("Syntax Error : illegal grammar - first token of line is closed parenthesis! \n"); 
                    }
                    if (temp[prevToken].prio != 10)
                    {
                        if (temp[prevToken].prio == 3)
                        {
                            throw Syntax_error("Syntax Error : illegal grammar - nothing inside parentheses! \n"); 
                        }
                        if (temp[prevToken].prio != 2)
                        {
                            throw Syntax_error("Syntax Error : non-identifier non-closed parentheses preceding current token! \n"); 
                        }
                    }
                    insidetrack--;
                    if (temp[prevToken].prio == temp[nextToken].prio && temp[prevToken].prio != 10)
                    {
                        if ( (temp[nextToken].prio) || (!insidetrack && (temp[prevToken].prio != 3 || temp[nextToken].prio != 2)) )
                        {
                            cout << "Possible illegal parenthetical statement found\n";
                        }
                    }
                    if (isRightMost && insidetrack != 0)
                    {
                        throw Syntax_error("Syntax Error : unbalanced parentheses, now shutting down \n"); 
                    }
                    tmp.assignExpr("<Term Prime> -> epsilon ", 1);
                    tmp.assignExpr("<Term>  -> <Factor> <Term Prime>", 2);
                    break;
                            
                case 3 : // open parentheses
                    if (isRightMost)
                    {
                        throw Syntax_error("Syntax Error : illegal grammar - last token is open parenthesis! \n"); 
                    }
                    insidetrack++;
                    if (isRightMost && insidetrack != 0)
                    {
                        throw Syntax_error("Syntax Error : unbalanced parentheses, now shutting down \n"); 
                    }
                    if (temp[nextToken].prio != 10)
                    {
                        if (temp[nextToken].prio == 2)
                        {
                            throw Syntax_error("Syntax Error : illegal grammar - nothing inside parentheses! \n"); 
                        }
                        if (temp[nextToken].prio != 3)
                        {
                            throw Syntax_error("Syntax Error : non-identifier non-open parentheses after open parentheis! \n"); 
                        }
                    }
                    tmp.assignExpr("<Term Prime> -> epsilon ", 1);
                    tmp.assignExpr("<Term>  -> <Factor> <Term Prime>", 2);
                    break;

                case 4 : // multdiv
                    if (isLeftMost || isRightMost)
                    {
                        throw Syntax_error("ERROR (operator at begininning or end of line");
                    }
                    
                    if (temp[prevToken].prio != 10 || temp[nextToken].prio != 10)
                    {
                        if (temp[nextToken].prio == 2)
                        { 
                            throw Syntax_error("Bad Syntax : multiply/divide operator before closing parentheses");
                        }
                        if (temp[prevToken].prio == 3 && temp[nextToken].prio == 2)
                        {
                            throw Syntax_error("Bad Syntax : parentheses enclosing operators");
                        }
                        if (temp[prevToken].prio == temp[nextToken].prio && temp[prevToken].prio != 3)
                        {
                            throw Syntax_error("Invalid operator syntax");
                        }
                    }
                    if (temp[prevToken].prio != 10)
                    {

                    }
                    if (temp[prevToken].prio != 10 || temp[nextToken].prio != 10)
                    {
                        if ( (temp[prevToken].prio == temp[nextToken].prio) || (!insidetrack && (temp[prevToken].prio != 3 || temp[nextToken].prio != 2)) )
                        {
                            cout << "Possible illegal parenthetical statement found\n";
                        }
                    }
                    //int i = temp[prevToken].prio;
                    //int j = temp[nextToken].prio;
                    /*if (i != 10 || j != 10)
                    {
                        if ( (i == j) || (!insidetrack && (i != 3 || j != 2)) )
                        {
                            cout << "Possible illegal parenthetical statement found\n";
                        }
                    }*/
                    tmp.assignExpr("<Term>  -> <Factor> <Term Prime>", 2);
                    advance();
                    continue;

                case 5 :    // +/-
                    tmp.assignExpr("<Term Prime> -> epsilon ", 1);
                    tmp.assignExpr("<Expression Prime> -> + <Term> <Expression Prime> ", 2);
                    advance();
                    continue;
                                
                case 6 :    // semicolor
                    cout << "semicolon (should be end of line)\n";
                    if (nextToken > token_ptr)
                    {                        
                        throw Syntax_error("Syntax Error : critical error - line continues after semicolon \n"); 
                    }
                    if (insidetrack)
                    {
                        throw Syntax_error("Syntax Error : fatal error - unequal number of '(' and ')' in expression \n"); 
                    }                    
                    tmp.assignExpr("<Term Prime> -> epsilon ", 1);
                    tmp.assignExpr("<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>", 2);
                    tmp.assignExpr("<Term Prime> -> epsilon ", 3);
                    tmp.assignExpr("<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <ExpressionPrime> | <Empty>  ", 4);
                    tmp.assignExpr("<Term Prime> -> epsilon ", 5);
                    break;


                default :
                    cout << "Invalid token not resolved! \n";
                    throw Syntax_error("Syntax Error : invalid token found, your system is now terminating\n"); 
                    break;
            }

            /*if (tmp.lexeme == ";")
            {
                duringStatement = false;
            }
            else if (duringStatement)
            {
                checkExpression();
            }
            else if (!duringStatement)
            {
                if (checkStatement())
                {
                    duringStatement = true;
                }
            };*/

            cout << "===========AFTER THE LOOP===========" << endl;
            cout << "duringStatement is " << (duringStatement ? "true" : "false") << endl;
            
            // Move to the next token
            advance();
            cout << endl;
        }

        for (Token tk : temp)
        {   
            cout << "\nExpression Statements for " << tk.lexeme << "\n";
            for (auto s : tk.expr)
            {
                if (s != "")
                    cout << s << endl;
            }
        }

    }

    return 0;
}

// Check if the following sentence is <Statement>
bool checkStatement()
{
    bool isStatement = false;
    Token token = tmp;
    cout << "In checkStatement(), token is " << tmp.lexeme << endl;

    if (checkAssign())
    {
        cout << "<Statement> -> <Assign>" << endl;
        isStatement = true;
    }
    else if (checkDeclarative())
    {
        cout << "<Statement> -> <Declarative>" << endl;
        isStatement = true;
    }

    return isStatement;
}

// Check if the following sentence is <Assign>
bool checkAssign()
{
    bool isAssign = false;
    Token token = tmp;
    cout << "In checkAssign(), token is " << tmp.lexeme << endl;

    if (checkIdentifier())
    {
        moveup();
        token = tmp;

        if (tmp.lexeme == "=")
        {
            moveup();

            if (checkExpression())
            {
                cout << "<Assign> -> <ID> = <Expression>" << endl;
                isAssign = true;
            }

            backdown();
        }

        backdown();
        token = tokens[token_ptr];
    }

    return isAssign;
}

// Check if the following sentence is <Declarative>
bool checkDeclarative()
{
    bool isDeclarative = false;
    Token token = tmp;
    cout << "In checkDeclarative(), token is " << tmp.lexeme << endl;

    if (checkType())
    {
        moveup();

        if (checkIdentifier())
        {
            cout << "<Declarative> -> <Type> <ID>" << endl;
            isDeclarative = true;
        }

        backdown();
    }

    return isDeclarative;
}

// Check if the following sentence is <Expression>
bool checkExpression()
{
    bool isExpression = false;
    Token token = tokens[token_ptr];
    cout << "In checkExpression(), token is " << token.lexeme << endl;

    if (checkTerm())
    {
        moveup();

        if (checkExpressionPrime())
        {
            cout << "<Expression> -> <Term> <ExpressionPrime>" << endl;
            isExpression = true;
        }

        backdown();
    }

    return isExpression;
}
// Check if the following sentence is <ExpressionPrime>
bool checkExpressionPrime()
{
    bool isExpressionPrime = false;
    Token token = tmp;
    cout << "In checkExpressionPrime(), token is " << tmp.lexeme << endl;

    if (tmp.lexeme == "+" || tmp.lexeme == "-")
    {
        backdown();
        //moveup();

        if (checkTerm())
        {
            moveup();
            moveup();
            moveup();
            // remove 2

            if (checkExpressionPrime())
            {
                backdown(); // To display the + or -
                backdown();

                cout << "<ExpressionPrime> -> " << token.lexeme << " <Term> <ExpressionPrime>" << endl;
                isExpressionPrime = true;

                moveup(); // Move back up to avoid backing down too much
                moveup();
            }

            backdown();
        }

        backdown();
    }
    else if (tmp.lexeme == ")" || tmp.lexeme == ";")
    {
        cout << "<ExpressionPrime> -> EPSILON" << endl;
        isExpressionPrime = true;
    }

    return isExpressionPrime;
}

// Check if the following sentence is <Term>
bool checkTerm()
{
    bool isTerm = false;
    Token token = tmp;
    cout << "In checkTerm(), token is " << tmp.lexeme << endl;

    if (checkFactor())
    {
        moveup();

        if (checkTermPrime())
        {
            cout << "<Term> -> <Factor> <TermPrime>" << endl;
            isTerm = true;
        }

        backdown();
    }

    return isTerm;
}

// Check if the following sentence is <TermPrime>
bool checkTermPrime()
{
    bool isTermPrime = false;
    Token token = tmp;
    cout << "In checkTermPrime(), token is " << tmp.lexeme << endl;

    if (tmp.lexeme == "*" || tmp.lexeme == "/")
    {
        moveup();

        if (checkFactor())
        {
            moveup();

            if (checkTermPrime())
            {
                backdown(); // To display the + or -
                backdown();

                cout << "<TermPrime> -> " << tmp.lexeme << " <Factor> <TermPrime>" << endl;
                isTermPrime = true;

                moveup(); // Move back up to avoid backing down too much
                moveup();
            }

            backdown();
        }

        backdown();
    }
    else if (tmp.lexeme == ")" || tmp.lexeme == ";" || tmp.lexeme == "+" || tmp.lexeme == "-")
    {
        cout << "<TermPrime> -> EPSILON" << endl;
        isTermPrime = true;
    }

    return isTermPrime;
}

// Check if the following sentence is <Factor>
bool checkFactor()
{
    bool isFactor = false;
    Token token = tmp;
    cout << "In checkFactor(), token is " << tmp.lexeme << endl;

    if (tmp.lexeme == "(")
    {
        moveup();

        if (checkExpression())
        {
            moveup();
            token = tmp;

            if (tmp.lexeme == ")")
            {
                cout << "<Factor> -> ( <Expression> )" << endl;
                isFactor = true;
            };

            backdown();
            token = tmp;
        }

        backdown();
    }
    else if (checkIdentifier())
    {
        cout << "<Factor> -> <ID>" << endl;
        isFactor = true;
    }
    else if (isNumeric(tmp.lexeme))
    {
        cout << "<Factor> -> num" << endl;
        isFactor = true;
    }

    return isFactor;
}

// Check if the following sentence is <Identifier>
bool checkIdentifier()
{
    bool isIdentifier = false;
    Token token = tmp;
    cout << "In checkIdentifier(), token is " << tmp.lexeme << endl;

    if (tmp.lexemeNum == IDENTIFIER)
    {
        cout << "<ID> -> " << tmp.lexeme << endl;
        isIdentifier = true;
    }

    return isIdentifier;
}

// Check if the following sentence is <Type>
bool checkType()
{
    bool isType = false;
    Token token = tmp;
    cout << "In checkType(), token is " << tmp.lexeme << endl;

    if (tmp.lexemeNum == KEYWORD)
    {

        if (tmp.lexeme == "bool" || tmp.lexeme == "float" || tmp.lexeme == "int")
        {
            cout << "<Type> -> " << tmp.lexeme << endl;
            isType = true;
        }
    }

    return isType;
}