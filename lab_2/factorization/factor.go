package factorization

import (
	"errors"
	"mizski/lab2/util"
)

func TrialDivision(n uint) ([]uint, error) {
	defer util.Measure("TrialDivision")()
	var result []uint
	if n < 2 {
		return nil, errors.New("TrialDivision: given number must be greater than 1")
	}
	for i := uint(2); i < n; i++ {
		for n % i == 0 {
			result = append(result, i)
			n /= i
		}
	}
	if n > 1 {
		result = append(result, n)
	}
	return result, nil
}
