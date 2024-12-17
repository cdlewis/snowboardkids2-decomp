package splat

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"

	"gopkg.in/yaml.v2"
)

var re = regexp.MustCompile(`(?i)([a-z_]+): ([a-z_0-9\.]+)`)

type yamlConfig struct {
	Segments []yamlSegment `yaml:"segments"`
}

type yamlSegment struct {
	VRAM        string           `yaml:"vram"`
	Start       string           `yaml:"start"`
	Subsegments []yamlSubsegment `yaml:"subsegments"`
}

type yamlSubsegment []string

type Config struct {
	Segments []Segment
}

type Segment struct {
	VRAM        int
	Start       int
	Subsegments []Subsegment
}

type Subsegment struct {
	RomOffset   int
	SegmentType string
	Name        string
}

func configFromYAML(input yamlConfig) Config {
	var segments []Segment
	for _, i := range input.Segments {
		segments = append(segments, segmentFromYAML(i))
	}
	return Config{
		Segments: segments,
	}
}

func segmentFromYAML(input yamlSegment) Segment {
	vram := 0
	if input.VRAM != "" {
		parsedVram, err := strconv.ParseUint(input.VRAM, 0, 64)
		if err != nil {
			fmt.Println(input)
			panic("failed to parse splat config: " + err.Error())
		}
		vram = int(parsedVram)
	}

	start, err := strconv.ParseUint(input.Start, 0, 64)
	if err != nil {
		panic("failed to parse splat config: " + err.Error())
	}

	var subsegments []Subsegment
	for _, i := range input.Subsegments {
		subsegments = append(subsegments, subsegmentFromYAML(i))
	}

	return Segment{
		Start:       int(start),
		VRAM:        int(vram),
		Subsegments: subsegments,
	}
}

func subsegmentFromYAML(input yamlSubsegment) Subsegment {
	romOffset, err := strconv.ParseUint(input[0], 0, 64)
	if err != nil {
		panic("failed to parse yaml: " + err.Error())
	}

	name := input[0]
	if input[1] == "lib" && len(input) == 4 {
		name = input[3]
	} else if input[1] == "lib" && len(input) == 5 {
		name = input[3] + "(" + input[4] + ")"
	} else if len(input) > 2 {
		name = input[2]
	}

	return Subsegment{
		Name:        name,
		RomOffset:   int(romOffset),
		SegmentType: input[1],
	}
}

func ReadConfig(filename string) (Config, error) {

	file, err := os.Open(filename)
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

		yamlString += line + "\n"
	}

	var config yamlConfig
	err = yaml.Unmarshal([]byte(yamlString), &config)

	return configFromYAML(config), err
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
