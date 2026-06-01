from sys import argv, exit
from pathlib import Path

if __name__ == "__main__":
    if len(argv) < 2:
        print(f"Usage: {argv[0]} <directory>")
        exit(1)
    Path(argv[1]).mkdir(parents = True, exist_ok = True)