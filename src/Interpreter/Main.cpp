#include <MathLib.hpp>
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
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <input file>");
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
        FunctionNode main = optimizer.GetFunction("Main");
        Array<Node*> args = Array<Node*>(argc - 1);
        for (int i = 1; i < argc; i++) args.At(i - 1) = new Node(Node::Type::String, argv[i]);
        optimizer.variables.Add(Variable(main.arguments[0].name, main.arguments[0].dataType, ToString(argc - 1), true));
        optimizer.variables.Add(Variable(main.arguments[1].name, main.arguments[1].dataType, ArrayToComma(args), true));
        Node* tmp = optimizer.Optimize(main.body);
        std::cout << "Return value: " << *tmp << std::endl;
        delete tmp;
        optimizer.Destroy();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}