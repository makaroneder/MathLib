#define SDL_MAIN_HANDLED
#include <Libc/HostFileSystem.hpp>
#include <EquationSolver/Preprocesor.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Libc/HostFunction.hpp>
#include <Image/PPM/P6.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 4) MathLib::Panic("Usage: "_M + argv[0] + " <input file> <output image directory> <output file>");
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Complex animation", 800, 800);
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
            for (MathLib::num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier)
                if (!domain.Add(MathLib::complex_t(x, 0))) MathLib::Panic("Failed to add point to domain");
            func = MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t>([funcNode, optimizer](MathLib::complex_t z) -> MathLib::complex_t {
                MathLib::Optimizer tmp = optimizer;
                if (!tmp.variables.Add(MathLib::Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, MathLib::ToString(z.GetReal()), true))) return MathLib::complex_t(MathLib::MakeNaN(), MathLib::MakeNaN());
                MathLib::Node* n = tmp.Optimize(funcNode.body);
                const MathLib::Array<MathLib::complex_t> complexRet = n->ToNumber();
                delete n;
                return MathLib::complex_t(z.GetReal(), (complexRet.GetSize() != 1 || complexRet.At(0).GetImaginary() != 0) ? MathLib::MakeNaN() : complexRet.At(0).GetReal());
            });
        }
        else if (funcNode.dataType == "C") {
            for (MathLib::num_t y = -4; y <= 4; y += 1 / renderer.pointMultiplier) {
                if (MathLib::FloatsEqual<MathLib::num_t>(y, MathLib::Round(y))) {
                    for (MathLib::num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier)
                        if (!domain.Add(MathLib::complex_t(x, y))) MathLib::Panic("Failed to add point to domain");
                }
                else for (MathLib::num_t x = -4; x <= 4; x++)
                    if (!domain.Add(MathLib::complex_t(x, y))) MathLib::Panic("Failed to add point to domain");
            }
            func = MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t>([funcNode, optimizer](MathLib::complex_t z) -> MathLib::complex_t {
                MathLib::Optimizer tmp = optimizer;
                if (!tmp.variables.Add(MathLib::Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, z.ToString(), true))) return MathLib::complex_t(MathLib::MakeNaN(), MathLib::MakeNaN());
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
            renderer.DrawAxis<MathLib::num_t>(0xffffffff, 0x808080ff, 1);
            for (size_t j = 0; j < domain.GetSize(); j++) {
                const MathLib::complex_t dir = domain.At(j) + directions.At(j) * i;
                renderer.SetPixel<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(dir.GetReal(), dir.GetImaginary(), 0), 0xff0000ff);
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update renderer");
            if (!renderer.SaveFromPath(fs, MathLib::String(argv[2]) + "/Stage" + MathLib::ToString((size_t)i) + ".ppm")) MathLib::Panic("Failed to save frame");
        }
        if (system(("ffmpeg -hide_banner -y -framerate 30 -i "_M + MathLib::String(argv[2]) + "/Stage%d.ppm -c:v libx264 -r 30 "_M + MathLib::String(argv[3])).GetValue())) MathLib::Panic("Failed to generate video");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}