package factorization

import (
	"errors"
	"math"
	"mizski/lab2/util"
)

const trialDivisiomMinimum = 2

func TrialDivision(n uint) ([]uint, error) {
	util.Debug("Call [TrialDivision]\tn=%d", n)
	defer util.Measure("TrialDivision")()
	var result []uint
	if n < trialDivisiomMinimum {
		return nil, errors.New("TrialDivision: given number must be greater than 1")
	}
	for i := uint(trialDivisiomMinimum); i < n; i++ {
		for n % i == 0 {
			util.Debug("[TrialDivision] result += %d", i)
			result = append(result, i)
			n /= i
		}
	}
	if n >= trialDivisiomMinimum {
		util.Debug("[TrialDivision] result += %d", n)
		result = append(result, n)
	}
	return result, nil
}

func FermatFactors(n uint) []uint {
	util.Debug("Call [TrialDivision]\tn=%d", n)
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
