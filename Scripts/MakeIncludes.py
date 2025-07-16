from sys import argv

if __name__ == "__main__":
    if len(argv) < 3:
        print(f"Usage: {argv[0]} <input files> <output file>")
        exit(1)
    includes : set[str] = set(argv[1:-1])
    with open(argv[-1], "w") as f:
        f.write("#ifndef MathLib_H\n")
        f.write("#define MathLib_H\n")
        for filename in includes: f.write(f"#include \"{filename}\"\n")
        f.write("#endif")