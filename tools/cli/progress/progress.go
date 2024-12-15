package main

import (
	"fmt"
	"os"
	"slices"

	"github.com/cdlewis/snowboardkids2-decomp/tools/cli/splat"
)

/*
 * Very hacky script to calculate completion %
 */

var finishedCSegments = []string{}

func main() {
	config, err := splat.ReadConfig(os.Args[1])
	if err != nil {
		panic(err)
	}

	codeSize := 0
	decompiledCodeSize := 0

	for _, segment := range config.Segments {
		for idx, subsegment := range segment.Subsegments {
			if idx == len(segment.Subsegments)-1 {
				continue
			}

			currentSegment := parseSubSegment(subsegment)
			nextSegment := parseSubSegment(segment.Subsegments[idx+1])

			segmentSize := nextSegment.Address - currentSegment.Address

			if currentSegment.Finished {
				decompiledCodeSize += int(segmentSize)
			}

			codeSize += int(segmentSize)
		}
	}

	fmt.Println(float64(decompiledCodeSize) / float64(codeSize) * 100)
}

type Subsegment struct {
	Address  uint64
	Finished bool
}

func parseSubSegment(segment splat.Subsegment) Subsegment {
	finished := false
	if segment.SegmentType == "c" && slices.Contains(finishedCSegments, segment.Name) {
		finished = true
	}

	if segment.SegmentType == "lib" || segment.SegmentType == "pad" {
		finished = true
	}

	return Subsegment{
		Address:  uint64(segment.RomOffset),
		Finished: finished,
	}

}
