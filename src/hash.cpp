#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include "hash.h"
#include "system_errors.h"

vector<unsigned char> Hash::encryption(string& password, vector<unsigned char>& salt, int iteration, int key_len)
{
    vector<unsigned char> hash(key_len);
    
    if(!PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.data(), salt.size(), iteration, EVP_sha256(), key_len, hash.data()))
    {
        throw Hash_Algorithm_Error("PBKDF2 Hash Algorithm Error", __FILE__); //hash failed
    }
    
    return hash;

}

vector<unsigned char> Hash::generate_salt(size_t size)
{
    vector<unsigned char> salt(size);
    RAND_bytes(salt.data(), size);
    return salt;
}