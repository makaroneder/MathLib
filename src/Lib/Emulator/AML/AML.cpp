#include "AML.hpp"
#include "../../Memory.hpp"
#include "AMLFieldFlags.hpp"

namespace MathLib {
    AML::AML(const Sequence<uint8_t>& memory) : Emulator(memory), root("Root"_M) {
        EmptyBenchmark
    }
    AML::AML(const DSDT* dsdt) : Emulator(Array<uint8_t>(dsdt->aml, dsdt->length - sizeof(ACPITable))), root("Root"_M) {
        StartBenchmark
        if (!dsdt->IsValid()) Panic("Invalid DSDT provided");
        EndBenchmark
    }
    void AML::Reset(void) {
        StartBenchmark
        pc = 0;
        root = AMLObject("Root"_M);
        EndBenchmark
    }
    AMLObject AML::GetRoot(void) const {
        return root;
    }
    Expected<AMLOpcode> AML::ParseOpcode(void) {
        const Expected<uint8_t> tmp = Fetch<uint8_t>();
        if (!tmp.HasValue()) return Expected<AMLOpcode>();
        switch (tmp.Get()) {
            case (uint8_t)AMLOpcode::Extension: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) return Expected<AMLOpcode>();
                return (AMLOpcode)(((uint16_t)AMLOpcode::Extension << 8) | tmp.Get());
            }
            default: return (AMLOpcode)tmp.Get();
        }
    }
    Expected<uint32_t> AML::ParsePackageLength(void) {
        const Expected<uint8_t> tmp = Fetch<uint8_t>();
        if (!tmp.HasValue()) return Expected<uint32_t>();
        const size_t packageSize = 1 + (tmp.Get() >> 6);
        if (packageSize == 1) return (tmp.Get() & 0b111111) - packageSize;
        else if (packageSize < 5) {
            uint8_t tmpArr[packageSize - 1];
            if (!ReadPositionedBuffer(tmpArr, SizeOfArray(tmpArr), pc)) return Expected<uint32_t>();
            pc += SizeOfArray(tmpArr);
            uint32_t ret = 0;
            MemoryCopy(tmpArr, &ret, SizeOfArray(tmpArr));
            return ((tmp.Get() & 0b1111) | ret << 4) - packageSize;
        }
        else return Expected<uint32_t>();
    }
    Expected<String> AML::ParseNameSegment(void) {
        Expected<char> tmp = Fetch<char>();
        if (!IsUpper(tmp.Get()) && tmp.Get() != '_') return Expected<String>();
        String ret = tmp.Get();
        for (uint8_t i = 0; i < 3; i++) {
            tmp = Fetch<char>();
            if (!tmp.HasValue()) return Expected<String>();
            if (!IsDigit(tmp.Get()) && !IsUpper(tmp.Get()) && tmp.Get() != '_') return Expected<String>();
            ret += tmp.Get();
        }
        return ret;
    }
    Expected<String> AML::ParseString(void) {
        String ret;
        Expected<char> tmp = Fetch<char>();
        if (!tmp.HasValue()) return Expected<String>();
        if (tmp.Get() == '\\') {
            ret += '\\';
            tmp = Fetch<char>();
            if (!tmp.HasValue()) return Expected<String>();
        }
        while (tmp.Get() == '^') {
            ret += '^';
            tmp = Fetch<char>();
            if (!tmp.HasValue()) return Expected<String>();
        }
        switch (tmp.Get()) {
            case (char)AMLOpcode::NullName: return ret;
            case (char)AMLOpcode::DualNamePrefix: {
                Expected<String> tmp = ParseNameSegment();
                if (!tmp.HasValue()) return Expected<String>();
                ret += tmp.Get();
                tmp = ParseNameSegment();
                return tmp.HasValue() ? ret + tmp.Get() : Expected<String>();
            }
            case (char)AMLOpcode::MultiNamePrefix: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) return Expected<String>();
                for (uint16_t i = 0; i < tmp.Get(); i++) {
                    const Expected<String> tmp = ParseNameSegment();
                    if (!tmp.HasValue()) return Expected<String>();
                    ret += tmp.Get();    
                }
                return ret;
            }
            default: {
                pc--;
                Expected<String> tmp = ParseNameSegment();
                return tmp.HasValue() ? ret + tmp.Get() : Expected<String>();
            }
        }
    }
    Array<uint64_t> AML::ParseComputetionalData(void) {
        const Expected<AMLOpcode> opcode = ParseOpcode();
        if (!opcode.HasValue()) return Array<uint64_t>();
        switch (opcode.Get()) {
            case AMLOpcode::Zero:
            case AMLOpcode::One:
            case AMLOpcode::Ones: return MakeArray<uint64_t>((uint8_t)opcode.Get());
            case AMLOpcode::Byte: {
                const Expected<uint8_t> ret = Fetch<uint8_t>();
                return ret.HasValue() ? MakeArray<uint64_t>(ret.Get()) : Array<uint64_t>();
            }
            case AMLOpcode::Word: {
                const Expected<uint16_t> ret = Fetch<uint16_t>();
                return ret.HasValue() ? MakeArray<uint64_t>(ret.Get()) : Array<uint64_t>();
            }
            case AMLOpcode::DWord: {
                const Expected<uint32_t> ret = Fetch<uint32_t>();
                return ret.HasValue() ? MakeArray<uint64_t>(ret.Get()) : Array<uint64_t>();
            }
            case AMLOpcode::QWord: {
                const Expected<uint64_t> ret = Fetch<uint64_t>();
                return ret.HasValue() ? MakeArray<uint64_t>(ret.Get()) : Array<uint64_t>();
            }
            case AMLOpcode::String: {
                Array<uint64_t> ret;
                while (true) {
                    const Expected<char> tmp = Fetch<char>();
                    if (!tmp.HasValue() || !ret.Add(tmp.Get())) return Array<uint64_t>();
                    if (!tmp.Get()) break;
                }
                return ret;
            }
            case AMLOpcode::Buffer: {
                const Expected<uint32_t> packageLength = ParsePackageLength();
                const size_t tmp = pc;
                const Array<uint64_t> size = ParseComputetionalData();
                if (size.GetSize() != 1) return Array<uint64_t>();
                if (packageLength.Get() - pc + tmp - size.At(0)) return Array<uint64_t>();
                Array<uint64_t> ret = Array<uint64_t>(size.At(0));
                for (uint64_t& x : ret) {
                    const Expected<uint8_t> tmp = Fetch<uint8_t>();
                    if (!tmp.HasValue()) return Array<uint64_t>();
                    x = tmp.Get();
                }
                return ret;
            }
            case AMLOpcode::Package: {
                const Expected<uint32_t> packageLength = ParsePackageLength();
                if (!packageLength.HasValue()) return Array<uint64_t>();
                const Expected<uint8_t> elements = Fetch<uint8_t>();
                if (!elements.HasValue()) return Array<uint64_t>();
                Array<uint64_t> ret;
                uint32_t length = packageLength.Get() - 1;
                for (uint16_t i = 0; i < elements.Get(); i++) {
                    const size_t savedPC = pc;
                    const Array<uint64_t> tmp = ParseComputetionalData();
                    const uint32_t size = pc - savedPC;
                    if (size > length) return Array<uint64_t>();
                    length -= size;
                    if (tmp.IsEmpty() || !ret.Add(tmp.GetSize())) return Array<uint64_t>();
                    for (const uint64_t& x : tmp)
                        if (!ret.Add(x)) return Array<uint64_t>();
                }
                return ret;
            }
            default: return Array<uint64_t>();
        }
    }
    bool AML::Run(AMLObject& object) {
        StartBenchmark
        if (pc >= GetSize()) ReturnFromBenchmark(true);
        const Expected<AMLOpcode> opcode = ParseOpcode();
        if (!opcode.HasValue()) ReturnFromBenchmark(false);
        switch (opcode.Get()) {
            case AMLOpcode::Scope:
            case AMLOpcode::Device: {
                const Expected<uint32_t> packageLength = ParsePackageLength();
                if (!packageLength.HasValue()) ReturnFromBenchmark(false);
                const size_t tmp = pc;
                const Expected<String> name = ParseString();
                if (!name.HasValue()) ReturnFromBenchmark(false);
                uint32_t length = packageLength.Get() - pc + tmp;
                AMLObject curr = AMLObject(name.Get());
                while (length) {
                    const size_t tmp = pc;
                    if (!Run(curr)) ReturnFromBenchmark(false);
                    const uint32_t size = pc - tmp;
                    if (size > length) ReturnFromBenchmark(false);
                    length -= size;
                }
                if (!object.AddChild(curr)) ReturnFromBenchmark(false);
                break;
            }
            case AMLOpcode::Name: {
                const Expected<String> name = ParseString();
                if (!name.HasValue()) ReturnFromBenchmark(false);
                const Array<uint64_t> data = ParseComputetionalData();
                if (data.IsEmpty() || !object.AddChild(AMLObject(name.Get(), data))) ReturnFromBenchmark(false);
                break;
            }
            case AMLOpcode::OperationRegion: {
                const Expected<String> name = ParseString();
                if (!name.HasValue()) ReturnFromBenchmark(false);
                const Expected<uint8_t> space = Fetch<uint8_t>();
                if (!space.HasValue()) ReturnFromBenchmark(false);
                const Array<uint64_t> offset = ParseComputetionalData();
                if (offset.GetSize() != 1) ReturnFromBenchmark(false);
                const Array<uint64_t> length = ParseComputetionalData();
                if (length.GetSize() != 1 || !object.AddChild(AMLObject(name.Get(), MakeArray<uint64_t>(space.Get(), offset.At(0), length.At(0)), AMLObject::Type::OperationRegion))) ReturnFromBenchmark(false);
                break;
            }
            case AMLOpcode::Field: {
                const Expected<uint32_t> packageLength = ParsePackageLength();
                if (!packageLength.HasValue()) ReturnFromBenchmark(false);
                const size_t tmp = pc;
                const Expected<String> name = ParseString();
                if (!name.HasValue()) ReturnFromBenchmark(false);
                const Expected<AMLFieldFlags> flags = Fetch<AMLFieldFlags>();
                if (!flags.HasValue()) ReturnFromBenchmark(false);
                AMLObject obj = AMLObject(name.Get(), MakeArray<uint64_t>(flags.Get().value), AMLObject::Type::Field);
                uint32_t length = packageLength.Get() - pc + tmp;
                while (length) {
                    const size_t tmp = pc;
                    const Expected<uint8_t> opcode = Fetch<uint8_t>();
                    if (!opcode.HasValue()) ReturnFromBenchmark(false);
                    switch (opcode.Get()) {
                        case 0x00: {
                            const Expected<uint32_t> packageLength = ParsePackageLength();
                            if (!packageLength.HasValue()) ReturnFromBenchmark(false);
                            // TODO: Verify package length
                            break;
                        }
                        case 0x01: {
                            // TODO: AccessType AccessAttrib
                            ReturnFromBenchmark(false);
                        }
                        case 0x02: {
                            // TODO: NameString | BufferData
                            ReturnFromBenchmark(false);
                        }
                        case 0x03: {
                            // TODO: AccessType ExtendedAccessAttrib AccessLength
                            ReturnFromBenchmark(false);
                        }
                        default: {
                            pc--;
                            const Expected<String> name = ParseNameSegment();
                            if (!name.HasValue() || !obj.AddChild(AMLObject(name.Get()))) ReturnFromBenchmark(false);
                            const Expected<uint32_t> packageLength = ParsePackageLength();
                            if (!packageLength.HasValue()) ReturnFromBenchmark(false);
                            // TODO: Verify package length
                            break;
                        }
                    }
                    const uint32_t size = pc - tmp;
                    if (size > length) ReturnFromBenchmark(false);
                    length -= size;
                }
                if (!object.AddChild(obj)) ReturnFromBenchmark(false);
                break;
            }
            default: ReturnFromBenchmark(false);
        }
        ReturnFromBenchmark(true);
    }
    bool AML::Run(void) {
        StartAndReturnFromBenchmark(Run(root));
    }
}