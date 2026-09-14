#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

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

#endif
