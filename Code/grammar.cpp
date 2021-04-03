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
    Expression  = E
    Expression_ = Q
    Term        = T
    Term_       = R
    Factor      = F
    ============================
    isExpression    = function E
    isExpression_   = function Q
    isTerm          = function T
    isTerm_         = function R
    isFactor        = function F
**/

bool isExpression(Token);
bool isExpression_(Token);
bool isTerm(Token);
bool isTerm_(Token);
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
        if (isExpression_(lexeme))
        {
            cout << "<Expression> -> <Term><Expression_>\n";
            expression = true;
        }
    }

    return expression;
}

// Expression_ is Q in the book
bool isExpression_(Token lexeme)
{
    bool expression_ = false;
    char cc = lexeme.token[0];
    if (cc == '+' || cc == '-')
    {
        if (isTerm(lexeme))
        {
            if (isExpression_(lexeme))
            {
                cout << "<Expression_> -> " << cc << " <Term><Expression_>\n";
                expression_ = true;
            }
        }
    }
    else if (cc == ')' || cc == '$')
    {
        backup();
        cout << "<Expression_> -> eps";
        expression_ = true;
    }
    return expression_;
}

// Term is T in the book
bool isTerm(Token lexeme)
{
    bool term = false;
    if (isFactor(lexeme))
    {
        if (isTerm_(lexeme))
        {
            cout << "<Term> -> <Factor><Term_>\n";
            term = true;
        }
    }

    return term;
}

// Term_ is R in the book
bool isTerm_(Token lexeme)
{
    bool term_ = false;
    char cc = lexeme.token[0];
    if (cc == '*' || cc == '/')
    {
        if (isFactor(lexeme))
        {
            if (isTerm_(lexeme))
            {
                cout << "<Term_> -> " << cc << " <Factor><Term_>\n";
                term_ = true;
            }
        }
    }
    else if (cc == ')' || cc == '$' || cc == '+' || cc == '-')
    {
        cout << "<Term_> -> eps";
        backup();
        term_ = true;
    }
    return term_;
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