#!/usr/bin/env python3

from os.path import dirname, isfile, splitext, exists
from subprocess import Popen, PIPE
from typing import Callable
from sys import argv, exit
from copy import deepcopy
from pathlib import Path
from os import listdir

buildDir : str = "bin"
srcDir : str = "src"
dataDir : str = "Data"
debug : bool = False
force : bool = False

def AssertNotNone(value : str | None) -> str:
    if value is not None: return value
    raise ValueError
def Sort(value : list[str]) -> list[str]:
    value.sort()
    return value
class Tool:
    program : str
    startFlags : list[str]
    endFlags : list[str]
    dependencies : list[str]

    def __init__(self : 'Tool', program : str, startFlags : list[str], endFlags : list[str], dependencies : list[str]) -> None:
        self.program = program
        self.startFlags = startFlags
        self.endFlags = endFlags
        self.dependencies = dependencies
    def __str__(self : 'Tool') -> str:
        return f"{self.program} {self.startFlags} {self.endFlags} {self.dependencies}"
    def __repr__(self : 'Tool') -> str:
        return self.__str__()
    def setProgram(self : 'Tool', program : str) -> 'Tool': return Tool(program, self.startFlags, self.endFlags, self.dependencies)
    def add(self : 'Tool', other : 'Tool') -> 'Tool': return Tool(other.program, self.startFlags + other.startFlags, self.endFlags + other.endFlags, self.dependencies + other.dependencies)
    def addFlags(self : 'Tool', startFlags : list[str], endFlags : list[str], dependencies : list[str]) -> 'Tool': return Tool(self.program, self.startFlags + startFlags, self.endFlags + endFlags, self.dependencies + dependencies)
    def generateCommand(self : 'Tool', output : str, flags : list[str]) -> str:
        ret : list[str] = []
        for x in [self.program] + self.startFlags + flags + self.endFlags:
            if not "@" in x or output: ret.append(x.replace("@", output))
        return " ".join(ret)
    def run(self : 'Tool', output : str, flags : list[str]) -> bool:
        cmd : str = self.generateCommand(output, flags)
        if debug: print(cmd)
        return Popen(cmd.split(" ")).wait() == 0
    def runAndGetStdout(self : 'Tool', output : str, flags : list[str]) -> str:
        cmd : str = self.generateCommand(output, flags)
        if debug: print(cmd)
        return Popen(cmd.split(" "), stdout = PIPE).communicate()[0].decode()
class Linker:
    tool : Tool
    runtimeEnvironments : dict[str, Tool]

    def __init__(self : 'Linker', tool : Tool, runtimeEnvironments : dict[str, Tool]) -> None:
        self.tool = tool
        self.runtimeEnvironments = runtimeEnvironments
    def __str__(self : 'Linker') -> str:
        return f"{self.tool} {self.runtimeEnvironments}"
    def __repr__(self : 'Linker') -> str:
        return self.__str__()
    def setProgram(self : 'Linker', program : str) -> 'Linker': return Linker(self.tool.setProgram(program), self.runtimeEnvironments)
    def add(self : 'Linker', other : 'Linker') -> 'Linker':
        ret : 'Linker' = deepcopy(self)
        ret.tool = ret.tool.add(other.tool)
        for env in other.runtimeEnvironments:
            if env not in ret.runtimeEnvironments: ret.runtimeEnvironments[env] = other.runtimeEnvironments[env]
            else: ret.runtimeEnvironments[env] = ret.runtimeEnvironments[env].add(other.runtimeEnvironments[env])
        return ret
    def addFlags(self : 'Linker', startFlags : list[str], endFlags : list[str], dependencies : list[str]) -> 'Linker': return Linker(self.tool.addFlags(startFlags, endFlags, dependencies), self.runtimeEnvironments)
    def run(self : 'Linker', output : str, flags : list[str]) -> bool:
        return self.tool.run(output, flags)
