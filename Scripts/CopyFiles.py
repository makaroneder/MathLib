from sys import argv, exit
from os import makedirs
from os import path

if __name__ == "__main__":
    if len(argv) < 3:
        print(f"Usage: {argv[0]} <input files> <output directory> <stripped directories>")
        exit(1)
    inputs : set[str] = set(argv[1:-2])
    strip : int = int(argv[-1])
    for input in inputs:
        with open(input, "rb") as inputFile:
            output : str = argv[-2] + '/' + str.join("/", input.split('/')[strip:])
            makedirs(path.dirname(output), exist_ok = True)
            with open(output, "wb") as outputFile:
                outputFile.write(inputFile.read())