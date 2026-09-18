#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>
#include <stdbool.h>

#define ENC_MODE "e"
#define DEC_MODE "d"

typedef enum {
   encryption = 0,
   decryption = 1
} mode;

/* typedef enum { */
/*    CLI = 0, */
/*    INT = 0 */
/* } ui; */

mode
match_mode(const char* arg);

wchar_t*
char_to_wchar(const char *c);

wchar_t*
regex_to_wchar(const wchar_t* range);

wchar_t*
read_file(const char* path);

bool
write_file(const char* path, const wchar_t* wstring);

#endif
