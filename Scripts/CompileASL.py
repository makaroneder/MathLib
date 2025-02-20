import os
import sys
import subprocess

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input file> <output file> <flags>")
        exit(1)
    strings = 0
    tmpPath = sys.argv[1] + ".tmp.asl"
    outputPath = os.path.relpath(sys.argv[2], start = os.path.dirname(tmpPath))
    with open(tmpPath, "w") as tmpFile:
        with open(sys.argv[1], "r") as inputFile:
            while True:
                chr = inputFile.read(1)
                if not chr: break
                if chr == '"':
                    strings += 1
                    if strings == 1:
                        tmpFile.write('"' + outputPath)
                if strings == 0 or strings >= 2: tmpFile.write(chr)
    if subprocess.run(["iasl"] + sys.argv[3:] + [tmpPath]).returncode:
        print("Failed to compile ASL file")
        exit(1)
    os.remove(tmpPath)