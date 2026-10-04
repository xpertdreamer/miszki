#ifndef MATH_H
#define MATH_H

#include <cstdint>

/**
 * @def i64
 * @brief Short alias for std::int64_t.
 */
#define i64 std::int64_t

namespace math {
    /**
    * @brief Computes Euler's totient function (phi) using brute force.
    * @param number Input integer.
    * @return Euler's totient of number.
    */
    i64
    euler_bruteforce(i64 number);

    i64

}

#endif
