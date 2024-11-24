#define SDL_MAIN_HANDLED
#include <MathLib.hpp>
#include <Libc/HostFunction.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 4) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <input file> <output image directory> <output file>");
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Complex animation", 800, 800);
        if (!renderer.SetImage<MathLib::P6>()) MathLib::Panic("Failed to set image interface in renderer");
        MathLib::HostFileSystem fs;
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, argv[1]));
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
        const MathLib::FunctionNode funcNode = optimizer.GetFunction("f");
        MathLib::Array<MathLib::complex_t> domain;
        MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t> func;
        if (funcNode.dataType == "R") {
            for (MathLib::num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier) domain.Add(MathLib::complex_t(x, 0));
            func = MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t>(nullptr, [funcNode, optimizer](const void*, MathLib::complex_t z) -> MathLib::complex_t {
                MathLib::Optimizer tmp = optimizer;
                tmp.variables.Add(MathLib::Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, MathLib::ToString(z.GetReal()), true));
                MathLib::Node* n = tmp.Optimize(funcNode.body);
                const MathLib::Array<MathLib::complex_t> complexRet = n->ToNumber();
                delete n;
                return MathLib::complex_t(z.GetReal(), (complexRet.GetSize() != 1 || complexRet.At(0).GetImaginary() != 0) ? MathLib::MakeNaN() : complexRet.At(0).GetReal());
            });
        }
        else if (funcNode.dataType == "C") {
            for (MathLib::num_t y = -4; y <= 4; y += 1 / renderer.pointMultiplier) {
                if (MathLib::FloatsEqual<MathLib::num_t>(y, MathLib::Round(y)))
                    for (MathLib::num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier) domain.Add(MathLib::complex_t(x, y));
                else for (MathLib::num_t x = -4; x <= 4; x++) domain.Add(MathLib::complex_t(x, y));
            }
            func = MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t>(nullptr, [funcNode, optimizer](const void*, MathLib::complex_t z) -> MathLib::complex_t {
                MathLib::Optimizer tmp = optimizer;
                tmp.variables.Add(MathLib::Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, z.ToString(), true));
                MathLib::Node* n = tmp.Optimize(funcNode.body);
                const MathLib::complex_t ret = n->ToNumber().At(0);
                delete n;
                return ret;
            });
        }
        else MathLib::Panic("Unsupported function domain");
        const MathLib::num_t iter = 100;
        MathLib::Array<MathLib::complex_t> directions = MathLib::Array<MathLib::complex_t>(domain.GetSize());
        for (size_t i = 0; i < domain.GetSize(); i++) directions.At(i) = (func(domain.At(i)) - domain.At(i)) / iter;
        optimizer.Destroy();
        for (MathLib::num_t i = 0; i <= iter; i++) {
            renderer.Fill(0);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            for (size_t j = 0; j < domain.GetSize(); j++) {
                const MathLib::complex_t dir = domain.At(j) + directions.At(j) * i;
                renderer.SetPixel<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(dir.GetReal(), dir.GetImaginary(), 0), 0xff0000ff);
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update renderer");
            if (!renderer.SaveFromPath(fs, MathLib::String(argv[2]) + "/Stage" + MathLib::ToString((size_t)i) + ".ppm")) MathLib::Panic("Failed to save frame");
        }
        if (system((MathLib::String("ffmpeg -hide_banner -y -framerate 30 -i ") + MathLib::String(argv[2]) + MathLib::String("/Stage%d.ppm -c:v libx264 -r 30 ") + MathLib::String(argv[3])).GetValue())) MathLib::Panic("Failed to generate video");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}