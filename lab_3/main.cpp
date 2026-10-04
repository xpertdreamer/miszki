#include "math.h"
#include <iostream>

int
main(void)
{
    u64 r = math::euler_bruteforce(1e6);
    std::cout << r << std::endl;
    u64 l = math::euler_sieve(1e6);
    std::cout << l << std::endl;
    return 0;
}
