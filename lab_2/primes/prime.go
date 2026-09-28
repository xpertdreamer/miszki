package primes

import (
	"math/big"
	"mizski/lab2/util"
	"slices"
)

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
