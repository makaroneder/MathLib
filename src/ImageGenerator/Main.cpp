#define SDL_MAIN_HANDLED
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <NeuralNetwork.hpp>
#include <ExternArray.hpp>
#include <SDL2.cpp>
#include <iostream>

[[nodiscard]] bool Train(MathLib::NeuralNetwork<MathLib::num_t>& neuralNetwork, const MathLib::matrix_t& argsMatrix, const MathLib::Sequence<MathLib::matrix_t>& trainingData) {
    const size_t size = trainingData.GetSize();
    for (size_t i = 0; i < size; i++) {
        #ifdef UseFiniteDiff
        const MathLib::NeuralNetwork<MathLib::num_t> diff = neuralNetwork.FiniteDiff(argsMatrix, trainingData.At(i));
        #else
        const MathLib::Expected<MathLib::NeuralNetwork<MathLib::num_t>> tmp = neuralNetwork.Backprop(argsMatrix, trainingData.At(i));
        if (!tmp.HasValue()) return false;
        const MathLib::NeuralNetwork<MathLib::num_t> diff = tmp.Get();
        #endif
        neuralNetwork.Learn(diff, 1e-1);
    }
    return true;
}
void PrintState(MathLib::NeuralNetwork<MathLib::num_t>& neuralNetwork, const MathLib::matrix_t& inputData, const MathLib::Sequence<MathLib::matrix_t>& imageData) {
    const size_t size = imageData.GetSize();
    std::cout << neuralNetwork << '\n';
    for (size_t j = 0; j < size; j++)
        std::cout << "Cost " << j << ": " << neuralNetwork.Cost(inputData, imageData.At(j)) << '\n';
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #ifdef Debug
        const MathLib::num_t start = MathLib::GetTime();
        #else
        srand(time(nullptr));
        #endif
        if (argc < 3) MathLib::Panic("Usage: "_M + argv[0] + " <model> <input data>");
        const MathLib::String model = argv[1];
        MathLib::HostFileSystem fs;
        MathLib::NeuralNetwork<MathLib::num_t> neuralNetwork;
        if (!neuralNetwork.LoadFromPath(fs, model)) {
            neuralNetwork = MathLib::NeuralNetwork<MathLib::num_t>(MathLib::NeuralNetwork<MathLib::num_t>::ActivationFunction::Sigmoid, MathLib::MakeArray<size_t>(
                2, 16, 1
            ));
            neuralNetwork.Random(0, 1);
        }
        MathLib::Aseprite data;
        if (!data.LoadFromPath(fs, MathLib::String(argv[2]))) MathLib::Panic("Failed to load input data");
        const size_t width = data.GetWidth();
        const size_t height = data.GetHeight();
        MathLib::matrix_t inputData = MathLib::matrix_t(2, width * height);
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                inputData.At(0, y * width + x) = (MathLib::num_t)x / (width - 1);
                inputData.At(1, y * width + x) = (MathLib::num_t)y / (height - 1);
            }
        }
        #if 0
        const size_t frames = data.GetFrameCount();
        #else
        const size_t frames = 1;
        #endif
        MathLib::matrix_t buff[frames];
        for (size_t i = 0; i < frames; i++) {
            buff[i] = MathLib::matrix_t(1, width * height);
            const MathLib::Image& frame = data.At(i);
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++)
                    buff[i].At(0, y * width + x) = (MathLib::num_t)frame.At(x, y) / UINT32_MAX;
        }
        const MathLib::ExternArray<MathLib::matrix_t> imageData = MathLib::ExternArray<MathLib::matrix_t>(buff, frames);

        #ifdef Debug
        std::cout << "Start:\n";
        PrintState(neuralNetwork, inputData, imageData);
        #endif
        const size_t limit = 50;
        const size_t printStatements = 10;
        for (size_t i = 0; i < limit; i++) {
            if (!Train(neuralNetwork, inputData, imageData)) MathLib::Panic("Failed to train neural network");
            #ifdef Debug
            if (!(i % (limit / printStatements))) {
                std::cout << i << ":\n";
                PrintState(neuralNetwork, inputData, imageData);
            }
            #endif
        }
        #ifdef Debug
        std::cout << "End:\n";
        PrintState(neuralNetwork, inputData, imageData);
        #endif
        if (!neuralNetwork.SaveFromPath(fs, model)) MathLib::Panic("Failed to save neural network");
        #ifdef Debug
        std::cout << "Time: " << MathLib::GetTime() - start << " second(s)" << std::endl;
        #endif

        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Image generator"_M, width, height);
        MathLib::Image image = MathLib::Image(width, height);
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++)
                image.pixels.At(x, y) = neuralNetwork.Run(MathLib::MakeArray<MathLib::num_t>((MathLib::num_t)x / (width - 1), (MathLib::num_t)y / (height - 1))).At(0, 0) * UINT32_MAX;
        renderer.DrawImage<MathLib::num_t>(image, MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        while (renderer.GetEvent().type != MathLib::Event::Type::Quit) (void)renderer.Update();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}