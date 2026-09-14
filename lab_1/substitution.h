#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <stddef.h>
#include <wchar.h>

#define PUNCTUATION L",.!?;:-()[]{}\"'`@#%^&+=/\\|<>~"
#define DIGIT       L"0123456789"
#define LETTER      L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя"

// 4 * (31+10+33) = 296 : Linux
// 2 * (31+10+33) = 148 : Windows
#define ALPHABET_SIZE 296

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

void
encrypt(buffer* buf, int shift);

void
decrypt(buffer* buf, int shift);


#endif
