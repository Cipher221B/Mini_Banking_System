#include "string_utils.h"
#include <sstream>
#include <iomanip>
#include <fstream>
using namespace std;

string String_Utils::standardization_number(int value, int n, char c)
{
    stringstream ss;
    ss << setw(n) << setfill(c) << to_string(value);
    return ss.str();
}