class Environment:
    tools : dict[str, Tool]
    linkers : dict[str, Linker]

    def __init__(self : 'Environment', tools : dict[str, Tool], linkers : dict[str, Linker]) -> None:
        self.tools = tools
        self.linkers = linkers
    def __str__(self : 'Environment') -> str:
        return f"{self.tools} {self.linkers}"
    def __repr__(self : 'Environment') -> str:
        return self.__str__()
    def setPrograms(self : 'Environment', programs : dict[str, str]) -> 'Environment':
        ret : 'Environment' = deepcopy(self)
        for tool in programs:
            if tool in ret.tools: ret.tools[tool] = ret.tools[tool].setProgram(programs[tool])
            if tool in ret.linkers: ret.linkers[tool] = ret.linkers[tool].setProgram(programs[tool])
        return ret
    def add(self : 'Environment', other : 'Environment') -> 'Environment':
        ret : 'Environment' = deepcopy(self)
        for otherTool in other.tools:
            if otherTool not in ret.tools: ret.tools[otherTool] = other.tools[otherTool]
            else: ret.tools[otherTool] = ret.tools[otherTool].add(other.tools[otherTool])
        for otherLinker in other.linkers:
            if otherLinker not in ret.linkers: ret.linkers[otherLinker] = other.linkers[otherLinker]
            else: ret.linkers[otherLinker] = ret.linkers[otherLinker].add(other.linkers[otherLinker])
        return ret
    def addFlags(self : 'Environment', flags : dict[str, list[list[str]]]) -> 'Environment':
        ret : 'Environment' = deepcopy(self)
        for name in flags:
            if name in ret.tools: ret.tools[name] = ret.tools[name].addFlags(flags[name][0], flags[name][1], flags[name][2])
            if name in ret.linkers: ret.linkers[name] = ret.linkers[name].addFlags(flags[name][0], flags[name][1], flags[name][2])
        return ret
class EnvironmentIdentifier:
    env : str | None
    linker : str | None
    runtimeEnvironment : str | None

    def __init__(self : 'EnvironmentIdentifier', env : str | None, linker : str | None, runtimeEnvironment : str | None) -> None:
        self.env = env
        self.linker = linker
        self.runtimeEnvironment = runtimeEnvironment
    def __str__(self : 'EnvironmentIdentifier') -> str:
        return f"{self.env}/{self.linker}/{self.runtimeEnvironment}"
    def __repr__(self : 'EnvironmentIdentifier') -> str:
        return self.__str__()
    def fill(self : 'EnvironmentIdentifier', other : 'EnvironmentIdentifier') -> 'EnvironmentIdentifier':
        ret : 'EnvironmentIdentifier' = deepcopy(self)
        if not ret.env: ret.env = other.env
        if not ret.linker: ret.linker = other.linker
        if not ret.runtimeEnvironment: ret.runtimeEnvironment = other.runtimeEnvironment
        return ret
    def combine(self : 'EnvironmentIdentifier', other : 'EnvironmentIdentifier') -> 'EnvironmentIdentifier':
        ret : 'EnvironmentIdentifier' = deepcopy(self)
        if ret.env is None: ret.env = other.env
        if ret.linker is None: ret.linker = other.linker
        if ret.runtimeEnvironment is None: ret.runtimeEnvironment = other.runtimeEnvironment
        return ret
    def isValid(self : 'EnvironmentIdentifier') -> bool:
        return self.env is not None and self.linker is not None and self.runtimeEnvironment is not None
    def toPath(self : 'EnvironmentIdentifier') -> str:
        ret : str = ""
        if AssertNotNone(self.env): ret += AssertNotNone(self.env) + "/"
        if AssertNotNone(self.linker): ret += AssertNotNone(self.linker) + "/"
        if AssertNotNone(self.runtimeEnvironment): ret += AssertNotNone(self.runtimeEnvironment) + "/"
        return ret
    def getEnv(self : 'EnvironmentIdentifier') -> Environment:
        return envs[AssertNotNone(self.env)]
    def getLinker(self : 'EnvironmentIdentifier') -> Linker:
        return self.getEnv().linkers[AssertNotNone(self.linker)]
    def getRuntimeEnvironment(self : 'EnvironmentIdentifier') -> Tool:
        return self.getLinker().runtimeEnvironments[AssertNotNone(self.runtimeEnvironment)]
    def getDependencies(self : 'EnvironmentIdentifier', tools : list[str]) -> list[str]:
        dependencies : list[str] = []
        for tool in tools:
            if tool == self.linker: dependencies += self.getLinker().tool.dependencies
            elif tool == self.runtimeEnvironment: dependencies += self.getRuntimeEnvironment().dependencies
            else: dependencies += self.getEnv().tools[tool].dependencies
        return dependencies
