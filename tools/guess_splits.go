package main

import (
    "bufio"
    "fmt"
    "os"
    "strings"
)

func match(line string) (string, bool) {
	if strings.HasPrefix(line, "dlabel") {
		return "data", true
	}

	if strings.HasPrefix(line, "glabel") {
		return "asm", true
	}

	return "", false
}

func main() {
    filePath := os.Args[1]
    readFile, err := os.Open(filePath)
    defer readFile.Close()

    if err != nil {
        fmt.Println(err)
    }
    fileScanner := bufio.NewScanner(readFile)
    fileScanner.Split(bufio.ScanLines)

    size := 0
    for fileScanner.Scan() {
	    size++
	    line := fileScanner.Text()

	    if sType, ok := match(line); ok {
		    tokens := strings.Split(line, "_")
		    if size > 10000 {
			    sType = "bin"
		    }
		    if size < 100 {
			    continue
		    }
		    fmt.Printf(" - [0x%s, %s]\n", tokens[len(tokens)-1], sType)

		    size = 0
	    }
    }
}
