package main

import (
	"github.com/pborman/getopt/v2"
	"os"
)

var (
	helpFlag = getopt.BoolLong("help", 'h', "display help")
)

func main() {
	getopt.Parse()
	if *helpFlag {
		getopt.Usage()
		os.Exit(0)
	}
}