class TargetEnvironment:
    env : list[str] | None
    linker : list[str] | None
    runtimeEnvironment : list[str] | None

    def __init__(self : 'TargetEnvironment', env : list[str] | None, linker : list[str] | None, runtimeEnvironment : list[str] | None) -> None:
        self.env = env
        self.linker = linker
        self.runtimeEnvironment = runtimeEnvironment
    def __str__(self : 'TargetEnvironment') -> str:
        return f"{self.env}/{self.linker}/{self.runtimeEnvironment}"
    def __repr__(self : 'TargetEnvironment') -> str:
        return self.__str__()
    def restrict(self : 'TargetEnvironment', other : EnvironmentIdentifier) -> EnvironmentIdentifier:
        ret : EnvironmentIdentifier = EnvironmentIdentifier(None, None, None)
        if self.env is not None:
            if len(self.env) == 0: ret.env = ""
            elif len(self.env) == 1: ret.env = self.env[0]
            elif other.env in self.env: ret.env = other.env
        else: ret.env = other.env
        if self.linker is not None:
            if len(self.linker) == 0: ret.linker = ""
            elif len(self.linker) == 1: ret.linker = self.linker[0]
            elif other.linker in self.linker: ret.linker = other.linker
        else: ret.linker = other.linker
        if self.runtimeEnvironment is not None:
            if len(self.runtimeEnvironment) == 0: ret.runtimeEnvironment = ""
            elif len(self.runtimeEnvironment) == 1: ret.runtimeEnvironment = self.runtimeEnvironment[0]
            elif other.runtimeEnvironment in self.runtimeEnvironment: ret.runtimeEnvironment = other.runtimeEnvironment
        else: ret.runtimeEnvironment = other.runtimeEnvironment
        return ret
class Target:
    makeInternal : Callable[[list[str], list[str], str, EnvironmentIdentifier], bool]
    getDependenciesInternal : Callable[[], list[str]]
    getToolsInternal : Callable[[EnvironmentIdentifier], list[str]]
    environment : TargetEnvironment
    inputs : list[str]
    givesOutput : bool

    def __init__(self : 'Target', makeInternal : Callable[[list[str], list[str], str, EnvironmentIdentifier], bool], getDependenciesInternal : Callable[[], list[str]], getToolsInternal : Callable[[EnvironmentIdentifier], list[str]], environment : TargetEnvironment, inputs : list[str], givesOutput : bool) -> None:
        self.makeInternal = makeInternal
        self.getDependenciesInternal = getDependenciesInternal
        self.getToolsInternal = getToolsInternal
        self.environment = environment
        self.inputs = inputs
        self.givesOutput = givesOutput
    def make(self : 'Target', inputs : list[str], dependencies : list[str], output : str, environment : EnvironmentIdentifier): return self.makeInternal(inputs, dependencies, output, environment)
    def getTools(self : 'Target', env : EnvironmentIdentifier): return self.getToolsInternal(env)
    def getDependencies(self : 'Target', sub : str, env : EnvironmentIdentifier):
        return [MakeTarget(dependency.replace("%", sub), env) for dependency in self.getDependenciesInternal()]
    def getToolsDependencies(self : 'Target', env : EnvironmentIdentifier):
        dependencies : list[str] = env.getDependencies(self.getTools(env))
        return [MakeTarget(dependency, env) for dependency in dependencies]

baseGCCFlags : list[str] = ["-g", "-O0", "-DDebug", "-Wall", "-Wextra", "-Werror", "-I .", f"-I {srcDir}/Lib", f"-I {srcDir}/Platform", f"-I {buildDir}", "-Wno-packed-bitfield-compat", "-Wno-unused-function"]
baseNASMFlags : list[str] = ["-g", "-O0", "-DDebug", "-Werror", f"-I {srcDir}", f"-I {srcDir}/Lib", f"-I {srcDir}/Platform"]
osGCCFlags : list[str] = [f"-I {srcDir}/OS/Shared", "-DFreestanding", "-ffreestanding", "-fstack-protector-all", "-mcmodel=large", "-mno-red-zone", "-fno-exceptions", "-fno-rtti", "-fno-omit-frame-pointer"]

run : Tool = Tool("./@", [], [], [])
valgrind : Tool = Tool("valgrind", ["-s", "--leak-check=full", "--show-leak-kinds=all", "./@"], [], [])
python : Tool = Tool("python3", [], [], [])
copyFile : Tool = Tool("cp", ["-rf"], ["@"], [])
makeDirectory : Tool = Tool("mkdir", ["-p", "@"], [], [])
removeDirectory : Tool = Tool("rm", ["-rf"], ["@"], [])
createDisk : Tool = Tool("dd", ["if=/dev/zero", "of=@", "bs=512"], [], [])
makeFAT : Tool = Tool("mkfs.vfat", [], ["@"], [])
copyToFAT : Tool = Tool("mcopy", ["-s -i @"], ["::"], [])
makeTAR : Tool = Tool("tar", ["-cf @"], [], [])
appendTAR : Tool = Tool("tar", ["-rf @"], [], [])
unwrapTAR : Tool = Tool("tar", ["-xf @"], [], [])
makeEXT2 : Tool = Tool("genext2fs", [], ["@"], [])
makeGRUB2 : Tool = Tool("grub-mkrescue", ["-quiet", "-l", "-J", "-R", "-o @"], [], [])

