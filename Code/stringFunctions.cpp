#include <string>

using namespace std;

// Return false if it is not a numeric string
// Return true if it is a numeric string
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