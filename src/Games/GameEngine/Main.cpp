#define SDL_MAIN_HANDLED
#include "Object.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Image/TGA/TGA.hpp>
#include <Image/PPM/P6.hpp>
#include <CommandLine.hpp>
#include <String.hpp>
#include <Host.hpp>
#include <JSON.hpp>
#include <SDL2.cpp>
#include <iostream>

MathLib::String path;
MathLib::Node* Log(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    for (const MathLib::Node* const& arg : args) std::cout << arg->value << std::endl;
    return nullptr;
}
MathLib::Event event;
MathLib::Node* GetEventData(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 1 || args.At(0)->type != MathLib::Node::Type::String) return nullptr;
    const MathLib::String name = args.At(0)->value;
    if (name == "type") return new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString((size_t)event.type, 10));
    else if (name == "mouseButton") return new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString((size_t)event.mouseButton, 10));
    else if (name == "pressed") return new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString(event.pressed, 10));
    else if (name == "key") return new MathLib::Node(MathLib::Node::Type::String, MathLib::String(event.key));
    else if (name == "mousePositionX") return new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString(GetX(event.mousePosition), 10));
    else if (name == "mousePositionY") return new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString(GetY(event.mousePosition), 10));
    else return nullptr;
}
MathLib::Array<MathLib::Image> images;
MathLib::Node* LoadImage(const void* data, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 2 || args.At(0)->type != MathLib::Node::Type::String || args.At(1)->type != MathLib::Node::Type::String) return nullptr;
    const MathLib::String type = args.At(0)->value;
    const MathLib::String name = path + args.At(1)->value;
    MathLib::FileSystem& fs = *(MathLib::FileSystem*)data;
    if (type == "P6") {
        MathLib::P6 tmp = MathLib::P6(0, 0);
        return tmp.LoadFromPath(fs, name) && images.Add(tmp) ? new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString(images.GetSize() - 1, 10)) : nullptr;
    }
    else if (type == "TGA") {
        MathLib::TGA tmp = MathLib::TGA(0, 0);
        return tmp.LoadFromPath(fs, name) && images.Add(tmp) ? new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString(images.GetSize() - 1, 10)) : nullptr;
    }
    else if (type == "Aseprite") {
        MathLib::Aseprite tmp = MathLib::Aseprite(0, 0);
        return tmp.LoadFromPath(fs, name) && images.Add(tmp) ? new MathLib::Node(MathLib::Node::Type::Constant, MathLib::ToString(images.GetSize() - 1, 10)) : nullptr;
    }
    else return nullptr;
}
MathLib::Node* DrawImage(const void* data, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 4) return nullptr;
    MathLib::num_t arg[4];
    for (size_t i = 0; i < 4; i++) {
        if (args.At(i)->type != MathLib::Node::Type::Constant) return nullptr;
        arg[i] = args.At(i)->ToNumber().At(0).ToReal();
    }
    MathLib::Renderer& renderer = *(MathLib::Renderer*)data;
    renderer.DrawImage<MathLib::num_t>(images.At(arg[0]), MathLib::CreateVector<MathLib::num_t>(arg[1], arg[2], arg[3]));
    return nullptr;
}
MathLib::Optimizer baseOptimizer = MathLib::Optimizer(std::vector<MathLib::BuiltinFunction> {
    MathLib::BuiltinFunction("Log", MathLib::FunctionPointer<MathLib::Node*, const MathLib::Array<const MathLib::Node*>&>(nullptr, &Log)),
    MathLib::BuiltinFunction("GetEventData", MathLib::FunctionPointer<MathLib::Node*, const MathLib::Array<const MathLib::Node*>&>(nullptr, &GetEventData)),
    MathLib::BuiltinFunction("LoadImage", MathLib::FunctionPointer<MathLib::Node*, const MathLib::Array<const MathLib::Node*>&>(nullptr, &LoadImage)),
    MathLib::BuiltinFunction("DrawImage", MathLib::FunctionPointer<MathLib::Node*, const MathLib::Array<const MathLib::Node*>&>(nullptr, &DrawImage)),
}, std::vector<MathLib::FunctionNode> {}, std::vector<MathLib::Variable> {
    MathLib::Variable("noneEvent", "EventType", MathLib::ToString((size_t)MathLib::Event::Type::None, 10), true),
    MathLib::Variable("quitEvent", "EventType", MathLib::ToString((size_t)MathLib::Event::Type::Quit, 10), true),
    MathLib::Variable("keyPressedEvent", "EventType", MathLib::ToString((size_t)MathLib::Event::Type::KeyPressed, 10), true),
    MathLib::Variable("mousePressedEvent", "EventType", MathLib::ToString((size_t)MathLib::Event::Type::MousePressed, 10), true),
    MathLib::Variable("noButton", "MouseButton", MathLib::ToString((size_t)MathLib::Event::MouseButton::None, 10), true),
    MathLib::Variable("leftButton", "MouseButton", MathLib::ToString((size_t)MathLib::Event::MouseButton::Left, 10), true),
    MathLib::Variable("middleButton", "MouseButton", MathLib::ToString((size_t)MathLib::Event::MouseButton::Middle, 10), true),
    MathLib::Variable("rightButton", "MouseButton", MathLib::ToString((size_t)MathLib::Event::MouseButton::Right, 10), true),
    MathLib::Variable("button4", "MouseButton", MathLib::ToString((size_t)MathLib::Event::MouseButton::Button4, 10), true),
    MathLib::Variable("button5", "MouseButton", MathLib::ToString((size_t)MathLib::Event::MouseButton::Button5, 10), true),
});
void OnStatus(MathLib::Node* status) {
    if (status->type != MathLib::Node::Type::Constant) MathLib::Panic("Invalid status type");
    if (!status->ToNumber().At(0).ToReal()) MathLib::Panic("Error detected");
    delete status;
}

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::HostFileSystem fs;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        MathLib::JSON json;
        path = cmdLine.GetEntry("program").Get("No game specified");
        if (!json.LoadFromPath(fs, path)) MathLib::Panic("Failed to load game data");
        #ifdef Debug
        std::cout << "Game: " << json << std::endl;
        #endif
        size_t tmp = 0;
        while (true) {
            const size_t t = path.Find('/', tmp);
            if (t == SIZE_MAX) break;
            tmp = t + 1;
        }
        path = MathLib::SubString(path, 0, tmp);
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Game engine", 800, 800);
        if (!baseOptimizer.SetBuiltinFunctionData("DrawImage", &renderer)) MathLib::Panic("Failed to set builtin function data");
        if (!baseOptimizer.SetBuiltinFunctionData("LoadImage", &fs)) MathLib::Panic("Failed to set builtin function data");
        Object root = Object(baseOptimizer, fs, path, json);
        const MathLib::JSON jsonObjects = json.Find("objects").Get("Game data must contain objects field");
        MathLib::Array<Object> objects = MathLib::Array<Object>(jsonObjects.GetChildrenCount());
        for (const MathLib::JSON& object : jsonObjects)
            if (!objects.Add(Object(baseOptimizer, fs, path, object))) MathLib::Panic("Failed to load object");
        MathLib::num_t prev = MathLib::GetTime();
        while (true) {
            const MathLib::num_t curr = MathLib::GetTime();
            const MathLib::String deltaTime = MathLib::ToString(curr - prev);
            prev = curr;
            event = renderer.GetEvent();
            OnStatus(root.Call("OnEvent", deltaTime));
            for (Object& object : objects) OnStatus(object.Call("Update", deltaTime));
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            if (event.type == MathLib::Event::Type::Quit) break;
        }
        for (Object& object : objects) object.Destroy();
        root.Destroy();
        baseOptimizer.Destroy();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}