baseExec : Tool = Tool("", baseGCCFlags, [f"-L {buildDir}", "-o @"], [])
baseEnvTools : Environment = Environment({
    "cc" : Tool("", baseGCCFlags + ["-x c", "-c"], ["-o @"], []),
    "cxx" : Tool("", baseGCCFlags + ["-x c++", "-c"], ["-o @"], []),
    "as" : Tool("", baseNASMFlags + ["-f elf64"], ["-o @"], []),
}, {})
baseEnv : Environment = baseEnvTools.add(Environment({}, {
    "static" : Linker(Tool("", ["-rcs", "@"], [], []), {}),
    "dynamic" : Linker(Tool("", baseGCCFlags + ["-shared"], ["-Wl,-soname,@", "-o @"], []), {}),
}))
envs : dict[str, Environment] = {
    "host" : baseEnv.add(Environment({
        "cc" : Tool("g++", ["-fPIC"], [], []),
        "cxx" : Tool("g++", ["-fPIC"], [], []),
        "as" : Tool("nasm", [], [], []),
    }, {
        "static" : Linker(Tool("ar", [], [], []), {
            "console" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/static", "-Wl,--whole-archive -l:ConsoleInit.lib -Wl,--no-whole-archive"], ["-l:Math.lib"], ["Math.lib", "ConsoleInit.lib"])),
            "sdl2" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/static", "-Wl,--whole-archive -l:SDL2Init.lib -Wl,--no-whole-archive"], ["-l:libSDL2.so", "-l:Math.lib"], ["Math.lib", "SDL2Init.lib"])),
            "openGL" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/static", "-Wl,--whole-archive -l:ConsoleInit.lib -Wl,--no-whole-archive"], ["-l:libGL.so", "-l:libglfw.so", "-l:libglbinding.so", "-l:Math.lib"], ["Math.lib", "ConsoleInit.lib"])),
            "curl" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/static", "-Wl,--whole-archive -l:ConsoleInit.lib -Wl,--no-whole-archive"], ["-l:libcurl.so", "-l:Math.lib"], ["Math.lib", "ConsoleInit.lib"])),
        }),
        "dynamic" : Linker(Tool("g++", [], [], []), {
            "console" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/dynamic", "-l:ConsoleInit.lib"], ["-l:Math.lib"], ["Math.lib", "ConsoleInit.lib"])),
            "sdl2" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/dynamic", "-l:SDL2Init.lib"], ["-l:libSDL2.so", "-l:Math.lib"], ["Math.lib", "SDL2Init.lib"])),
            "openGL" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/dynamic", "-l:ConsoleInit.lib"], ["-l:libGL.so", "-l:libglfw.so", "-l:libglbinding.so", "-l:Math.lib"], ["Math.lib", "ConsoleInit.lib"])),
            "curl" : baseExec.add(Tool("g++", [f"-L {buildDir}/host/dynamic", "-l:ConsoleInit.lib"], ["-l:libcurl.so", "-l:Math.lib"], ["Math.lib", "ConsoleInit.lib"])),
        }),
    })),
    "os" : baseEnvTools.add(Environment({
        "cc" : Tool("x86_64-elf-g++", osGCCFlags, [], []),
        "cxx" : Tool("x86_64-elf-g++", osGCCFlags, [], []),
        "as" : Tool("nasm", [], [], []),
        "qemu" : Tool("qemu-system-x86_64", [
            "-usb", "-smp 1", "-M q35", "-m 4096M", "-rtc base=localtime", "-boot d",
            "-device rtl8139,netdev=net0", "-netdev user,id=net0,hostfwd=tcp::8080-:8080",
            f"-serial file:{buildDir}/os/OS.log", f"-cdrom {buildDir}/os/OS.img",
            f"-drive file={buildDir}/os/FAT.img,format=raw,media=disk",
            f"-drive file={buildDir}/os/EXT.img,format=raw,media=disk",
        ], [], ["OS.img", "FAT.img", "EXT.img"]),
        "gdb" : Tool("gdb", ["-x @"], [], ["OS.img", "FAT.img", "EXT.img"]),
    }, {
        "static" : Linker(Tool("x86_64-elf-ar", ["-rcs", "@"], [], []), {}),
        "executable" : Linker(Tool("false", [], [], []), {
            "elf" : baseExec.add(Tool("x86_64-elf-g++", ["-Bsymbolic", "-nostdlib"], ["-l gcc", f"-Xlinker -Map={buildDir}/os/Kernel.map"], [])),
        }),
    })),
    "binaryAssembler" : Environment({
        "as" : Tool("nasm", baseNASMFlags + ["-f bin"], ["-o @"], []),
    }, {})
}

