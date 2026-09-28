package primes

import (
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
