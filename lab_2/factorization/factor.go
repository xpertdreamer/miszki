package factorization

import (
	"errors"
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