def CompileSourceFiles(directories : list[str], validExtensions : list[str]) -> list[str]:
    ret : list[str] = []
    directories.sort()
    for directory in directories:
        ret += [source.removeprefix(f"{srcDir}/") + ".o" for source in ListDirectory(f"{srcDir}/{directory}") if splitext(source)[1] in validExtensions]
    ret.sort()
    return ret
def Link(env : TargetEnvironment, flags : list[str], directories : list[str], validExtensions : list[str]) -> Target:
    return Target(lambda inputs, dependencies, output, env : env.getLinker().run(output, dependencies + flags), lambda : CompileSourceFiles(directories, validExtensions), lambda env : [AssertNotNone(env.linker)], env, [], True)

def MatchesTarget(output : str, target : str) -> str | None:
    split : list[str] = target.replace("*", "%").split("%")
    if len(split) > 2: return None
    if len(split) == 1: return "" if target == output else None
    if not (output.startswith(split[0]) and output.endswith(split[1])): return None
    return output.removeprefix(split[0]).removesuffix(split[1])
def ListDirectory(path : str) -> list[str]:
    path = path.removesuffix("/")
    content : list[str] = listdir(path)
    ret : list[str] = []
    for tmp in content:
        tmp = path + "/" + tmp
        if isfile(tmp): ret.append(tmp)
        else: ret += ListDirectory(tmp)
    return ret
def ConvertPath(path : str) -> list[str]:
    split : list[str] = path.split("*")
    if len(split) > 2: return []
    if len(split) == 1: return split
    return [file for file in ListDirectory(split[0]) if file.endswith(split[1])]
def MakeTarget(output : str, env : EnvironmentIdentifier) -> str:
    target : str = ""
    sub : str = ""
    for tmpTarget in targets:
        tmp : str | None = MatchesTarget(output, tmpTarget)
        if tmp == None: continue
        target = tmpTarget
        sub = tmp
        break
    if target == "": return ""
    forwardEnv : EnvironmentIdentifier = targets[target].environment.restrict(env).fill(env)
    if not forwardEnv.isValid(): return ""
    env = targets[target].environment.restrict(env)
    if not env.isValid(): return ""
    outputPath : str = buildDir + '/' + env.toPath() + output
    inputs : list[str] = []
    for input in targets[target].inputs: inputs += ConvertPath(srcDir + "/" + input.replace("%", sub))
    dependencies : list[str] = targets[target].getDependencies(sub, forwardEnv)
    if "" in dependencies: return ""
    toolsDependencies : list[str] = targets[target].getToolsDependencies(forwardEnv)
    if "" in toolsDependencies: return ""
    if targets[target].givesOutput:
        Path(dirname(outputPath)).mkdir(parents = True, exist_ok = True)
        if not force and exists(outputPath) and max([Path(file).stat().st_mtime for file in inputs + dependencies + toolsDependencies]) <= Path(outputPath).stat().st_mtime:
            return outputPath
        print(f"==> [{env}] {output}")
    return outputPath if targets[target].make(inputs, dependencies, outputPath, env) else ""

