#ifndef Node_H
#define Node_H
#include <Interfaces/Printable.hpp>

struct Node : MathLib::Collection<Node>, MathLib::Printable {
    Node(void);
    Node(size_t type, const MathLib::Sequence<char>& value, const MathLib::Sequence<Node>& children);
    [[nodiscard]] size_t GetType(void) const;
    [[nodiscard]] MathLib::String GetData(void) const;
    [[nodiscard]] virtual Node At(size_t index) const override;
    [[nodiscard]] virtual Node& At(size_t index) override;
    [[nodiscard]] virtual bool Add(const Node& val) override;
    [[nodiscard]] virtual bool Reset(void) override;
    [[nodiscard]] virtual const Node* GetValue(void) const override;
    [[nodiscard]] virtual size_t GetSize(void) const override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const Node& other) const;
    [[nodiscard]] bool operator!=(const Node& other) const;

    private:
    MathLib::String value;
    MathLib::Array<Node> children;
    size_t type;
};

#endif