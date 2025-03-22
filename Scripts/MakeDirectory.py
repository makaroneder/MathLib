from pathlib import Path
from sys import argv

if __name__ == "__main__":
    if len(argv) < 2:
        print(f"Usage: {argv[0]} <directory>")
        exit(1)
    Path(argv[1]).mkdir(parents = True, exist_ok = True)