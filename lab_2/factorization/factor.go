package factorization

import (
	"errors"
	"mizski/lab2/util"
)

const trialDivisiomMinimum = 2

func TrialDivision(n uint) ([]uint, error) {
	defer util.Measure("TrialDivision")()
	var result []uint
	if n < trialDivisiomMinimum {
		return nil, errors.New("TrialDivision: given number must be greater than 1")
	}
	for i := uint(trialDivisiomMinimum); i < n; i++ {
		for n % i == 0 {
			result = append(result, i)
			n /= i
		}
	}
	if n >= trialDivisiomMinimum {
		result = append(result, n)
	}
	return result, nil
}
