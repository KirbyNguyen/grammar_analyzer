#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "lexertmp.cpp"

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

bool isE();
bool isQ();
bool isT();
bool isR();
bool isF();
void backup(vector<Token>);
Token token;

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
        while (tokens[token_ptr].lexemeNum == 6)
        {
            token_ptr++;
        };

        cout << "Token: " << tokens[token_ptr].lexemeName << "\t\t Lexeme: " << tokens[token_ptr].token << endl;
        // if (isExpression(tokens[token_ptr]))
        // {
        //     cout << "Success.\n";
        // }
        // else
        // {
        //     cout << "Failures.\n";
        // }
        token = tokens[token_ptr];
        cout << token.token << endl;
        char cc = token.token[0];
        if (isE() && cc == '$')
        {
          cout << "Success.\n";
        }
        else if (token_ptr >= tokens.size() - 1) {
          cout << "Failures.\n";
          exit(-1);
        }
        else {
          //cout << "Failures.\n";
          //exit(-1);
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
    //token_ptr > 0 ? token_ptr -= token_ptr : token_ptr = 0;
    token_ptr = (token_ptr > 0 ? token_ptr - 1 : 0);
};

// Expression is E in the book
bool isE()
{
    // bool expression = false;
    // if (isTerm(token))
    // {
    //     if (isExpressionPrime(token))
    //     {
    //         cout << "<Expression> -> <Term><ExpressionPrime>\n";
    //         expression = true;
    //     }
    // }
    // return expression;
    string cc = token.token;
    //cout << "Debug Line - " << endl;
    //cout << "token = " << cc << " :: token num = " << token.lexemeNum;
    //cout << " :: lexemeName = " << token.lexemeName << endl;
    /*if (token.lexemeNum == 5 || cc[0] == '(')
    {
        cout << "<Expression> -> <Term> <ExpressionPrime>\n";
        isTerm(token);
        isExpressionPrime(token);
    }
    else if (cc[0] != '=' && token.lexemeNum == 3 || token.lexemeNum == 4)
    {
        cout << "Empty -> Epsilon\n";
        isTerm(token);
        isExpressionPrime(token);
    }
    else
    {
        cout << "Token in First of (E) expected\n";
    }*/
    bool e = false;
    if (isT())
    {
      if (isQ())
      {
        cout << "<Expression> -> <Term> <ExpressionPrime>\n";
        e = true;
      }
    }
    return e;
}

// Expression_ is Q in the book
bool isQ()
{
    // bool expressionPrime = false;
    // char cc = token.lexeme[0];
    // if (cc == '+' || cc == '-')
    // {
    //     if (isTerm(token))
    //     {
    //         if (isExpressionPrime(token))
    //         {
    //             cout << "<ExpressionPrime> -> " << cc << " <Term><ExpressionPrime>\n";
    //             expressionPrime = true;
    //         }
    //     }
    // }
    // else if (cc == ')' || cc == '$')
    // {
    //     backup();
    //     cout << "<ExpressionPrime> -> eps";
    //     expressionPrime = true;
    // }
    // return expressionPrime;
    bool q = false;
    string cc = token.token;
    if (cc[0] == '+' || cc[0] == '-')
    {
      //cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | eps\n";
      if (isT())
      {
        if (isQ())
        {
          cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | eps\n";
          q = true;
        }
      }
    }
    /*else if (token.lexemeName.compare("+") == 0 && token.lexemeName.compare("-"))
    {
        cout << "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | eps\n";
        isTerm(token);
        isExpressionPrime(token);
    }*/
    else if (cc[0] == ')' || cc[0] == '$')
    {
        backup();
        cout << "<ExpressionPrime> -> eps\n";
        cout << "Token in Follow of (E') expected\n";
        q = true;
    }
    return q;
}

// Term is T in the book
bool isT()
{
    // bool term = false;
    // if (isFactor(token))
    // {
    //     if (isTermPrime(token))
    //     {
    //         cout << "<Term> -> <Factor><TermPrime>\n";
    //         term = true;
    //     }
    // }
    // return term;string cc = token.token;
    bool t = false;
    if (isF())
    {
      if (isR())
      {
        cout << "<Term> -> <Factor> <TermPrime>\n";
        t = true;
      }
    }
    return t;
    /*if (token.lexemeName[0] == 'I' || cc[0] == '(')
    {
        isFactor(token);
        isTermPrime(token);
        cout << "<Term> -> <Factor> <TermPrime>\n";
    }
    else if (token.lexemeNum == 3 || token.lexemeNum == 4)
    {
        cout << "<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>\n";
    }
    else
    {
        cout << "Token in First of (T) expected\n";
    }*/
}

// Term_ is R in the book
bool isR()
{
    // bool termPrime = false;
    // char cc = token.lexeme[0];
    // if (cc == '*' || cc == '/')
    // {
    //     if (isFactor(token))
    //     {
    //         if (isTermPrime(token))
    //         {
    //             cout << "<TermPrime> -> " << cc << " <Factor><TermPrime>\n";
    //             termPrime = true;
    //         }
    //     }
    // }
    // else if (cc == ')' || cc == '$' || cc == '+' || cc == '-')
    // {
    //     cout << "<Term_> -> eps";
    //     backup();
    //     termPrime = true;
    // }
    // return termPrime;
    bool r = false;
    string ss = token.token;
    /*if ((token.lexemeNum == 3 || token.lexemeNum == 4) && ss[0] != '=' && ss[0] != ')')
    //if (ss[0] == '*' || ss[0] == ')')
    {
        cout << "<TermPrime> -> * <Factor> <TermPrime> | - <Factor> <TermPrime> | eps\n";
        cout << "<Empty> -> Epsilon\n";
        isTerm(token);
        isExpressionPrime(token);
    }*/
    if (ss[0] == '*' || ss[0] == '/')
    {
      if (isF())
      {
        if (isR())
        {
          cout << "<TermPrime> -> * <Factor> <TermPrime> | - <Factor> <TermPrime> | eps\n";
          r = true;
        }
      }
    }
    else if (ss[0] == '$' || ss[0] == ')' || ss[0] == '+' || ss[0] == '-')
    {
      cout << "<Empty> -> Epsilon\n";
      backup();
      r = true;

      //cout << "Token in Follow of (T') expected\n";
    }
    return r;
}

// Factor is F in the book
bool isF(){
    // bool factor = false;

    bool f = false;
    string ss = token.token;

    if (isalpha(ss[0]))
    {
        cout << "<Factor> -> <Identifier>\n";
        f = true;
    }
    else if (isdigit(ss[0]))
    {
        cout << "<Factor> -> num\n";
        f = true;
    }
    else if (ss[0] == '(')
    {
        if (isE())
        {
          ss = token.token;
          if (ss[0] == ')')
          {
            cout << "<Factor> -> ( <Expression> )\n";
            f = true;
          }
        }

    }
    return f;
};
