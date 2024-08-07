#include <P6.hpp>
#include <MathLib.hpp>
#include <Libc/HostFunction.hpp>
#include <Libc/HostFileSystem.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Preprocesor.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 4) Panic(String("Usage: ") + argv[0] + " <input file> <output image directory> <output file>");
        SDL2Renderer renderer = SDL2Renderer("Complex animation", 800, 800);
        if (!renderer.SetImage<P6>()) Panic("Failed to set image interface in renderer");
        HostFileSystem fs;
        Node* root = Tokenize(Preproces(fs, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        EquationSolverState state = EquationSolverState();
        Node* optimizedRoot = Optimize(root, state);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        state.runtime = true;
        const FunctionNode funcNode = state.GetFunction("f");
        Array<complex_t> domain;
        HostFunction<complex_t, complex_t> func;
        if (funcNode.domain == "R" && funcNode.codomain == "R") {
            for (num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier) domain.Add(complex_t(x, 0));
            func = HostFunction<complex_t, complex_t>(nullptr, [funcNode, state](const void*, complex_t z) -> complex_t {
                EquationSolverState tmp = state;
                tmp.variables.Add(Variable(funcNode.arguments[0].name, ToString(z.GetReal()), true));
                Node* n = Optimize(funcNode.body, tmp);
                const Array<complex_t> complexRet = n->ToNumber();
                delete n;
                return complex_t(z.GetReal(), (complexRet.GetSize() != 1 || complexRet.At(0).GetImaginary() != 0) ? MakeNaN() : complexRet.At(0).GetReal());
            });
        }
        else if (funcNode.domain == "C" && funcNode.codomain == "C") {
            for (num_t y = -4; y <= 4; y += 1 / renderer.pointMultiplier) {
                if (FloatsEqual<num_t>(y, Round(y)))
                    for (num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier) domain.Add(complex_t(x, y));
                else for (num_t x = -4; x <= 4; x++) domain.Add(complex_t(x, y));
            }
            func = HostFunction<complex_t, complex_t>(nullptr, [funcNode, state](const void*, complex_t z) -> complex_t {
                EquationSolverState tmp = state;
                tmp.variables.Add(Variable(funcNode.arguments[0].name, z.ToString(), true));
                Node* n = Optimize(funcNode.body, tmp);
                const complex_t ret = n->ToNumber().At(0);
                delete n;
                return ret;
            });
        }
        else Panic("Unsupported domain and codomain pair");
        const num_t iter = 100;
        Array<complex_t> directions = Array<complex_t>(domain.GetSize());
        for (size_t i = 0; i < domain.GetSize(); i++) directions.At(i) = (func(domain.At(i)) - domain.At(i)) / iter;
        for (num_t i = 0; i <= iter; i++) {
            renderer.Fill(0);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            for (size_t j = 0; j < domain.GetSize(); j++) {
                const complex_t dir = domain.At(j) + directions.At(j) * i;
                renderer.SetPixel<num_t>(CreateVector<num_t>(dir.GetReal(), dir.GetImaginary(), 0), 0xff0000ff);
            }
            if (!renderer.Update()) Panic("Failed to update renderer");
            if (!renderer.SaveFromPath(fs, String(argv[2]) + "/Stage" + ToString((size_t)i) + ".ppm")) Panic("Failed to save frame");
        }
        if (system((String("ffmpeg -hide_banner -y -framerate 30 -i ") + String(argv[2]) + String("/Stage%d.ppm -c:v libx264 -r 30 ") + String(argv[3])).GetValue())) Panic("Failed to generate video");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}