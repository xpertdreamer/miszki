package main

import (
	"fmt"
	"math/big"
	"os"

	"mizski/lab2/factorization"
	"mizski/lab2/primes"
	"mizski/lab2/util"

	"github.com/pborman/getopt"
)

const (
	td = "td"
	ferma = "f"
	sieve = "s"
	bruteforce = "b"
	euclid = "ee"
)

var (
	optNum = getopt.StringLong("num", 'n', "2", "a number fed to the algorithm as input")
	optAlg = getopt.StringLong("alg", 'a', "td", "an algorithm (td=trial-division, f=ferma, s=sieve, b=perfect-bruteforce, ee=euclid-euler)")
	helpFlag = getopt.BoolLong("help", '?', "display help")
)

func main() {
	getopt.Parse()
	if *helpFlag {
		getopt.Usage()
		os.Exit(0)
	}
	var inputNum = new(big.Int)
	if _, ok := inputNum.SetString(*optNum, 10); !ok {
		util.Error("Failed to fetch number (%s)", *optNum)
	}
	switch *optAlg {
	case td: {
		util.Debug("Calling td")
		if !inputNum.IsUint64() {
			util.Error("Number is too big for Trial Division")
		}
		var res, err = factorization.TrialDivision(uint(inputNum.Uint64()))
		if err != nil {
			util.Error("%s", err.Error())
		}
		fmt.Printf("%v\n", res)
		break;
	}
	case ferma: {
		util.Debug("Calling fermat")
		var res = factorization.FermatFactors(uint(inputNum.Uint64()))
		fmt.Printf("%v\n", res)
		break;
	}
	case sieve: {
		util.Debug("Calling sieve")
		var	res bool = primes.SieveTest(uint(inputNum.Uint64()))
		fmt.Printf("prime? %t\n", res)
		break;
	}
	case bruteforce: {
		util.Debug("Calling bruteforce for perfect")
		var res bool = primes.PerfectTestBruteForce(uint(inputNum.Uint64()))
		fmt.Printf("perfect? %t\n", res)
		break;
	}
	case euclid: {
		util.Debug("Calling Euclid-Euler method for perfect")
		var res bool = primes.PerfectTestEuclid(inputNum)
		fmt.Printf("perfect? %t\n", res)
		break;
	}
	}
}
