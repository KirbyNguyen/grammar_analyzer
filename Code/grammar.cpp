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

bool isExpression(Token);
bool isExpressionPrime(Token);
bool isTerm(Token);
bool isTermPrime(Token);
bool isFactor(Token);

void backup(vector<Token>);

int token_ptr;
bool epsilon = false;

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

    Token moneySign;
    moneySign.lexemeNum = 4;
    moneySign.lexeme = '$';
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
        // Skip comment section
        while (tokens[token_ptr].lexemeNum == 6)
        {
            token_ptr++;
        };

        cout << "Token: " << tokens[token_ptr].lexemeName << "\t\t Lexeme: " << tokens[token_ptr].lexeme << endl;
        isExpression(tokens[token_ptr]);

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
bool isExpression(Token token)
{
    bool expression = false;

    if (isTerm(token))
    {
        if (isExpressionPrime(token))
        {
            expression = true;
            cout << "<Expression> -> <Term> <ExpressionPrime>\n";
        }
    };

    return expression;
}

// Expression_ is Q in the book
bool isExpressionPrime(Token token)
{
    bool expressionPrime = false;

    if (token.lexeme == "+" || token.lexeme == "-")
    {
        if (isTerm(token))
        {
            // if (isExpressionPrime(token))
            // {
                expressionPrime = true;
                cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | eps\n";
            // };
        };

        // isExpressionPrime(token);
    }
    else if (token.lexeme != "$" || token.lexeme != ")")
    {
        expressionPrime = true;
        cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | eps\n";
        // cout << "Token in Follow of (E') expected\n";
    }

    return expressionPrime;
}

// Term is T in the book
bool isTerm(Token token)
{
    bool term = false;

    if (isFactor(token))
    {
        if (isTermPrime(token))
        {
            term = true;
            cout << "<Term> -> <Factor> <TermPrime>\n";
        };
    };

    return term;
}

// TermPrime is R in the book
bool isTermPrime(Token token)
{
    bool termPrime = false;

    if (token.lexeme == "*" || token.lexeme == "/")
    {
        termPrime = true;

        if (isFactor(token))
        {
            // if (isTermPrime(token))
            // {
                termPrime = true;
                cout << "<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | eps\n";
            // };
        };
        // isTermPrime(token);
    }
    else if (token.lexeme != "$" || token.lexeme != ")" || token.lexeme == "+" || token.lexeme == "-")
    {
        termPrime = true;
        cout << "<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | eps\n";
        // cout << "Token in Follow of (T') expected\n";
    }

    return termPrime;
}

// Factor is F in the book
bool isFactor(Token token)
{

    bool factor = false;

    if (isNumeric(token.lexeme))
    {
        factor = true;
        cout << "<Factor> -> num\n";
    }
    else if (token.lexeme == ")")
    {
        factor = true;
        cout << "<Factor> -> ( <Expression> )\n";
    }
    else if (token.lexemeName == "IDENTIFER")
    {
        factor = true;
        cout << "<Factor> -> <Identifier>\n";
    }

    return factor;
};