targets : dict[str, Target] = {
    "clean": Target(lambda inputs, dependencies, output, env : removeDirectory.run(f"{buildDir}/", []), lambda : [], lambda env : [], TargetEnvironment([], [], []), [], False),

    "%.cpp.o": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["cxx"].run(output, [inputs[0]]), lambda : [], lambda env : ["cxx"], TargetEnvironment(None, [], []), ["%.cpp"], True),
    "%.asm.o": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["as"].run(output, [inputs[0]]), lambda : [], lambda env : ["as"], TargetEnvironment(None, [], []), ["%.asm"], True),
    "%.s.o": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["as"].run(output, [inputs[0]]), lambda : [], lambda env : ["as"], TargetEnvironment(None, [], []), ["%.s"], True),

    "Math.lib": Target(lambda inputs, dependencies, output, env : env.getLinker().run(output, dependencies), lambda : ["LibStub.o"], lambda env : [AssertNotNone(env.linker)], TargetEnvironment(None, None, []), [], True),
    "LibStub.o": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["cxx"].run(output, dependencies), lambda : ["MathLib.hpp"], lambda env : ["cxx"], TargetEnvironment(None, [], []), ["Lib/*.hpp"], True),
    "MathLib.hpp": Target(lambda inputs, dependencies, output, env : python.run("", inputs + dependencies + [output]), lambda : ["Fonts.cpp"], lambda env : [], TargetEnvironment([], [], []), ["../Scripts/MakeIncludes.py", "Lib/*.cpp"], True),
    "Fonts.cpp": Target(lambda inputs, dependencies, output, env : python.run("", [inputs[0], output, dirname(output) + "/Fonts.hpp"] + inputs[1:]), lambda : [], lambda env : [], TargetEnvironment([], [], []), ["../Scripts/PSFToCXX.py", "*.psf"], True),
    "Fonts.hpp": Target(lambda inputs, dependencies, output, env : True, lambda : ["Fonts.cpp"], lambda env : [], TargetEnvironment([], [], []), [], True),

    "ConsoleInit.lib": Link(TargetEnvironment(["host"], None, []), [], ["Platform/Console"], [".cpp"]),
    "SDL2Init.lib": Link(TargetEnvironment(["host"], None, []), [], ["Platform/SDL2"], [".cpp"]),

    "Main.aml": Target(lambda inputs, dependencies, output, env : python.run("", inputs + [output, "-oa"]), lambda : [], lambda env : [], TargetEnvironment([], [], []), ["../Scripts/CompileASL.py", f"../{dataDir}/ACPI/Main.asl"], True),
    "X86.bin": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["as"].run(output, inputs), lambda : [], lambda env : ["as"], TargetEnvironment(["binaryAssembler"], [], []), [f"../{dataDir}/X86/Program.asm"], True),

    "Kernel.map": Target(lambda inputs, dependencies, output, env : True, lambda : ["Kernel.elf"], lambda env : [], TargetEnvironment(["os"], [], []), [], True),
    "Kernel.elf": Target(lambda inputs, dependencies, output, env : env.getRuntimeEnvironment().run(output, [f"-T {inputs[0]}", dependencies[1], env.getEnv().tools["cxx"].runAndGetStdout("", ["-print-file-name=crtbegin.o"]).removesuffix("\n")] + dependencies[2:-1] + [env.getEnv().tools["cxx"].runAndGetStdout("", ["-print-file-name=crtend.o"]).removesuffix("\n"), dependencies[-1]]), lambda : ["MathLib.hpp", "OS/Shared/crti.s.o"] + Sort(CompileSourceFiles(["OS/Kernel", "OS/Shared"], [".cpp", ".asm"]) + ["LibStub.o"]) + ["OS/Shared/crtn.s.o"], lambda env : [AssertNotNone(env.runtimeEnvironment), "cxx"], TargetEnvironment(["os"], ["executable"], ["elf"]), ["OS/Kernel/Linker.ld"], True),
    "libModule.a": Link(TargetEnvironment(["os"], ["static"], []), [], ["OS/Module"], [".cpp", ".asm"]),

    "FAT.img": Target(lambda inputs, dependencies, output, env : createDisk.run(output, ["count=93750"]) and makeFAT.run(output, ["-F 16"]) and copyToFAT.run(output, inputs), lambda : [], lambda env : [], TargetEnvironment(["os"], [], []), [f"../{dataDir}/OS/FAT/*"], True),
    "EXT.img": Target(lambda inputs, dependencies, output, env : makeDirectory.run(f"{buildDir}/tmp/lib", []) and python.run("", inputs + [f"{buildDir}/tmp", "5"]) and python.run("", [inputs[0]] + dependencies + [f"{buildDir}/tmp/lib", "3"]) and makeEXT2.run(output, ["-b 100000", f"-d {buildDir}/tmp"]) and removeDirectory.run(f"{buildDir}/tmp", []), lambda : ["libModule.a"], lambda env : [], TargetEnvironment(["os"], [], []), ["../Scripts/CopyFiles.py", f"../{dataDir}/OS/EXT/*"], True),
    "OS.img": Target(lambda inputs, dependencies, output, env : makeDirectory.run(f"{buildDir}/tmp/boot", []) and python.run("", inputs + [f"{buildDir}/tmp", "5"]) and python.run("", [inputs[0]] + dependencies + [f"{buildDir}/tmp/boot", "4"]) and makeGRUB2.run(output, [f"{buildDir}/tmp"]) and removeDirectory.run(f"{buildDir}/tmp", []), lambda : ["Kernel.elf"], lambda env : [], TargetEnvironment(["os"], [], []), ["../Scripts/CopyFiles.py", f"../{dataDir}/OS/ISO9660/*"], True),
    "OS.gdb": Target(lambda inputs, dependencies, output, env : python.run("", inputs + [output, f"{buildDir}/os/executable/elf/Kernel.elf", "Entry", env.getEnv().tools["qemu"].generateCommand("", ["-no-reboot", "-no-shutdown"])]), lambda : [], lambda env : ["qemu"], TargetEnvironment(["os"], [], []), ["../Scripts/MakeGDB.py"], True),

    "runOS": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["qemu"].run("", ["-debugcon stdio"]), lambda : [], lambda env : ["qemu"], TargetEnvironment(["os"], [], []), [], False),
    "debugOS": Target(lambda inputs, dependencies, output, env : env.getEnv().tools["gdb"].run(dependencies[0], []), lambda : ["OS.gdb"], lambda env : ["gdb"], TargetEnvironment(["os"], [], []), [], False),
}
gfxRuntime : list[str] = ["sdl2"]
executables : list[str] = ["Main.aml", "X86.bin", "OS.gdb"]
def AddExecutableWithRunDependencies(name : str, env : TargetEnvironment, runFlags : Callable[[list[str]], list[str]], flags: list[str], directories: list[str], validExtensions : list[str], runDeps : list[str]) -> None:
    executables.append(f"{name}.out")
    targets[f"{name}.lib"] = Link(TargetEnvironment(env.env, env.linker, []), flags, directories, validExtensions)
    targets[f"{name}.out"] = Target(lambda inputs, dependencies, output, env : env.getRuntimeEnvironment().run(output, [f"-l:{name}.lib"]), lambda : [f"{name}.lib"], lambda env : [AssertNotNone(env.runtimeEnvironment)], env, [], True)
    targets[f"run{name}"] = Target(lambda inputs, dependencies, output, env : run.run(dependencies[0], runFlags(dependencies)), lambda : [f"{name}.out"] + runDeps, lambda env : [], TargetEnvironment([], [], []), [], False)
    targets[f"debug{name}"] = Target(lambda inputs, dependencies, output, env : valgrind.run(dependencies[0], runFlags(dependencies)), lambda : [f"{name}.out"] + runDeps, lambda env : [], TargetEnvironment([], [], []), [], False)
