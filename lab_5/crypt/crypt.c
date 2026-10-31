#include "crypt.h"
#include "util.h"

#include <errno.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

bool
get_salt(char buffer[SALTLEN+1])
{
    int fd = open(SALTPATH, O_RDONLY);
    if (fd == -1) {
        ERROR("%d", errno);
        return false;
    }

    ssize_t b_read = read(fd, buffer, SALTLEN - 1);
    close(fd);
    if (b_read <= 0) {
        ERROR("bytes read=%ld", b_read);
        return false;
    }
    if (buffer[b_read - 1] == '\n')
        buffer[b_read - 1] = '\0';
    else
        buffer[b_read] = '\0';


    return true;
}

void
encrypt(char* str, char output[SIZE])
{
    // TODO: try to find documentation on new EVP API

    char s[SALTLEN + 1];
    if (get_salt(s) == false) {
        ERROR("cannot get salt");
        return;
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Update(&sha256, s, strlen(s));

    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        snprintf(output + (i * 2), SIZE - (i * 2), "%02x", hash[i]);
    output[SIZE - 1] = '\0';
}
