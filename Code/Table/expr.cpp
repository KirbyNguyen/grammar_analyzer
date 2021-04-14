#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <stack>

using namespace std;

/*
    Enumerator (currently unused)
*/

//#ifndef _MY_HEADER_H_
//#define _MY_HEADER_H_


//unused for now
enum EXPR_TYPES
{
	INVALID = -1,
	Te,
	PLUSTe,
	MINUSTe,
	Ft,
	STARFt,
	SLASHFt,
	id,
	EPSILON,
	E_WITH_PARENTHESES,
    STAT,
    ASSIGN,
	NOTHNG
};

//#endif /* _MY_HEADER_H_ */
// Set to hold seperators
static const unordered_set<char> SEPS({'{', '}', '[', ']', '(', ')', ',', '.', ';', ':'});
// Set to hold operators
static const unordered_set<char> OPS({'+', '-', '*', '/', '=', '<', '>', '%'});
// Set to hold special characters
static const unordered_set<char> SPEC({'!', '_'});
// Set to hold keywords
static const unordered_set<string> KEYS({"int", "float", "bool", "True", "False",
											 "if", "else", "then", "endif", "endelse", "while", "whileend", "do",
											 "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"});

class Expr 
{
    public:
    stack<string> stk;                  // stack for pushing and popping table elements (shorthand production tokens)
    string str2;                        // holds each sentence passed into the interpret()
    unordered_set<string> expansion;    // holds production rules from the stack in the proper order to be expanded and prevents duplicates
    vector<string> expanded;            // holds expanded production tokens values to be printed out
    string prev;                        // holds previous lexeme, used for syntax checks
    int curr = 0;                       // current string ptr
    bool assigned = false;              // false if an assignment-statement is needed (before EQUALS sign)
    bool once = true;                   // prevent printing assignment-statement multiple times
    string op;                          // specifies operator in the production tokens for ±Te, *Ft, /Ft during printing

    bool isTerminal(string S) 
    {
        return  S == "*" || S == "/" || 
                S == "+" || S == "-" ||
                S == "i" || S == "#" || 
                S == "(" || S == ")";

    }
    // Expression production rules starting with E -> T E'
    string RULES[5][10] = {
    //        id      +     -      *         /      (       )       $       ;         =
            {"Te",  "nil", "nil",  "nil",  "nil",  "Te",   "nil",  "nil",   "",    "skip"},    // E        E  ->  T E'                  in the table : e = E'
            {"nil", "+Te", "-Te",  "nil",  "nil",  "nil",   "",    "",      "",    "skip"},    // E’       E' ->  +- T E' | epsilon     in the table : e = E' , "" = epsilon
            {"Ft",  "nil", "nil",  "nil",  "nil",  "Ft",   "nil",  "nil",   "",    "skip"},    // T        T  ->  F T'                  in the table : t = T'
            {"nil",   "",    "",   "*Ft",  "/Ft",  "nil",   "",     "",     "",    "skip"},    // T’       T' ->  */ F T' | epsilon     in the table : t = T' , "" = epsilon
            {"i",   "nil", "nil",  "nil",  "nil",  "(E)",  "nil",  "nil",  "nil",  "skip"}     // F        F  ->  ( E )   | id          in the table : i stands for id
    };

    int newRULES[5][10] = {
    //        id          +          -          *         /             (                   )          $          ;         =
            {Te,       INVALID,   INVALID,   INVALID,   INVALID,     Te,                 INVALID,   NOTHNG,   EPSILON,   NOTHNG},    // E
            {INVALID,  PLUSTe,    MINUSTe,   INVALID,   INVALID,     INVALID,            EPSILON,   NOTHNG,   EPSILON,   NOTHNG},    // E’
            {Ft,       INVALID,   INVALID,   INVALID,   INVALID,     Ft,                 INVALID,   NOTHNG,   EPSILON,   NOTHNG},    // T
            {INVALID,  EPSILON,   EPSILON,   STARFt,    SLASHFt,     INVALID,            EPSILON,   NOTHNG,   EPSILON,   NOTHNG},    // T’
            {id,       INVALID,   INVALID,   INVALID,   INVALID,     E_WITH_PARENTHESES, INVALID,   NOTHNG,   EPSILON,   NOTHNG}     // F
    };

