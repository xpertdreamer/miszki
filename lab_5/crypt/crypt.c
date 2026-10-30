#include "crypt.h"

#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

void
encrypt(char* str, char output[SIZE])
{
    // TODO: try to find documentation on new EVP API
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        snprintf(output + (i * 2), SIZE - (i * 2), "%02x", hash[i]);
    output[SIZE - 1] = '\0';
}
