#include "math.h"
#include <iostream>

int
main(void)
{
    i64 r = math::euler_bruteforce(3);
    std::cout << r << std::endl;
    return 0;
}
