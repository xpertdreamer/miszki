#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <stddef.h>

#define PUNCTUATION ",.!?;:-()[]{}\"'`@#%^&+=/\\|<>~"
#define DIGIT       "0123456789"
#define LETTER      "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"

#define ALPHABET_SIZE 105 // russian letters = 2 bytes

typedef struct {
    size_t size;
    int N;
    char* raw;
    char* cypher;
    char alphabet[ALPHABET_SIZE];
} buffer;

buffer*
buffer_alloc(const char* str);

void
buffer_free(buffer* ptr);



// TODO: encrypt
// TODO: decrypt

#endif
