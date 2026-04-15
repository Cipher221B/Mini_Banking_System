#pragma once
#include <vector>
using namespace std;

class Hash
{
    public:
        static vector<unsigned char> encryption(const string& password, vector<unsigned char>& salt, int iteration = 100000, int key_len = 32);
        static vector<unsigned char> generate_salt(size_t size = 16);

};