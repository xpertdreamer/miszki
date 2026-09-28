package factorization

import (
	"errors"
	"math"
	"mizski/lab2/util"
)

// trialDivisiomMinimum is the smallest divisor candidate used in trial division.
const trialDivisiomMinimum = 2

// TrialDivision factors n using the trial division method.
// Parameter n is the number to factorize (must be greater than 1).
// Returns a slice of prime factors in ascending order, or an error if n is less than 2.
func TrialDivision(n uint) ([]uint, error) {
	util.Debug("Call [TrialDivision]\tn=%d", n)
	defer util.Measure("TrialDivision")()
	var result []uint
	if n < trialDivisiomMinimum {
		return nil, errors.New("TrialDivision: given number must be greater than 1")
	}
	for i := uint(trialDivisiomMinimum); i < n; i++ {
		for n % i == 0 {
			util.Debug("(TrialDivision) result += %d", i)
			result = append(result, i)
			n /= i
		}
	}
	if n >= trialDivisiomMinimum {
		util.Debug("(TrialDivision) result += %d", n)
		result = append(result, n)
	}
	return result, nil
}

// FermatFactors finds a pair of factors of n using Fermat's factorization method.
// Parameter n is the number to factorize.
// Returns a slice of two factors: {2, n/2} for even n, or {a-b, a+b} for odd n where n = a^2 - b^2.
func FermatFactors(n uint) []uint {
	util.Debug("Call (TrialDivision)\tn=%d", n)
	defer util.Measure("FermatFactors")()
	// check if an even number given
	if (n & 0x01) == 0 {
		return []uint{2, n / 2}
	}
	var start uint = uint(math.Ceil(math.Sqrt(float64(n))))
	// check if its perfect root
	if (start * start) == n {
		return []uint{start, start}
	}
	var b uint
	for {
		var sqY uint = start * start - n
		b = uint(math.Round(math.Sqrt(float64(sqY))))
		if b * b == sqY {
			break
		} else {
			start += 1
		}
	}
	return []uint{start-b, start+b}
}
