#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <stddef.h>

#define PUNCTUATION ",.!?;:-()[]{}\"'`@#%^&+=/\\|<>~"
#define DIGIT       "0123456789"
#define LETTER      "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"

typedef struct {
    char* raw;
    size_t size;
    char* cypher;
} buffer;

buffer*
buffer_alloc(const char* str);

void
buffer_free(buffer* ptr);



// TODO: encrypt
// TODO: decrypt

#endif
