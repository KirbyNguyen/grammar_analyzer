#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "lexer.cpp"

using namespace std;

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

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

    // Get each line in the text file

    // Determine each token in the line
    while (getline(infile, line))
    {
        vector<Token> temp = lexer(line);
        tokens.insert(tokens.end(), temp.begin(), temp.end());
    }

    // Display the tokens to the screen
    for (unsigned x = 0; x < tokens.size(); ++x)
    {
        // Don't print out the comment
        if (tokens[x].lexemeNum != IGNORE)
        {
            cout << tokens[x].lexemeName << "  \t"
                 << tokens[x].token << endl;
        }
    };

    // Close the file
    infile.close();
    infile.clear();

    return 0;
}