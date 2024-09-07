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
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        if (argc < 4) Panic(String("Usage: ") + argv[0] + " <input file> <output image directory> <output file>");
        SDL2Renderer renderer = SDL2Renderer("Complex animation", 800, 800);
        if (!renderer.SetImage<P6>()) Panic("Failed to set image interface in renderer");
        HostFileSystem fs;
        Node* root = Tokenize(Preproces(fs, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        Optimizer optimizer = Optimizer();
        Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.runtime = true;
        const FunctionNode funcNode = optimizer.GetFunction("f");
        Array<complex_t> domain;
        HostFunction<complex_t, complex_t> func;
        if (funcNode.dataType == "R") {
            for (num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier) domain.Add(complex_t(x, 0));
            func = HostFunction<complex_t, complex_t>(nullptr, [funcNode, optimizer](const void*, complex_t z) -> complex_t {
                Optimizer tmp = optimizer;
                tmp.variables.Add(Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, ToString(z.GetReal()), true));
                Node* n = tmp.Optimize(funcNode.body);
                const Array<complex_t> complexRet = n->ToNumber();
                delete n;
                return complex_t(z.GetReal(), (complexRet.GetSize() != 1 || complexRet.At(0).GetImaginary() != 0) ? MakeNaN() : complexRet.At(0).GetReal());
            });
        }
        else if (funcNode.dataType == "C") {
            for (num_t y = -4; y <= 4; y += 1 / renderer.pointMultiplier) {
                if (FloatsEqual<num_t>(y, Round(y)))
                    for (num_t x = -4; x <= 4; x += 1 / renderer.pointMultiplier) domain.Add(complex_t(x, y));
                else for (num_t x = -4; x <= 4; x++) domain.Add(complex_t(x, y));
            }
            func = HostFunction<complex_t, complex_t>(nullptr, [funcNode, optimizer](const void*, complex_t z) -> complex_t {
                Optimizer tmp = optimizer;
                tmp.variables.Add(Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, z.ToString(), true));
                Node* n = tmp.Optimize(funcNode.body);
                const complex_t ret = n->ToNumber().At(0);
                delete n;
                return ret;
            });
        }
        else Panic("Unsupported function domain");
        const num_t iter = 100;
        Array<complex_t> directions = Array<complex_t>(domain.GetSize());
        for (size_t i = 0; i < domain.GetSize(); i++) directions.At(i) = (func(domain.At(i)) - domain.At(i)) / iter;
        optimizer.Destroy();
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