    //  retreives the appropriate production rule from RULES to be analyzed and (if applicable) pushed onto the stack
    string table(string X, string a)
    {
        int col, row = -1;
        //  set any alphanumeric token to follow the rules for id
        if (isalnum(a[0]))
        {
            a = "i";
        }
        //  get row of the table that corresponds to the token type
        switch (a[0]) 
        {
            case 'i' : col=0; break;
            case '+' : col=1; break;
            case '-' : col=2; break;
            case '*' : col=3; break;
            case '/' : col=4; break;
            case '(' : col=5; break;
            case ')' : col=6; break;
            case '$' : col=7; break;
            case ';' : col=8; break;
            case '=' : col=9; break;
            case '#' : col=9; break;
            default: cout << "ERROR while looking up table\n"; exit(-1); break;
        }
        //  get column of the table that corresponds to the token type
        switch (X[0]) 
        {
            case 'E' : row=0; break;
            case 'e' : row=1; break;
            case 'T' : row=2; break;
            case 't' : row=3; break;
            case 'F' : row=4; break;
        }
        return RULES[row][col];
    }
    // expand and print the production rules into their expressions (hard-coded the statements)
    string convert(string S)        // expands one production rule at a time (e.g. E -> T E')
    {
        if (S == "") {return NULL;}
        string tmp;
        //  Override for tokens belonging to statement assignment-declaration
        if (!assigned)
        {
            if (once)
            {
                tmp = "     <Statement> -> <Assign>\n     <Assign> -> <ID> = <Expression>;\n"
                      "     <Statement> -> <Declarative>\n     <Declarative> -> <Type> <ID>\n";     // not sure about this line
                cout << tmp;
                //  prevent printing this multiple times
                once = false;
            }
            
            return "";
        }
        //  needs fixing
        //  Production Rules for all tokens belonging to Expression are printed here
        for (char c : S)
        {
            switch(c)
            {
                case 'E' :      //  expand E -> T E'
                    tmp += "    <Expression> -> <Term> <ExpressionPrime> ";
                    break;
                case 'e' :      //  ONLY print on semicolon  ::  expand E' -> ± T E' | epsilon   (false positives on Te, +Te, -Te)
                    if (str2[curr] == ';')
                    {
                        tmp += "    <ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty> ";
                    }
                    break;
                case '+' :      //  expand E' -> ± T E' | epsilon
                    tmp += "    <Expression Prime> -> + <Term> <ExpressionPrime> | <Empty> ";      
                    break;
                case '-' :
                    tmp += "    <Expression Prime> -> - <Term> <ExpressionPrime> | <Empty> "; 
                    break;
                case 'T' :      //  expand T -> F T' | epsilon
                    tmp += "    <Term> -> <Factor> <TermPrime> ";
                    break;
                case '*' :
                    tmp += "    <Term Prime> -> * <Factor> <TermPrime> | <Empty> ";
                    break;
                case '/' :
                    tmp += "    <Term Prime> -> / <Factor> <TermPrime> | <Empty> ";
                    break;
                case 't' :      //  ONLY print on semicolon  ::  expand T' -> F T' | epsilon    (false positives on Ft, *Ft, /Ft))
                    if (str2[curr] == ';')
                    {
                        tmp += "    <Empty> -> Epsilon\n    <TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty> ";
                    }
                    break;
                case 'F' :      //  ONLY print on id or (  ::  expand F -> ( E ) | id | num     (false positives on *Ft and /Ft)
                    if (str2[curr] == ')' || isalnum(str2[curr]))
                    {
                        tmp += "    <Factor> -> ( Expression ) | <ID> | <Num> ";
                    }
                    break;
                case 'i' :      //  print ID -> id (actual)
                    tmp += "    <ID> -> "+str2.substr(curr,1);
                    break;
                case '~' :      //  expand epsilon
                    tmp += "    <Empty> -> Epsilon ";
                    break;
                default  :
                    tmp += "    <Empty> -> Epsilon ";
                    break;
                //  if operator is part of production rules, add to the corresponding statement during printing
                /*case '+' :  
                    op = '+';
                    break;
                case '-' :
                    op = '-'; 
                    break;
                case '*' :
                    op = '*';
                    break;       
                case '/' :
                    op = '/';
                    break;*/
                
            }
        }
        // don't print an empty line
        if (tmp!= "") { cout << tmp << endl; }
        return tmp;
    }
    // gets the lexeme's name while ruling out bad syntax such as duplicate consective operators
    string getLex(string tk)
    {
        string tmp, tmp2;
        //  crashes for wrong syntax: consecutive Identifiers
        if (isalpha(tk[0]))
        {
            tmp = (KEYS.find(tk) != KEYS.end() ? "KEYWORD" : "IDENTIFIER");
            if (prev == tmp)
            {
                cout << "\nCRITICAL SYNTAX ERROR : Consecutive " << tmp << " tokens found in the sentence!\n** Now Exiting..."; exit(1);
            }
            prev = tmp;
            tmp2 = tk;
            return tmp;
        }
        //  crashes for wrong syntax: consecutive Numbers
        if (isdigit(tk[0]))
        {
            tmp = "INTEGER";
            if (prev == tmp)
            {
                cout << "\nCRITICAL SYNTAX ERROR : Consecutive " << tmp << " tokens found in the sentence!\n** Now Exiting..."; exit(1);
            }
            prev = tmp;
            tmp2 = tk;
            return "INTEGER";
        }
        //  crashes for wrong syntax: consecutive Operators
        if (OPS.find(tk[0]) != OPS.end())
        {
            tmp = "OPERATOR";
            if (prev == tmp)
            {
                cout << "\nCRITICAL SYNTAX ERROR : Consecutive " << tmp << " tokens found in the sentence!\n** Now Exiting..."; exit(1);
            }
            prev = tmp;
            tmp2 = tk;
            return "OPERATOR";
        }
        //  crashes for wrong syntax: consecutive Separators if not same-parentheses (with a warning for same-parentheses)
        if (SEPS.find(tk[0]) != SEPS.end())
        {
            tmp = "SEPARATOR";
            if (tk == ";" && prev == "OPERATOR")
            {
                cout << "\nCRITICAL SYNTAX ERROR : " << tmp << " right before " << tk << " token!\n** Now Exiting..."; exit(1);
                exit(1);
            }
            if (tk == tmp2)
            {
                if (tk != "(" && tk != ")")
                {
                    cout << "\nCRITICAL SYNTAX ERROR : consecutive " << tk << " tokens in the same sentence!\n** Now Exiting..."; exit(1);
                }
                cout << "\nWarning : consecutive " << tk << " tokens encountered\n";
            }
            if (tmp2 == "(" && tk == ")")
            {
                cout << "\nCRITICAL SYNTAX ERROR : Empty parenthetical enclosure\n** Now Exiting..."; exit(1);
            }
            prev = tmp;
            tmp2 = tk;
            return "SEPARATOR";
        }
        //  crashes if a token that does not belong to any of the above groups made it to this point
        cout << "\nCRITICAL SYNTAX ERROR : Illegal or mismatched token present in the sentence!\n** Now Exiting..."; exit(1);
        return NULL;
    }

