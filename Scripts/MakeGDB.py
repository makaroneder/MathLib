from sys import argv

if __name__ == "__main__":
    if len(argv) < 5:
        print(f"Usage: {argv[0]} <output file> <symbol file> <breakpoint> <commands>")
        exit(1)
    with open(argv[1], "w") as file:
        file.write("set disassembly-flavor intel\n")
        file.write(f"symbol-file {argv[2]}\n")
        file.write(f"break *{argv[3]}\n")
        file.write(f"target remote | {argv[4]} -S -gdb stdio\n")
        file.write("continue\n")