package main

import (
	"fmt"
	"os"
	"regexp"
	"slices"
	"strconv"
	"strings"

	"github.com/cdlewis/snowboardkids2-decomp/tools/cli/splat"
)

var re = regexp.MustCompile(`0x([0-9A-Fa-fA-F]+)`)

type Symbol struct {
	VRAMAddress int
	Line        int
}

func main() {
	splatFile := os.Args[1]
	symbolFile := os.Args[2]

	splatConfig, err := splat.ReadConfig(splatFile)
	if err != nil {
		panic(err)
	}

	rawFile, err := os.ReadFile(symbolFile)
	if err != nil {
		panic(err)
	}

	lines := strings.Split(string(rawFile), "\n")
	var symbols []Symbol
	for idx, l := range lines {
		addr, ok := castStringToHex(l)
		if !ok {
			continue
		}

		symbols = append(symbols, Symbol{
			VRAMAddress: addr,
			Line:        idx,
		})
	}

	slices.SortFunc(symbols, func(a, b Symbol) int {
		return a.VRAMAddress - b.VRAMAddress
	})

	var output string
	for _, segment := range splatConfig.Segments {
		offset := segment.VRAM

		for jdx, subsegment := range segment.Subsegments {
			if jdx == len(segment.Subsegments)-1 {
				continue
			}

			var resultBuffer string

			startAddress := subsegment.RomOffset + offset
			endAddress := segment.Subsegments[jdx+1].RomOffset + offset

			// flush anything that's less than the start address
			// something weird has happened (or address is just weird)
			flushed := false
			for len(symbols) > 0 && symbols[0].VRAMAddress < startAddress {
				output += lines[symbols[0].Line] + "\n"
				symbols = symbols[1:]
				flushed = true
			}
			if flushed {
				output += "\n"
			}

			for len(symbols) > 0 {
				curr := symbols[0]
				if curr.VRAMAddress >= startAddress && curr.VRAMAddress < endAddress {
					resultBuffer += lines[curr.Line] + "\n"
					symbols = symbols[1:]
					continue
				}

				break
			}

			if len(resultBuffer) > 0 {
				output += fmt.Sprintf("// %s \n\n", subsegment.Name)
				output += resultBuffer + "\n"
			}
		}
	}

	err = os.WriteFile(symbolFile, []byte(output), 0644)
	if err != nil {
		panic(err)
	}
}

func castStringToHex(s string) (int, bool) {
	matches := re.FindStringSubmatch(s)
	if len(matches) < 1 {
		return -1, false
	}

	address, err := strconv.ParseUint(matches[1], 16, 64)
	if err != nil {
		return -1, false
	}

	return int(address), true
}