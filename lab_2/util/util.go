package util

import (
	"fmt"
	"log"
	"os"
	"runtime"
	"time"
)

var DebugMode = "0"

var (
	errorColor string = "\033[1;31m"
	debugColor = "\033[1;34m"
	resetColor = "\033[0m"
)

var (
	debugLogger = log.New(os.Stderr, "", 1)
	errorLogger = log.New(os.Stderr, "", 1)
)

func init() {
	if runtime.GOOS == "windows" {
		errorColor = ""
		debugColor = ""
		resetColor = ""
	}
	debugLogger.SetPrefix(debugColor + "DEBUG" + resetColor + " ")
	errorLogger.SetPrefix(errorColor + "ERROR" + resetColor + " ")
}

func Debug(format string, a ...any) {
	if (DebugMode == "true" || DebugMode == "1") {
		debugLogger.Printf(format, a...)
	}
}

func Error(format string, a ...any) {
	errorLogger.Fatalf(format, a...)
}

func Measure(name string) func() {
	var start = time.Now()
	return func() {
		fmt.Printf("Timer [%s] took: %v\n", name, time.Since(start))
	}
}
