package main

import (
	"fmt"
	"mizski/lab2/factorization"
	"mizski/lab2/util"
)

func main() {
	var res, err = factorization.TrialDivision(105)
	if err != nil {
		util.Error("%s", err.Error())
	}
	fmt.Printf("%v\n", res)
	var res2 = factorization.FermatFactors(105)
	fmt.Printf("%v\n", res2)
}