def AddExecutable(name : str, env : TargetEnvironment, runFlags : Callable[[str], list[str]], flags: list[str], directories: list[str], validExtensions : list[str]) -> None:
    AddExecutableWithRunDependencies(name, env, lambda list : runFlags(list[0]), flags, directories, validExtensions, [])
AddExecutable("4D", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [f"-program {dataDir}/4D/Tesseract.txt"], [], ["4D"], [".cpp"])
AddExecutable("AES", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/AES"], [], ["AES"], [".cpp"])
AddExecutable("AI", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["AI"], [".cpp"])
AddExecutable("ASL", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/ACPI/OS.asl"], [], ["ASL"], [".cpp"])
AddExecutable("Bencode", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/Bencode/ArchLinux.torrent"], [], ["Bencode"], [".cpp"])
AddExecutable("BNF", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["BNF"], [".cpp"])
AddExecutable("IsomerGenerator", TargetEnvironment(["host"], None, ["console"]), lambda program : ["6"], [], ["Chemistry/IsomerGenerator"], [".cpp"])
AddExecutable("ReactionBalancer", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/Chemistry/All.txt"], [], ["Chemistry/ReactionBalancer"], [".cpp"])
AddExecutable("Circuit", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Circuit"], [".cpp"])
AddExecutable("Compiler", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Compiler"], [".cpp"])
AddExecutable("Electricity", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Electricity"], [".cpp"])
AddExecutable("ELFParser", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"-program {program}"], [], ["ELFParser"], [".cpp"])
AddExecutable("6502", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Emulator/6502"], [".cpp"])
AddExecutable("FileSystemConverter", TargetEnvironment(["host"], None, ["console"]), lambda program : ["GNUTAR", f"{buildDir}/FS.tar", "Makefile Makefile", "TODO.md TODO.md"], [], ["FileSystemConverter"], [".cpp"])
AddExecutable("BlockFiller", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Games/BlockFiller"], [".cpp"])
AddExecutable("Cards", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Games/Cards"], [".cpp"])
AddExecutable("Chomp", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Games/Chomp"], [".cpp"])
AddExecutable("GameOfLife", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Games/GameOfLife"], [".cpp"])
AddExecutable("GameTheory", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Games/GameTheory"], [".cpp"])
AddExecutable("SlotMachine", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Games/SlotMachine"], [".cpp"])
AddExecutable("Git", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Git"], [".cpp"])
AddExecutable("HeaderTree", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{srcDir}/Lib/Typedefs.hpp"], [], ["HeaderTree"], [".cpp"])
AddExecutable("HTTP", TargetEnvironment(["host"], None, ["console"]), lambda program : ["8080"], [], ["HTTP"], [".cpp"])
AddExecutable("ImageConverter", TargetEnvironment(["host"], None, gfxRuntime), lambda program : ["-inputType TGA", f"-input {dataDir}/RPG/Emoji.tga", "-outputType P6", f"-output {buildDir}/ImageConverter.ppm"], [], ["ImageConverter"], [".cpp"])
AddExecutable("LambdaCalculus", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/LambdaCalculus/Main.txt"], [], ["LambdaCalculus"], [".cpp"])
AddExecutable("MachineLearning", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{buildDir}/ML.bin", "20000"], [], ["MachineLearning"], [".cpp"])
AddExecutable("Channel", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Math/Channel"], [".cpp"])
AddExecutable("ChaosTheory", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Math/ChaosTheory"], [".cpp"])
AddExecutable("ComplexAnimation", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [f"{dataDir}/Math/Trigonometry/Sine.txt", f"{buildDir}/ComplexAnimationFrames", f"{buildDir}/Animation.mp4"], [], ["Math/ComplexAnimation"], [".cpp"])
AddExecutable("ConstantCalculator", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Math/ConstantCalculator"], [".cpp"])
AddExecutable("Graph", TargetEnvironment(["host"], None, gfxRuntime), lambda program : ListDirectory(f"{dataDir}/Math"), [], ["Math/Graph"], [".cpp"])
AddExecutable("Polynomial", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/Math/Polynomial.txt"], [], ["Math/Polynomial"], [".cpp"])
AddExecutable("PolynomialApproximation", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Math/PolynomialApproximation"], [".cpp"])
AddExecutable("ProjectEuler", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Math/ProjectEuler"], [".cpp"])
AddExecutable("Prover", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/Prover/Main.txt"], [], ["Math/Prover"], [".cpp"])
AddExecutable("Quiz", TargetEnvironment(["host"], None, ["console"]), lambda program : ListDirectory(f"{dataDir}/Math"), [], ["Math/Quiz"], [".cpp"])
AddExecutable("Sine", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Math/Sine"], [".cpp"])
AddExecutable("Transform", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Math/Transform"], [".cpp"])
AddExecutable("OpenGL", TargetEnvironment(["host"], None, ["openGL"]), lambda program : [], [], ["OpenGL"], [".cpp"])
AddExecutable("Paint", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Paint"], [".cpp"])
AddExecutable("Gravity", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Physics/Gravity"], [".cpp"])
AddExecutable("Ray", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Physics/Ray"], [".cpp"])
AddExecutable("Rope", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Physics/Rope"], [".cpp"])
AddExecutable("Queue", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Queue"], [".cpp"])
AddExecutable("SequentCalculus", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/SequentCalculus/ModusPonens.txt"], [], ["SequentCalculus"], [".cpp"])
AddExecutable("Sort", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["Sort"], [".cpp"])
AddExecutable("Sudoku", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/Sudoku/2.csv"], [], ["Sudoku"], [".cpp"])
AddExecutable("TerrainGenerator", TargetEnvironment(["host"], None, gfxRuntime), lambda program : [], [], ["TerrainGenerator"], [".cpp"])
AddExecutable("Tests", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["Tests"], [".cpp"])
AddExecutable("TextGenerator", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["TextGenerator"], [".cpp"])
AddExecutable("TheoremProver", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["TheoremProver"], [".cpp"])
AddExecutable("TypeTheory", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{dataDir}/TypeTheory/Main.txt"], [], ["TypeTheory"], [".cpp"])
AddExecutable("VideoPlayer", TargetEnvironment(["host"], None, gfxRuntime), lambda program : ["-width 800", "-height 800", "-speed 0.1", "-skipDuration 1", "-multX 4", "-multY 4", f"-path {dataDir}/VideoPlayer/Video.aseprite"], [], ["VideoPlayer"], [".cpp"])
AddExecutable("WebScraper", TargetEnvironment(["host"], None, ["curl"]), lambda program : [f"{dataDir}/WebScraper/Wikipedia.json", f"{buildDir}/WebScraperOutput"], [], ["WebScraper"], [".cpp"])
AddExecutable("WebsiteReader", TargetEnvironment(["host"], None, ["console"]), lambda program : [], [], ["WebsiteReader"], [".cpp"])
AddExecutableWithRunDependencies("AML", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{program[1]}"], [], ["Emulator/AML"], [".cpp"], ["Main.aml"])
AddExecutableWithRunDependencies("X86", TargetEnvironment(["host"], None, ["console"]), lambda program : [f"{program[1]}"], [], ["Emulator/X86"], [".cpp"], ["X86.bin"])

if __name__ == "__main__":
    if len(argv) < 2:
        print(f"Usage: {argv[0]} <target>")
        exit(1)
    defaultEnv : EnvironmentIdentifier = EnvironmentIdentifier("host", "dynamic", "sdl2")
    currentTargets : list[str] = executables if argv[1] == "all" else [argv[1]]
    failedTargets : list[str] = []
    for target in currentTargets:
        if not MakeTarget(target, defaultEnv):
            failedTargets.append(target)
    if len(failedTargets) == 1:
        print(f"Failed to run target {failedTargets[0]}")
        exit(1)
    elif failedTargets:
        print(f"Failed to run targets {failedTargets}")
        exit(1)