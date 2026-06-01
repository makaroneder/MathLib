from sys import argv, exit
from pathlib import Path

def ConvertName(path : str, extension : bool) -> str:
    tmp : Path = Path(path)
    return (tmp.name if extension else tmp.stem).replace("-", "_").replace(".", "_").replace(" ", "_")
if __name__ == "__main__":
    if len(argv) < 3:
        print(f"Usage: {argv[0]} <source output file> <header output file> <input files>")
        exit(1)
    inputs : set[str] = set(argv[3:])
    with open(argv[1], "w") as sourceOutput:
        sourceOutput.write("#include \"ExternArray.hpp\"\n\nnamespace MathLib {\n")
        with open(argv[2], "w") as headerOutput:
            tmp : str = ConvertName(argv[2], False)
            headerOutput.write(f"#ifndef MathLib_{tmp}_H\n#define MathLib_{tmp}_H\n")
            headerOutput.write("#include \"ExternArray.hpp\"\n\nnamespace MathLib {\n")
            for path in inputs:
                name : str = ConvertName(path, True)
                sourceOutput.write(f"\t// {path}\n")
                headerOutput.write(f"\t// {path}\n")
                sourceOutput.write("\tuint8_t " + name + "Buffer[] = {")
                i : int = 0
                with open(path, "rb") as input:
                    while True:
                        byte : bytes = input.read(1)
                        if not byte: break
                        sourceOutput.write(f"{" " if i else "\n\t\t"}0x{byte.hex()},")
                        i = (i + 1) % 16
                sourceOutput.write("\n\t};\n")
                sourceOutput.write(f"\tExternArray<uint8_t> {name} = ExternArray<uint8_t>({name}Buffer, sizeof({name}Buffer));\n")
                headerOutput.write(f"\textern ExternArray<uint8_t> {name};\n")
            headerOutput.write("}\n\n")
            headerOutput.write("#endif")
        sourceOutput.write("}")