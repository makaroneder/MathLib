import sys

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input files> <output file>")
        exit(1)
    includes = sys.argv[1:-1]
    with open(sys.argv[-1], "w") as f:
        f.write("#ifndef MathLib_H\n")
        f.write("#define MathLib_H\n")
        for filename in includes: f.write(f"#include \"{filename}\"\n")
        f.write("#endif")