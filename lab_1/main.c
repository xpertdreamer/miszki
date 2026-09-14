#include "substitution.h"
#include "input.h"
#include "util.h"

#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <wchar.h>

#define HELP(str) do { fprintf(stderr, "Usage: %s -a <alphabet> -m <encrypt/decrypt> -s <shift> -g <source>\n", (str)); return 1; } while(0)

int main(int argc, char** argv)
{
    if (!setlocale(LC_ALL, "C.UTF-8")) {
        ERROR("setlocale failed — wide output will be empty");
        return 1;
    }
    // (require args):
    // a - alphabet ;
    // m - mode ;
    // s - shift ;
    // g - given source ;
    bool a_providen = false, m_providen = false, s_providen = false, g_providen = false;
    int opt, s;
    mode m;
    wchar_t  *a, *g;
    while ((opt = getopt(argc, argv, "g:a:m:s:")) != -1) {
        switch (opt) {
            case 'a': {
                a = char_to_wchar(optarg);
                if (a == NULL) {
                    ERROR("Failed to allocate wchar");
                    FREE_WCHAR(g);
                    return 1;
                }
                a_providen = true;
                break;
            }
            case 'm': m = match_mode(optarg); m_providen = true; break;
            case 's': {
                int arg = atoi(optarg);
                if (arg <= 0) {
                    ERROR("Shift cannot be negative or equal to zero");
                    FREE_WCHAR(a);
                    FREE_WCHAR(g);
                    return 1;
                }
                s = arg;
                s_providen = true;
                break;
            }
            case 'g': {
                g = char_to_wchar(optarg);
                if (g == NULL) {
                    ERROR("Failed to allocate wchar");
                    FREE_WCHAR(a);
                    return 1;
                }
                g_providen = true;
                break;
            }
            default: HELP(argv[0]);
        }
    }
    if ((a_providen && m_providen && s_providen && g_providen) == false) {
        ERROR("Some arguments not providen");
        FREE_WCHAR(a);
        FREE_WCHAR(g);
        HELP(argv[0]);
    }
    buffer* buf;
    buf = buffer_alloc(g);
    if (buf == NULL) {
        ERROR("Failed to allocate buffer");
        FREE_WCHAR(a);
        FREE_WCHAR(g);
        return 1;
    }
    build_alphabet(buf, a);
    switch (m) {
        case encryption: encrypt(buf, s); break;
        case decryption: decrypt(buf, s); break;
    }
    PRINT_WCHAR(buf->cypher);
    buffer_free(buf);
    FREE_WCHAR(a);
    FREE_WCHAR(g);
    return 0;
}
