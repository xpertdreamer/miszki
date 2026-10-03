#include "math.h"
#include <iostream>

int
main(void)
{
    i64 r = math::euler(16);
    std::cout << r << std::endl;
    return 0;
}
