#include "replacement.h"
#include "substitution.h"
#include "input.h"
#include "util.h"

#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <wchar.h>

#define HELP(str) do { fprintf(stderr, "Usage: %s [-a <alphabet_regex>  -s <shift>] [-u <path>||-d <path>] -m <e/d> -g <source>\n", (str)); return 1; } while(0)

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
    // u - upload table ;
    // d - download table ;
    bool a_providen = false, m_providen = false, s_providen = false, g_providen = false, u_providen = false, d_providen = false;
    int opt, s;
    mode m;
    wchar_t  *a = NULL, *g = NULL, *alphabet = NULL;
    const char* d_path = NULL, *u_path = NULL;
    while ((opt = getopt(argc, argv, "g:a:m:s:u:d:")) != -1) {
        switch (opt) {
            case 'a': {
                a = char_to_wchar(optarg);
                if (a == NULL) {
                    ERROR("Failed to allocate wchar");
                    FREE_WCHAR(a);
                    FREE_WCHAR(g);
                    FREE_WCHAR(alphabet);
                    return 1;
                }
                alphabet = regex_to_wchar(a);
                if (alphabet == NULL) {
                    ERROR("Failed to turn regex to wchar");
                    FREE_WCHAR(a);
                    FREE_WCHAR(g);
                    FREE_WCHAR(alphabet);
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
                    FREE_WCHAR(alphabet);
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
                    FREE_WCHAR(g);
                    FREE_WCHAR(alphabet);
                    return 1;
                }
                g_providen = true;
                break;
            }
            case 'u': u_path = optarg; u_providen = true; break;
            case 'd': d_path = optarg; d_providen = true; break;
            default: HELP(argv[0]);
        }
    }
    bool group1 = a_providen && s_providen && m_providen && g_providen && !u_providen && !d_providen;
    bool group2 = (u_providen || d_providen) && m_providen && g_providen && !a_providen && !s_providen;
    if (group1 == true) {
        buffer* buf;
        buf = buffer_alloc(g);
        if (buf == NULL) {
            ERROR("Failed to allocate buffer");
            FREE_WCHAR(a);
            FREE_WCHAR(g);
            FREE_WCHAR(alphabet);
            return 1;
        }
        build_alphabet(buf, alphabet);
        switch (m) {
            case encryption: encrypt(buf, s); break;
            case decryption: decrypt(buf, s); break;
        }
        PRINT_WCHAR(buf->cypher);
        buffer_free(buf);
    }
    else if (group2 == true) {
        replacement_buffer* buf = replace_buf_alloc(g);
        if (buf == NULL) {
            ERROR("Failed to allocate buffer");
            FREE_WCHAR(a);
            FREE_WCHAR(g);
            FREE_WCHAR(alphabet);
            return 1;
        }
        replace_table *table = build_table(u_path);
        if (table == NULL) {
            ERROR("Failed to allocate buffer");
            FREE_WCHAR(a);
            FREE_WCHAR(g);
            FREE_WCHAR(alphabet);
            free(buf);
            return 1;
        }
        switch (m) {
            case encryption: r_encrypt(table, buf); break;
            case decryption: r_decrypt(table, buf); break;
        }
        PRINT_WCHAR(buf->processed);
        if (d_providen == true) save_replace_table(table, d_path);
        free_workspace(table, buf);
    }
    else {
        ERROR("Some arguments not providen, or group of flags is incorrect");
        FREE_WCHAR(a);
        FREE_WCHAR(g);
        FREE_WCHAR(alphabet);
        HELP(argv[0]);
    }
    FREE_WCHAR(a);
    FREE_WCHAR(g);
    FREE_WCHAR(alphabet);
    return 0;
}
