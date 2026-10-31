#ifndef CRYPT_H
#define CRYPT_H

#ifndef SALTPATH
#define SALTPATH ""
#endif

#ifndef SALTLEN
#define SALTLEN 0
#endif

#define SIZE 65

void
encrypt(char* str, char output[SIZE]);

#endif
