#include "hash.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include "error/bussiness_error.h"
using namespace std;

vector<unsigned char> Hash::encryption(const string& password, const vector<unsigned char>& salt, int iteration, int key_len)
{
    vector<unsigned char> hash(key_len);
    
    if(!PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.data(), salt.size(), iteration, EVP_sha256(), key_len, hash.data()))
    {
        throw Hash_Algorithm_Error("PBKDF2 Hash Algorithm Error", __FILE__, __LINE__); //hash failed
    }
    
    return hash;

}

vector<unsigned char> Hash::generate_salt(size_t size)
{
    vector<unsigned char> salt(size);
    RAND_bytes(salt.data(), size);
    return salt;
}