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

std::pair<u64, std::pair<i64, i64>>
math::extended_gcd(u64 a, u64 b)
{
    DEBUG("Call extended_gcd\ta=%lu,\tb=%lu", a, b);
    START(extended_gcd);
    if (a == 0) {
        return {b, {0, 1}};
    }
    if (b == 0) {
        return {a, {1, 0}};
    }
    i64 r_prev2 = static_cast<i64>(a);
    i64 r_prev1 = static_cast<i64>(b);
    // k_0 = 1,  k_1 = 0
    i64 k_prev2 = 1, k_prev1 = 0;
    // m_0 = 0,  m_1 = 1
    i64 m_prev2 = 0, m_prev1 = 1;
    while (r_prev1 != 0) {
        i64 q = r_prev2 / r_prev1;        // q_{i-1}
        i64 k_i = k_prev2 - q * k_prev1;  // k_i = k_{i-2} - q_{i-1} * k_{i-1}
        i64 r_i = r_prev2 - q * r_prev1;  // r_i = r_{i-2} - q_{i-1} * r_{i-1}
        i64 m_i = m_prev2 - q * m_prev1;  // m_i = m_{i-2} - q_{i-1} * m_{i-1}
        // (i-2, i-1) = (i-1, i)
        r_prev2 = r_prev1;  r_prev1 = r_i;
        k_prev2 = k_prev1;  k_prev1 = k_i;
        m_prev2 = m_prev1;  m_prev1 = m_i;
    }
    END(extended_gcd);
    return {static_cast<u64>(r_prev2), {k_prev2, m_prev2}};
}

u64
math::crt(const std::vector<u64>& ms, const std::vector<u64>& rs)
{
    DEBUG("Call math::crt\tcount=%zu", ms.size());
    START(crt);
    if (ms.empty() || ms.size() != rs.size()) {
        END(crt);
        return 0;
    }
    u64 M = 1;
    for (u64 a : ms) M *= a;
    u64 result = 0;
    for (std::size_t i = 0; i < ms.size(); ++i) {
        u64 a_i = ms[i];
        u64 r_i = rs[i];
        u64 M_i = M / a_i;
        auto [g, km] = math::extended_gcd(M_i, a_i);
        if (g != 1) { END(crt); return 0;}
        auto k = km.first;
        i64 ai = static_cast<i64>(a_i);
        u64 N_i = static_cast<u64>(((k % ai) + ai) % ai) ;
        u64 t = (r_i * M_i) % M;
        result = (result + t * N_i % M) % M;
    }
    END(crt);
    return result;
}
