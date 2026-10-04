#include "math.h"
#include "util.h"

i64
math::euler_bruteforce(i64 number)
{
    DEBUG("Call math::euler\tnumber=%ld", number);
    if (number <= 0) return 0;
    TODO("math::euler not implemented yet!");
    i64 result = number;
    for (i64 i = 2; i * i <= number; ++i) {
        if (number % i == 0) {
            while (number % i == 0) number /= i;
            result -= result / i;
        }
    }
    if (number > 1) result -= result / number;
    return result;
}
