#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>

#include "lexer.cpp"

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

// A vector to hold the tokens
vector<Token> tokens;
// Token token;
int token_ptr;

// Auxiliry functions
void moveup()
{
    if (token_ptr < tokens.size())
    {
        token_ptr += 1;
    }
};

void backup()
{
    if (token_ptr > 0)
    {
        token_ptr -= 1;
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
    cout << "\nPlease enter the name of the file: ";
    getline(cin, fileName);

    // Open the file
    infile.open(fileName.c_str());

    // Error shooting if the file cannot be opened
    if (infile.fail())
    {
        cout << "\n** ERROR - the file \"" << fileName << "\" cannot be found!\n\n";
        exit(1);
    }

    token_ptr = 0;

    // Determine each token in the line
    while (getline(infile, line))
    {
        vector<Token> temp = lexer(line);
        tokens.insert(tokens.end(), temp.begin(), temp.end());
    }

    tokens.push_back(moneySign);

    // Parse the tree
    while (token_ptr < tokens.size())
    {
        // Skip comment section
        while (tokens[token_ptr].lexemeNum == 6)
        {
            token_ptr++;
        };

        // Display the token
        cout << left << setw(5) << "Token: " << setw(15) << tokens[token_ptr].lexemeName
             << setw(5) << "Lexeme: " << tokens[token_ptr].lexeme << endl;

        if (checkExpression())
        {
        }
        else
        {
        };

        // Move to the next token
        token_ptr++;
        cout << endl;
    }

    // Close the file
    infile.close();
    infile.clear();

    return 0;
}

// Check if the following sentence is a statement
bool checkStatement()
{
    bool isStatement = false;

    // Check if this is a new statement or not
    // If not, start a new statement

    return isStatement;
}

bool checkAssign()
{
    bool isAssign = false;

    return isAssign;
}

bool checkDeclarative()
{
    bool isDeclarative = false;

    return isDeclarative;
}

bool checkExpression()
{
    bool isExpression = false;
    Token token = tokens[token_ptr];

    if (token.lexemeNum == IDENTIFIER || isNumeric(token.lexeme) || token.lexeme == "(")
    {
        if (checkTerm())
        {
            if (checkExpressionPrime())
            {
                cout << "<Expression> -> <Term> <ExpressionPrime>" << endl;
                isExpression = true;
            };
        }
    } else {
        cout << "Token in First(<Expression>) expected" << endl;
    }

    return isExpression;
}

bool checkTerm()
{
    bool isTerm = false;
    Token token = tokens[token_ptr];

    if (token.lexemeNum == IDENTIFIER || isNumeric(token.lexeme) || token.lexeme == "(")
    {
        if (checkFactor())
        {
            if (checkTermPrime())
            {
                cout << "<Term> -> <Factor> <TermPrime>" << endl;
                isTerm = true;
            }
        }
    } else {
        cout << "Token in First(<Term>) expected" << endl;
    }

    return isTerm;
}

bool checkExpressionPrime()
{
    bool isExpressionPrime = false;
    Token token = tokens[token_ptr];

    if (token.lexeme == "+" || token.lexeme == "-")
    {
        if (checkTerm())
        {
            if (checkExpressionPrime())
            {
                cout << "<ExpressionPrime> -> " << token.lexeme << " <Term> <ExpressionPrime>" << endl;
                isExpressionPrime = true;
            }
        }
    }
    else if (token.lexeme == "$" || token.lexeme == ")")
    {
        // backup();
        cout << "<ExpressionPrime> -> Epsilon" << endl;
        isExpressionPrime = true;
    }
    else
    {
        cout << "Token in First(<ExpressionPrime>) expected" << endl;
    }

    return isExpressionPrime;
}

bool checkTermPrime()
{
    bool isTermPrime = false;
    Token token = tokens[token_ptr];

    if (token.lexeme == "*" || token.lexeme == "/")
    {
        if (checkFactor())
        {
            if (checkTermPrime())
            {
                cout << "<TermPrime> -> " << token.lexeme << " <Factor> <TermPrime>" << endl;
                isTermPrime = true;
            }
        }
    }
    else if (token.lexeme == "$" || token.lexeme == ")" || token.lexeme == "+" || token.lexeme == "-")
    {
        cout << "<TermPrime> -> Epsilon" << endl;
        backup();
        isTermPrime = true;
    }
    else
    {
        cout << "Token in First(<TermPrime>) expected" << endl;
    }

    return isTermPrime;
}

bool checkFactor()
{
    bool isFactor = false;
    Token token = tokens[token_ptr];

    if (checkIdentifier())
    {
        cout << "<Factor> -> <ID>" << endl;
        isFactor = true;
    }
    else if (isNumeric(token.lexeme))
    {
        cout << "<Factor> -> num" << endl;
        isFactor = true;
    }
    else if (token.lexeme == "(")
    {
        if (checkExpression())
        {
            token = tokens[token_ptr];
            if (token.lexeme == ")")
            {
                cout << "<Factor> -> ( <Expression> )" << endl;
                isFactor = true;
            }
        }
    }

    return isFactor;
}

bool checkIdentifier()
{
    bool isIdentifier = false;
    Token token = tokens[token_ptr];

    if (token.lexemeNum == IDENTIFIER)
    {
        isIdentifier = true;
        cout << "<ID> -> " << token.lexeme << endl;
    };

    return isIdentifier;
}