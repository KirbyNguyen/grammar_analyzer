#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "lexer.cpp"
#include "stringFunctions.cpp"

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

// bool isExpression(Token);
// bool isExpressionPrime(Token);
// bool isTerm(Token);
// bool isTermPrime(Token);
// bool isFactor(Token);
// void backup(vector<Token>);

bool isExpression(vector<Token>);
bool isExpressionPrime(vector<Token>);
bool isTerm(vector<Token>);
bool isTermPrime(vector<Token>);
bool isFactor(vector<Token>);

void backup();

// A vector to hold the tokens
vector<Token> tokens;
int token_ptr;

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

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

    // Token moneySign;
    // moneySign.lexemeNum = 4;
    // moneySign.token = '$';
    // moneySign.lexemeName = "SEPERATOR";

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

    // tokens.push_back(moneySign);

    // Parse the tree
    while (token_ptr < tokens.size())
    {
        // Skip comment section
        while (tokens[token_ptr].lexemeNum == 6)
        {
            token_ptr++;
        };

        cout << "Token: " << tokens[token_ptr].lexemeName << "\t\t Lexeme: " << tokens[token_ptr].lexeme << endl;

        if (isExpression(tokens))
        {
            cout << "Success!\n";
        }
        else
        {
            cout << "Failure!\n";
        }

        token_ptr++;
        cout << endl;
    }

    // Close the file
    infile.close();
    infile.clear();

    return 0;
}

bool isExpression(vector<Token> tokens)
{
    bool expression = false;

    if (isTerm(tokens))
    {
        if (isExpressionPrime(tokens))
        {
            expression = true;
            cout << "<Expression> -> <Term> <ExpressionPrime>\n";
        }
    }

    return expression;
}

bool isExpressionPrime(vector<Token> tokens)
{
    bool expressionPrime = false;

    if (tokens[token_ptr].lexeme == "+" || tokens[token_ptr].lexeme == "-")
    {
        moveup();
        if (isTerm(tokens))
        {
            if (isExpressionPrime(tokens))
            {
                expressionPrime = true;
                cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | epsilon \n";
            }
        }
    }
    else if (tokens[token_ptr].lexeme != "$" || tokens[token_ptr].lexeme != ")")
    {
        expressionPrime = true;
        cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | epsilon \n";
    }

    return expressionPrime;
}

bool isTerm(vector<Token> tokens)
{
    bool term = false;

    if (isFactor(tokens))
    {
        if (isTermPrime(tokens))
        {
            term = true;
            cout << "<Term> -> <Factor> <TermPrime>\n";
        }
    }

    return term;
}

bool isTermPrime(vector<Token> tokens)
{
    bool termPrime = false;

    if (tokens[token_ptr].lexeme == "*" || tokens[token_ptr].lexeme == "/")
    {
        moveup();
        if (isTerm(tokens))
        {
            if (isExpressionPrime(tokens))
            {
                termPrime = true;
                cout << "<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | epsilon \n";
            }
        }
    }
    else if (tokens[token_ptr].lexeme != "$" || tokens[token_ptr].lexeme != ")")
    {
        termPrime = true;
        cout << "<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | epsilon \n";
    }

    return termPrime;
}

bool isFactor(vector<Token> tokens)
{
    bool factor = false;

    if (tokens[token_ptr].lexeme == ")") {
        factor = true;
        cout << "<Factor> -> ( <Expression> )\n";
    } else if (isNumeric(tokens[token_ptr].lexeme)) {
        factor = true;
        cout << "<Factor> -> num\n";
    } else if (tokens[token_ptr].lexemeName == "IDENTIFER") {
        factor = true;
        cout << "<Factor> -> <Identifier>\n";
    }

    return factor;
}