    bool interpret()
    {   
        int index;                                      // only holds index of first EQUALS sign, if one exists
        if (str2[curr] == '!' || str2[curr] == '_')     // stop running if illegal character passed into function
        {
            return false;
        }
        if ((index = str2.find('=')) == string::npos)   // if no EQUALS sign present skip statement assignment
        {
            assigned = true;
        }
        
        //  Print the contents of the sentence
        //  Print the token and lexeme about to be syntactically analyzed    
        cout << "\n\n   ::::::::::::::::: New Sentence :::::::::::::::::   " << str2 << "\n\n\n";
        cout << "Token: " << getLex(str2.substr(curr,1)) << "      " << "Lexeme:    " << str2[curr] << endl;
        //  Start stack with basic default elements
        stk.push("$"); stk.push("E");
        string a, X;

        //  Loop until error or end of sentence / bottom of stack is reached
        for(;;) {
            a = str2[curr];
            X = stk.top();
            //  Special conditions for assignment statement
            if (a == "=")
            {
                if (assigned)           
                {
                    if (curr != index)      //  logic check for bad syntax
                    {
                        cout << "Bad Syntax Error : " << a << " was found multiple times in the current sentence\n** Now Exiting..."; exit(-1);
                    }
                }
                //  don't assign after EQUALS sign
                assigned = true;
                once = false;
            }
            //  Check if Stack points to a valid terminal Production Element (not belonging in {E, T, e, t, F})
            if (isTerminal(X) || X == "$") 
            {
                //  Check if both the stack and current token point to an identifier
                if (a == X || (isalnum(a[0]) && X == "i")) 
                {                       
                    //  Fully expand the production rules
                    //  Empty the stack (stk) and unordered set (expansion), restore stack with default contents
                    //  Increment token pointer
                    expansion.insert(expansion.begin(), stk.top());
                    expandTerms();
                    expansion.clear();
                    stk.pop(); 
                    curr++;
                    if (!assigned)
                    {
                        assigned = true;
                    }
                    //  Print the next token and lexeme about to be syntactically analyzed
                    cout << "\nToken = " << getLex(str2.substr(curr,1)) << "      " << "Lexeme:    " << str2[curr] << "\n";
                }   
                else
                //  Semicolon denotes end of current sentence so process the completed expansion set and clear all values for the next sentence
                //  If not a semicolon the syntax must be wrong -> crash
                {
                    if (a == ";")
                    {
                        expandTerms();
                        expansion.clear();
                        curr = 0;
                        assigned = false;
                        once = true;
                        return true;
                    }
                    cout << "interpreted value = false\nreason : bad token " << a << " is not compatible with stack top " << X << "\n** Now Exiting...";
                    exit(1);
                    return false;
                }
            } 
            else
            {
            //  All Non-Terminal Production Elements are processed here {E, T, e, t, F} 
                //  lookup the RULES table for shorthand production rule corresponding to current stack pointer and lexeme
                string prod = table(X,a);

                //  Special rules for EQUALS sign
                //  Empty the stack (stk) and unordered set (expansion), and restore stack with default contents
                //  Statement assignment won't be used in expression parsing
                //  Increment token pointer
                if (prod == "skip")
                {
                    while(!stk.empty())
                    {
                        stk.pop();
                    }                    
                    expansion.clear();
                    stk.push("$");
                    stk.push("E");
                    assigned = true;
                    curr++;
                    cout << "\nToken = " << getLex(str2.substr(curr,1)) << "           " << "Lexeme: " << str2[curr] << "\n";
                    continue;
                }
                //  Immediately process top value of stack and expansion
                //  Print expanded terms in expansion
                //  Pop top element of stack and reset expansion
                expansion.insert(expansion.begin(), stk.top());
                expandTerms();
                expansion.clear();
                stk.pop();
                //  Breakpoint for bad syntax: rule mismatch
                if (prod == "nil") 
                {
                    cout << "\n** ERROR - interpreted value = false\nreason : token " << a << " has broken a nil production rule\n** Now Exiting...";
                    exit(1);
                    return false;
                }
                // Insert each letter of the production rule onto the top of the stack
                // Insert each letter of the production rule to the front of the unordered set
                for(int i = prod.length() - 1; i>=0; i--)
                {
                    stk.push(prod.substr(i,1));
                    expansion.insert(expansion.begin(), prod.substr(i,1));
                    // Insert EPSILON if requirements are met                  
                    if ((a == "+" || a == "-" || a == "*" || a == "/") && (prod[i] == 'e' || prod[i] == 't'))
                    {
                        expansion.insert("~");  // for EPSILON use
                    }
                }
                // Insert EPSILON if table lookup condition is met    
                if (prod == "")
                {
                    expansion.insert("~");      // for EPSILON use
                }
            }
            // Finished with no errors
            if (X == "$" || X == ";") {
                    cout << boolalpha << "interpreted value = true\n";return true;}
        }
    }
    //  Process contents of expansion (unordered set) so the Production Rules can be printed
    vector<string> expandTerms()
    {
        op = "";
        vector<string> tmp;
        for (string st : expansion)
        {
            if (st == ";")
            {
                
            }
            tmp.push_back(convert(st));            
        }
        expanded = tmp;
        return tmp;
    }
};

