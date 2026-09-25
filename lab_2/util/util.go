package util

import (
	"log"
	"os"
	"runtime"
)

var DebugMode = "0"

var (
	ErrorColor string = "\033[1;31m"
	DebugColor = "\033[1;34m"
	ResetColor = "\033[0m"
)

var (
	debugLogger = log.New(os.Stderr, "", 1)
	errorLogger = log.New(os.Stderr, "", 1)
)

func init() {
	if runtime.GOOS == "windows" {
		ErrorColor = ""
		DebugColor = ""
		ResetColor = ""
	}
	debugLogger.SetPrefix(DebugColor + "DEBUG" + ResetColor + " ")
	errorLogger.SetPrefix(ErrorColor + "ERROR" + ResetColor + " ")
}

func Debug(format string, a ...any) {
	if (DebugMode == "true" || DebugMode == "1") {
		debugLogger.Printf(format, a...)
	}
}

func Error(format string, a ...any) {
	errorLogger.Fatalf(format, a...)
}
