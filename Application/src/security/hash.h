#pragma once
#include <vector>
#include <string>

class Hash
{
    public:
        static std::vector<unsigned char> encryption(const std::string& password, const std::vector<unsigned char>& salt, int iteration = 100000, int key_len = 32);
        static std::vector<unsigned char> generate_salt(size_t size = 16);

};