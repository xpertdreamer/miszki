package main

import "mizski/lab2/util"

func main() {
	defer util.Measure("main")()
	teststring := "Test"
	util.Debug("Test debug fmt %s", teststring)
	//util.Error("Error test")
	util.Debug("Unreachable")
}
