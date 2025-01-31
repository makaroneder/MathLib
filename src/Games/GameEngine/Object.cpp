#include "Object.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <String.hpp>

size_t uniqueName = 0;
Object::Object(void) : name(), optimizer(nullptr) {}
Object::Object(MathLib::Optimizer& baseOptimizer, MathLib::FileSystem& fileSystem, const MathLib::String& path, const MathLib::JSON& json) {
    const MathLib::JSON n = json.Find("name").Get("Every object must contain name field");
    if (n.GetType() == MathLib::JSON::Type::String) name = n.GetValue();
    else name = MathLib::ToString(uniqueName++, 10);
    const MathLib::JSON script = json.Find("script").Get("Every object must contain script field");
    if (script.GetType() == MathLib::JSON::Type::String) {
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fileSystem, path + script.GetValue()));
        MathLib::Optimizer ret = baseOptimizer.Recreate();
        delete ret.Optimize(root);
        delete root;
        ret.runtime = true;
        optimizer = new MathLib::Optimizer(ret);
    }
    else optimizer = nullptr;
}
MathLib::String Object::GetName(void) const {
    return name;
}
MathLib::Node* Object::Call(const MathLib::String& function, const MathLib::String& arg) const {
    if (!optimizer) return new MathLib::Node(MathLib::Node::Type::Constant, "1");
    MathLib::Optimizer tmp = optimizer->Recreate();
    MathLib::FunctionNode node = tmp.GetFunction(function);
    if (!tmp.variables.Add(MathLib::Variable(node.arguments.At(0).name, node.arguments.At(0).dataType, arg, node.arguments.At(0).constant))) return nullptr;
    MathLib::Node* ret = tmp.Optimize(node.body);
    tmp.Destroy();
    return ret;
}
void Object::Destroy(void) {
    if (optimizer) {
        optimizer->Destroy();
        delete optimizer;
        optimizer = nullptr;
    }
}