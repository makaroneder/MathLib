from sys import argv
from subprocess import run
from os import path, remove

if __name__ == "__main__":
    if len(argv) < 3:
        print(f"Usage: {argv[0]} <input file> <output file> <flags>")
        exit(1)
    strings : int = 0
    tmpPath : str = argv[1] + ".tmp.asl"
    outputPath : str = path.relpath(argv[2], start = path.dirname(tmpPath))
    with open(tmpPath, "w") as tmpFile:
        with open(argv[1], "r") as inputFile:
            while True:
                chr : str = inputFile.read(1)
                if not chr: break
                if chr == '"':
                    strings += 1
                    if strings == 1:
                        tmpFile.write('"' + outputPath)
                if strings == 0 or strings >= 2: tmpFile.write(chr)
    if run(["iasl"] + argv[3:] + [tmpPath]).returncode:
        print("Failed to compile ASL file")
        exit(1)
    remove(tmpPath)