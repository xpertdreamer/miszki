#include "math.h"
#include "util.h"

namespace {
    struct Sieve {
        bool data[SIEVE_LIMIT + 1];
    };

    constexpr Sieve
    build_sieve()
    {
        Sieve sieve{};
        sieve.data[0] = true;
        sieve.data[1] = true;
        for (std::size_t i = 2; i <= SIEVE_LIMIT / i; ++i)
            if (!sieve.data[i])
                for (std::size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve.data[j] = true;
        return sieve;
    }

    constexpr Sieve COMPOSITE = build_sieve();
}

u64
math::euler_bruteforce(u64 number)
{
    START(euler_bruteforce);
    DEBUG("Call math::euler_bruteforce\tnumber=%ld", number);
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

u64
math::euler_sieve(u64 number)
{
    DEBUG("Call math::euler_sieve\tnumber=%ld", number);
    if (number > static_cast<u64>(SIEVE_LIMIT) * SIEVE_LIMIT) {
        DEBUG("number too large for sieve");
        return 0;
    }
    START(euler_sieve);
    const bool* s = COMPOSITE.data;
    if (number == 0) return 0;
    u64 result = number;
    for (u64 i = 2; i * i <= number; ++i) {
        if (s[i]) continue;
        if (number % i == 0) {
            while (number % i == 0) number /= i;
            result -= result / i;
            if (number == 1) break;
        }
    }
    if (number > 1) result -= result / number;
    END(euler_sieve);
    return result;
}
