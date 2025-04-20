def levenshtein(s1, s2):
    """Compute the Levenshtein distance between two strings."""
    if len(s1) < len(s2):
        return levenshtein(s2, s1)

    previous_row = list(range(len(s2) + 1))
    for i, c1 in enumerate(s1):
        current_row = [i + 1]
        for j, c2 in enumerate(s2):
            insertions = previous_row[j + 1] + 1
            deletions = current_row[j] + 1
            substitutions = previous_row[j] + (c1 != c2)
            current_row.append(min(insertions, deletions, substitutions))
        previous_row = current_row

    return previous_row[-1]

def read_file(path):
    """Read the entire contents of a file as a single string."""
    with open(path, 'rb') as f:  # open in binary mode to avoid decode errors
        return f.read()

def main(file1, file2):
    text1 = read_file(file1)
    text2 = read_file(file2)
    distance = levenshtein(text1, text2)
    
    max_len = max(len(text1), len(text2))
    if max_len == 0:
        percent_match = 100.0
    else:
        percent_match = (1 - distance / max_len) * 100

    print(f"Edit distance between source and target: {distance}")
    print(f"Percentage match: {percent_match:.2f}%")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <file1> <file2>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])

