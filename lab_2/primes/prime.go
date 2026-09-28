package primes

import (
	"math/big"
	"mizski/lab2/util"
	"slices"
)

// buildSieve builds a slice of all primes up to n using the Sieve of Eratosthenes.
// Parameter n is the upper bound (inclusive).
// Returns a slice of prime numbers in ascending order.
func buildSieve(n uint) []uint {
	var sieve []bool = make([]bool, n + 1)
	var result []uint
	for i := range sieve {
		sieve[i] = true
	}
	for i := uint(2); i * i <= n; i++ {
		if (sieve[i] == true) {
			for j := i * i; j <= n; j += i {
				sieve[j] = false
			}
		}
	}
	for p := uint(2); p <= n; p++ {
		if sieve[p] {
			result = append(result, p)
		}
	}
	return result
}

// SieveTest checks whether n is prime using the Sieve of Eratosthenes.
// Parameter n is the number to test.
// Returns true if n is prime, false otherwise (including n <= 1).
func SieveTest(n uint) bool {
	util.Debug("Call TestSieve\tnumber=%d", n)
	defer util.Measure("TestSieve")()
	if n <= 1 {
		util.Debug("(TestSieve) Returning false")
		return false
	}
	var sieve = buildSieve(n)
	if slices.Contains(sieve, n) == false {
		util.Debug("(TestSieve) Builded sieve %v doesnt contains %d", sieve, n)
		return false
	}
	util.Debug("(TestSieve) Sieve %v contains %d", sieve, n)
	return true
}

// PerfectTestBruteForce checks whether n is a perfect number by brute force.
// Parameter n is the number to test.
// Returns true if n equals the sum of its proper divisors, false otherwise.
func PerfectTestBruteForce(n uint) bool {
	util.Debug("Call PerfectTestBruteForce\tnumber=%d", n)
	defer util.Measure("PerfectTestBruteForce")()
	if n % 2 != 0 {
		util.Debug("(PerfectTestBruteForce) Returning false (num is odd)")
		return false
	}
	var sum uint = 0;
	for i := uint(1); i < n; i++ {
		if n % i == 0 {
			sum += i
		}
		if (sum > n) {
			return false;
		}
	}
	return sum == n
}

// mersennNum computes the p-th even perfect number using Euclid's formula: 2^(p-1) * (2^p - 1).
// Parameter p is the exponent of the Mersenne prime (must be prime).
// Returns a pointer to a big.Int holding the perfect number.
func mersennNum(p uint) *big.Int {
	var one = big.NewInt(1)
	var two = big.NewInt(2)
	var bigP = big.NewInt(int64(p))
	var bigPMinus1 = big.NewInt(int64(p - 1))
	var mersennePrime = new(big.Int).Exp(two, bigP, nil)
	mersennePrime.Sub(mersennePrime, one)
	multiplier := new(big.Int).Exp(two, bigPMinus1, nil)
	return new(big.Int).Mul(multiplier, mersennePrime)
}

// PerfectTestEuclid checks whether n is an even perfect number using Euclid's-Euler theorem.
// Parameter n is the number to test.
// Returns true if n equals 2^(p-1) * (2^p - 1) for some Mersenne prime exponent p, false otherwise.
func PerfectTestEuclid(n *big.Int) bool {
	util.Debug("Call PerfectTestEuclid\tnumber=%s", n.String())
	defer util.Measure("PerfectTestEuclid")()
	if n.Sign() <= 0 {
		return false
	}
	var zero = big.NewInt(0)
	var two = big.NewInt(2)
	var rem = new(big.Int).Mod(n, two)
	if rem.Cmp(zero) != 0 {
		util.Debug("(PerfectTestEuclid) Returning false (num is odd)")
		return false
	}
	var primes = []uint{2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127}
	for _, p := range primes {
		var perf = mersennNum(p)
		if n.Cmp(perf) == 0 {
			return true
		}
		if perf.Cmp(n) > 0 {
			break
		}
	}
	return false
}
