#ifndef MATH_H
#define MATH_H

#include <cstdint>

/**
 * @def i64
 * @brief Short alias for std::int64_t.
 */
#define i64 std::int64_t

/**
 * @def u64
 * @brief Short alias for std::uint64_t.
 */
#define u64 std::uint64_t

namespace math {
    /**
    * @brief Computes Euler's totient function (phi) using brute force.
    * @details Works only on numbers < 500_000. Computes boolean array of composite flags durint compile time. Faster than bruteforce.
    * @param number Input integer.
    * @return Euler's totient of number.
    */
    u64
    euler_bruteforce(u64 number);

    /**
    * @brief Competes Euler's totient function (phi) usign Sieve of Eratosthene
    * @param number Input integer.
    * @return Euler's totient of number.
    */
    u64
    euler_sieve(u64 number);
}

#endif
