#ifndef MATH_H
#define MATH_H

#include <cstdint>
#include <utility>
#include <vector>

/**
 * @typedef i64
 * @brief Short alias for std::int64_t.
 */
typedef std::int64_t i64;

/**
 * @typedef u64
 * @brief Short alias for std::uint64_t.
 */
typedef std::uint64_t u64;

/**
 * @def SIEVE_LIMIT
 * @brief Constant value used to generate Sieve of Eratosthenes at compile-time
 */
#define SIEVE_LIMIT 500'000

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

    /**
    * @brief Extended Euclid's algorithm (ax + by = gcd(x, y))
    * @param a First unsigned integer
    * @param b Second unsigned integer
    * @return Signature: pair.first = greatest common divisor of a and b; pair.second.first = x; pair.second.second = y
    */
    std::pair<u64, std::pair<i64, i64>>
    extended_gcd(u64 a, u64 b);

    /**
    * @brief Chinese Remainder Theorem using direct construction
    * @details If numbers are non coprime integers returns 0
    * @param ms moduli
    * @param rs remainders
    * @return x in [0; M] satisfying every congruence, or 0 when the moduli are not pairwise coprime
    */
    u64
    crt(const std::vector<u64>& ms, const std::vector<u64>& rs);
}

#endif
