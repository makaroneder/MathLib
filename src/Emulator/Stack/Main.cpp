#include <Emulator/StepEmulator.hpp>
#include <Interfaces/Sequence/VariadicSequence.hpp>
#include <Interfaces/Sequence/FixedSizeCollection.hpp>
#include <iostream>

struct StackEmulator : MathLib::StepEmulator, MathLib::Printable {
    enum class Opcode : uint8_t {
        Nop = 0x00,
        Push,
        Loop,
        Add,
    };
    StackEmulator(const MathLib::Sequence<uint8_t>& memory);
    [[nodiscard]] virtual bool Reset(void) override;
    [[nodiscard]] virtual bool Step(void) override;
    [[nodiscard]] virtual MathLib::Register GetPC(void) const override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    template <typename T>
    [[nodiscard]] MathLib::Expected<T> Fetch(void) {
        const MathLib::Expected<T> ret = ReadPositioned<T>(pc.value);
        if (ret.HasValue()) pc.value += sizeof(T);
        return ret;
    }
    template <typename T>
    [[nodiscard]] MathLib::Expected<T> Pop(void) {
        if (stack.GetSize() < sizeof(T)) return MathLib::Expected<T>();
        MathLib::FixedSizeCollection<uint8_t, sizeof(T)> buffer;
        for (size_t i = 0; i < sizeof(T); i++) buffer.At(i) = stack.At(stack.GetSize() - i - 1);
        MathLib::Array<uint8_t> tmp = MathLib::Array<uint8_t>(stack.GetSize() - sizeof(T));
        for (size_t i = 0; i < tmp.GetSize(); i++) tmp.At(i) = stack.At(i);
        stack = tmp;
        return *(const T*)buffer.GetValue();
    }
    template <typename T>
    [[nodiscard]] bool Push(const T& value) {
        MathLib::FixedSizeCollection<uint8_t, sizeof(T)> buffer;
        *(T*)buffer.GetValue() = value;
        for (size_t i = 0; i < sizeof(T); i++)
            if (!stack.Add(buffer.At(i))) return false;
        return true;
    }

    MathLib::Array<uint8_t> stack;
    MathLib::Register pc;
};
StackEmulator::StackEmulator(const MathLib::Sequence<uint8_t>& memory) : MathLib::StepEmulator(memory), stack(), pc(0) {}
bool StackEmulator::Reset(void) {
    pc = 0;
    return stack.Reset();
}
bool StackEmulator::Step(void) {
    const MathLib::Expected<Opcode> opcode = Fetch<Opcode>();
    if (!opcode.HasValue()) return false;
    switch (opcode.Get()) {
        case Opcode::Nop: return true;
        case Opcode::Push: {
            const MathLib::Expected<uint8_t> value = Fetch<uint8_t>();
            return value.HasValue() && Push<uint8_t>(value.Get());
        }
        case Opcode::Loop: {
            const MathLib::Expected<uint8_t> limit = Pop<uint8_t>();
            if (!limit.HasValue()) return false;
            const MathLib::Register save = pc;
            for (uint16_t i = 0; i < limit.Get(); i++) {
                pc = save;
                if (!Step()) return false;
            }
            return true;
        }
        case Opcode::Add: {
            const MathLib::Expected<uint8_t> a = Pop<uint8_t>();
            if (!a.HasValue()) return false;
            const MathLib::Expected<uint8_t> b = Pop<uint8_t>();
            if (!b.HasValue()) return false;
            return Push<uint8_t>(a.Get() + b.Get());
        }
        default: return false;
    }
}
MathLib::Register StackEmulator::GetPC(void) const {
    return pc;
}
MathLib::String StackEmulator::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = "{\n"_M + padding + "\tPC: " + pc.ToString() + '\n' + padding + "\tStack: {\n";
    for (const uint8_t& x : stack)
        ret += MathLib::CollectionToString(padding) + "\t\t0x" + MathLib::ToString(x, 16, 2) + '\n';
    return ret + padding + "\t}\n" +  padding + "}";
}

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        StackEmulator emulator = StackEmulator(MathLib::VariadicSequence<uint8_t,
            (uint8_t)StackEmulator::Opcode::Nop,
            (uint8_t)StackEmulator::Opcode::Push, 0x10,
            (uint8_t)StackEmulator::Opcode::Push, 0x01,
            (uint8_t)StackEmulator::Opcode::Push, 0x10,
            (uint8_t)StackEmulator::Opcode::Push, 0x02,
            (uint8_t)StackEmulator::Opcode::Loop,
            (uint8_t)StackEmulator::Opcode::Add
        >());
        if (!emulator.Run()) MathLib::Panic("Failed to update emulator");
        std::cout << "State: " << emulator << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}