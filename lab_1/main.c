#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int opt;
    // (require args):
    // a - alphabet ;
    // m - mode ;
    // s - shift ;
    while ((opt = getopt(argc, argv, "a:m:s:"))!= -1) {
        switch (opt) {
            case 'a': break;
            case 'm': break;
            case 's': break;
            default: fprintf(stderr, "Usage: %s -a <alphabet> -m <encrypt/decrypt> -s <shift>", argv[0]); return 1;
        }
    }
    return 0;
}
