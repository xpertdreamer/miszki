#include "math.h"
#include "util.h"

u64
math::euler_bruteforce(u64 number)
{
    START(euler_bruteforce);
    DEBUG("Call math::euler\tnumber=%ld", number);
    if (number == 0) return 0;
    u64 result = number;
    for (u64 i = 2; i * i <= number; ++i) {
        if (number % i == 0) {
            while (number % i == 0) number /= i;
            result -= result / i;
        }
    }
    if (number > 1) result -= result / number;
    END(euler_bruteforce);
    return result;
}
