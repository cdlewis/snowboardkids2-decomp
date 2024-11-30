package main

import (
	"os"
	"strings"
	"bufio"
	"fmt"
	"strconv"
	"slices"
	"regexp"	

	"gopkg.in/yaml.v3"
)

/*
 * Very hacky script to calculate completion %
 */

var finishedCSegments = []string{}
var re = regexp.MustCompile(`start:\s*(0x[0-9A-Fa-f]+).*type:\s*([a-zA-Z0-9_]+)`)

type SplatConfig struct {
	Segments []Segment `yaml:"segments"`
}

type Segment struct {
	Subsegments [][]string `yaml:"subsegments"`
}

func main() {
	file, err := os.Open("../snowboardkids2.yaml")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	var yamlString string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		// adapt weirdly formatted bss segments
		matches := re.FindStringSubmatch(line)
		if len(matches) > 0 {
			padding := strings.Split(line, "-")
			line = fmt.Sprintf("%s- [%s, bss]", padding[0], matches[1])
		}

		// ignore final entry
		if strings.HasPrefix(line, "  - [") {
			continue
		}

		yamlString += line + "\n"
	}

	var config SplatConfig
	err = yaml.Unmarshal([]byte(yamlString), &config)
	if err != nil {
		panic(err)
	}

	codeSize := 0
	decompiledCodeSize := 0

	for _, segment := range config.Segments {
		for idx, subsegment := range segment.Subsegments {
				if idx == len(segment.Subsegments) - 1 {
					continue
				}

				currentSegment := parseSubSegment(subsegment)
				nextSegment := parseSubSegment(segment.Subsegments[idx + 1])

				segmentSize := nextSegment.Address - currentSegment.Address

				if segmentSize < 0 {
					panic("invalid segment")
				}

				if currentSegment.Finished {
					decompiledCodeSize += int(segmentSize)
				}

				codeSize += int(segmentSize)
		}
	}

	fmt.Println(float64(decompiledCodeSize) / float64(codeSize) * 100)


}

type Subsegment struct {
	Name string
	Address uint64
	Finished bool
}

func parseSubSegment(segment []string) Subsegment {
	address, err := strconv.ParseUint(segment[0], 0, 64)
	if err != nil {
		panic(err)
	}

	finished := false
	if segment[1] == "c" && slices.Contains(finishedCSegments, segment[2]) {
		finished = true
	}

	if segment[1] == "lib" {
		finished = true
	}

	return Subsegment{
		Name: strings.Join(segment, "_"),
		Address: address,
		Finished: finished,
	}

}