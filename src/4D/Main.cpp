#include <Geometry/Hyperrectangle.hpp>
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
[[nodiscard]] MathLib::Array<T> GetArray(MathLib::Optimizer& optimizer, const MathLib::FunctionNode& func, size_t variable, const MathLib::String& value) {
    optimizer.variables.At(variable).value->value = value;
    MathLib::Node* tmp = optimizer.Optimize(func.body);
    const MathLib::Array<MathLib::complex_t> tmpArr = tmp->ToNumber();
    MathLib::Array<T> ret = MathLib::Array<T>(tmpArr.GetSize());
    for (size_t i = 0; i < tmpArr.GetSize(); i++) ret.At(i) = (T)tmpArr.At(i);
    delete tmp;
    return ret;
}
[[nodiscard]] size_t FindVariable(const MathLib::Array<MathLib::Variable>& variables, const MathLib::String& name) {
    for (size_t i = 0; i < variables.GetSize(); i++)
        if (name == variables.At(i).name) return i;
    return SIZE_MAX;
}

#define AddFunction(vname, identifier)                                                                                                                                      \
    MathLib::FunctionNode vname = optimizer.GetFunction(identifier);                                                                                                        \
    size_t vname##Variable = FindVariable(optimizer.variables, vname.arguments.At(0).name);                                                                                 \
    if (vname##Variable == SIZE_MAX) {                                                                                                                                      \
        if (!optimizer.variables.Add(MathLib::Variable(vname.arguments.At(0).name, vname.arguments.At(0).dataType, "0", true))) MathLib::Panic("Failed to add variable");   \
        vname##Variable = optimizer.variables.GetSize() - 1;                                                                                                                \
    }
#define FunctionToArray(name) const MathLib::Array<MathLib::num_t> name##Arr = GetArray<MathLib::num_t>(optimizer, name, name##Variable, time)

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("4D viewer", 800, 800);
        MathLib::HostFileSystem fs;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, cmdLine.GetEntry("program").Get("No program specified")));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer();
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.runtime = true;
        AddFunction(position, "p")
        AddFunction(size, "s")
        AddFunction(rotation, "r")
        AddFunction(color, "c")
        const MathLib::num_t startTime = MathLib::GetTime();
        bool pause = false;
        while (true) {
            if (!pause) {
                const MathLib::String time = MathLib::ToString(MathLib::GetTime() - startTime);
                FunctionToArray(position);
                FunctionToArray(size);
                FunctionToArray(rotation);
                FunctionToArray(color);
                renderer.Fill(0);
                renderer.DrawShape<MathLib::num_t>(MathLib::Hyperrectangle<MathLib::num_t>(MathLib::matrix_t(positionArr.GetSize(), 1, positionArr), MathLib::matrix_t(sizeArr.GetSize(), 1, sizeArr)), MathLib::matrix_t(rotationArr.GetSize(), 1, rotationArr), MathLib::Color::FromVector<MathLib::num_t>(MathLib::matrix_t(colorArr.GetSize(), 1, colorArr)).hex);
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed && event.key == ' ') pause = !pause;
        }
        optimizer.Destroy();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}