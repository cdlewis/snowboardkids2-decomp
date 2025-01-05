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
var re = regexp.MustCompile(`(?i)([a-z_]+): ([a-z_0-9\.]+)`)

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
		matches := parseMapFields(line)

		if matches["type"] == "bss" {
			if matches["start"] == "" {
				continue
			}

			padding := strings.Split(line, "-")
			line = fmt.Sprintf("%s- [%s, bss]", padding[0], matches["start"])
		}

		if matches["section"] == ".bss" {
			continue
		}

		// ignore final entry
		if strings.HasPrefix(line, "  - [") {
			continue
		}
		// fmt.Println(line)
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

	if segment[1] == "lib" || segment[1] == "pad" || segment[1] == "textbin" || segment[1] == "rodatabin" {
		finished = true
	}

	return Subsegment{
		Name: strings.Join(segment, "_"),
		Address: address,
		Finished: finished,
	}

}

func parseMapFields(line string) map[string]string {
	matches := re.FindAllStringSubmatch(line, -1)
	results := map[string]string{}
	for _, i := range matches {
		if len(i) < 3 {
			continue
		}
		results[i[1]] = i[2]
	}
	return results
}