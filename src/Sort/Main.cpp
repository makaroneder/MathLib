#include "SignaledWritableSequence.hpp"
#include "Action.hpp"
#include <WindowManager.hpp>
#include <FileSystem/FileSystem.hpp>
#include <Interfaces/ComparisionFunction.hpp>
#include <Interfaces/Sequence/RandomSequence.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem&, MathLib::WindowManager& windowManager) {
    #ifndef Debug
    srand(time(nullptr));
    #endif
    const size_t width = 800;
    const size_t height = 800;
    const size_t blockWidth = 10;
    MathLib::Renderer* renderer = windowManager.MakeRenderer("Sort"_M, width, height);
    if (!renderer) MathLib::Panic("Failed to create renderer");
    MathLib::Array<size_t> base = MathLib::CollectionToArray<size_t>(MathLib::RandomSequence<size_t>(
        MathLib::Interval<size_t>(0, height - 1), width / blockWidth
    ));
    for (size_t x = 0; x < width; x += blockWidth) {
        const size_t h = base.AtUnsafe(x / blockWidth);
        for (size_t y = 0; y < height; y++) {
            const uint32_t color = y <= h ? UINT32_MAX : 0;
            const size_t ry = height - 1 - y;
            for (size_t rx = 0; rx < blockWidth; rx++) renderer->AtUnsafe(x + rx, ry) = color;
        }
    }
    if (!renderer->Update()) MathLib::Panic("Failed to update UI");
    bool running = true;
    const auto func = MathLib::MakeFunctionT<bool, size_t, size_t>([&renderer, &running, blockWidth, height](size_t x, size_t h) -> bool {
        for (size_t y = 0; y < height; y++) {
            const uint32_t color = y <= h ? UINT32_MAX : 0;
            const size_t ry = height - 1 - y;
            for (size_t rx = 0; rx < blockWidth; rx++)
                renderer->AtUnsafe(x * blockWidth + rx, ry) = color;
        }
        if (renderer->GetEvent().type != MathLib::Event::Type::Quit) return renderer->Update();
        running = false;
        return false;
    });
    SignaledWritableSequence<size_t> sequence = SignaledWritableSequence<size_t>(base, func);
    Action actions[] = {
        Action::CocktailShakerSort, Action::Reverse,
        Action::InsertationSort, Action::Reverse,
        Action::StoogeSort, Action::Reverse,
        Action::OddEvenSort, Action::Reverse,
        Action::BubbleSort, Action::Reverse,
    };
    const MathLib::ComparisionFunction<size_t> compare = MathLib::ComparisionFunction<size_t>(MathLib::ComparisionFunctionType::GreaterThan);
    const size_t size = SizeOfArray(actions);
    for (size_t i = 0; running; i = (i + 1) % size) {
        bool ret = false;
        switch (actions[i]) {
            case Action::Reverse: {
                ret = sequence.Reverse();
                break;
            }
            case Action::BubbleSort: {
                ret = sequence.BubbleSort(compare);
                break;
            }
            case Action::CocktailShakerSort: {
                ret = sequence.CocktailShakerSort(compare);
                break;
            }
            case Action::StoogeSort: {
                ret = sequence.StoogeSort(compare);
                break;
            }
            case Action::OddEvenSort: {
                ret = sequence.OddEvenSort(compare);
                break;
            }
            case Action::InsertationSort: {
                ret = sequence.InsertationSort(compare);
                break;
            }
            default: ret = false;
        }
        if (!ret && running) MathLib::Panic("Failed to vizualize array action");
    }
}