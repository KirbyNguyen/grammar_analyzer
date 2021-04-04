#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "lexer.cpp"

using namespace std;

/** BASED ON PAGE 103 - 106 IN THE BOOK
    Expression      = E
    ExpressionPrime = Q
    Term            = T
    TermPrime       = R
    Factor          = F
    ============================
    isExpression        = function E
    isExpressionPrime   = function Q
    isTerm              = function T
    isTermPrime         = function R
    isFactor            = function F
**/

bool isExpression(Token);
bool isExpressionPrime(Token);
bool isTerm(Token);
bool isTermPrime(Token);
bool isFactor(Token);
void backup(vector<Token>);

int token_ptr;

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

    Token moneySign;
    moneySign.lexemeNum = 4;
    moneySign.token = '$';
    moneySign.lexemeName = "SEPERATOR";

    // A vector to hold the tokens
    vector<Token> tokens;

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
        while (tokens[token_ptr].lexemeNum == 6) {token_ptr++;};

        cout << "Token: " << tokens[token_ptr].lexemeName << "\t\t Lexeme: " << tokens[token_ptr].token << endl;
        if (isExpression(tokens[token_ptr])) {
            cout << "Success.\n";
        } else {
            cout << "Failures.\n";
        }

        token_ptr++;
        cout << endl;
    }

    // Close the file
    infile.close();
    infile.clear();

    return 0;
}

void backup()
{
    token_ptr > 0 ? token_ptr -= token_ptr : token_ptr = token_ptr;
};

// Expression is E in the book
bool isExpression(Token lexeme)
{
    bool expression = false;
    if (isTerm(lexeme))
    {
        if (isExpressionPrime(lexeme))
        {
            cout << "<Expression> -> <Term><ExpressionPrime>\n";
            expression = true;
        }
    }

    return expression;
}

// Expression_ is Q in the book
bool isExpressionPrime(Token lexeme)
{
    bool expressionPrime = false;
    char cc = lexeme.token[0];
    if (cc == '+' || cc == '-')
    {
        if (isTerm(lexeme))
        {
            if (isExpressionPrime(lexeme))
            {
                cout << "<ExpressionPrime> -> " << cc << " <Term><ExpressionPrime>\n";
                expressionPrime = true;
            }
        }
    }
    else if (cc == ')' || cc == '$')
    {
        backup();
        cout << "<ExpressionPrime> -> eps";
        expressionPrime = true;
    }
    return expressionPrime;
}

// Term is T in the book
bool isTerm(Token lexeme)
{
    bool term = false;
    if (isFactor(lexeme))
    {
        if (isTermPrime(lexeme))
        {
            cout << "<Term> -> <Factor><TermPrime>\n";
            term = true;
        }
    }

    return term;
}

// Term_ is R in the book
bool isTermPrime(Token lexeme)
{
    bool termPrime = false;
    char cc = lexeme.token[0];
    if (cc == '*' || cc == '/')
    {
        if (isFactor(lexeme))
        {
            if (isTermPrime(lexeme))
            {
                cout << "<TermPrime> -> " << cc << " <Factor><TermPrime>\n";
                termPrime = true;
            }
        }
    }
    else if (cc == ')' || cc == '$' || cc == '+' || cc == '-')
    {
        cout << "<Term_> -> eps";
        backup();
        termPrime = true;
    }
    return termPrime;
}

// Factor is F in the book
bool isFactor(Token lexeme)
{
    bool factor = false;
    char cc = lexeme.token[0];
    if (isalpha(cc))
    {
        cout << "<Factor> -> id\n";
        factor = true;
    }
    else if (cc == ')')
    {
        cout << "<Factor> -> ( <Expression> )\n";
        factor = true;
    }
    return factor;
};