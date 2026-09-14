#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <stddef.h>
#include <wchar.h>

#define PUNCTUATION L",.!?;:-()[]{}\"'`@#%^&+=/\\|<>~"
#define DIGIT       L"0123456789"
#define LETTER      L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя"

#define ALPHABET_SIZE 150

typedef struct {
    size_t size;
    int N;
    wchar_t* raw;
    wchar_t* cypher;
    wchar_t alphabet[ALPHABET_SIZE];
} buffer;

buffer*
buffer_alloc(const wchar_t* str);

void
buffer_free(buffer* ptr);

void
build_alphabet(buffer* buf, const wchar_t* given);

// TODO: encrypt
// TODO: decrypt

#endif
