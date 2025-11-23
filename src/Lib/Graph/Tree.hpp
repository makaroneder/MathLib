#ifndef MathLib_Graph_Tree_H
#define MathLib_Graph_Tree_H
#include "../Bitmap.hpp"
#include "../Pair.hpp"

namespace MathLib {
    template <typename T>
    struct Tree : Printable {
        struct TreeElement {
            Tree<T> self;
            TreeElement* parent;
            Array<TreeElement*> children;
        
            TreeElement(const Tree<T>& self, TreeElement* parent) : self(self), parent(parent) {}
            ~TreeElement(void) {
                for (TreeElement*& child : children) delete child;
            }
            [[nodiscard]] const TreeElement* Find(const Sequence<char>& name) const {
                if (self.name == name) return this;
                for (const TreeElement* const& child : children) {
                    const TreeElement* tmp = child->Find(name);
                    if (tmp) return tmp;
                }
                return nullptr;
            }
            [[nodiscard]] bool AddChild(TreeElement* child) {
                if (!child) return false;
                child->parent = this;
                return children.Add(child);
            }
        };
        String name;
        T data;
        Array<Tree<T>> children;

        Tree(void) {}
        Tree(const Sequence<char>& name, const T& data) : name(CollectionToString(name)), data(data) {}
        Tree(const Sequence<char>& name, const T& data, const Array<Tree<T>>& children) : name(CollectionToString(name)), data(data), children(children) {}
        [[nodiscard]] Tree<T> AddNamePostfix(const Sequence<char>& namePostfix) const {
            Tree<T> ret = *this;
            ret.name += namePostfix;
            for (Tree<T>& child : ret.children) child = child.AddNamePostfix(namePostfix);
            return ret;
        }
        [[nodiscard]] bool Add(const Tree<T>& tree, const Sequence<char>& namePostfix = ""_M) {
            return children.Add(tree.AddNamePostfix(namePostfix));
        }
        [[nodiscard]] Array<Tree<T>> GetContaining(const Sequence<char>& name) const {
            Array<Tree<T>> ret;
            for (const Tree<T>& child : children) {
                if (child.name == name && !ret.Add(*this)) return Array<Tree<T>>();
                const Array<Tree<T>> tmp = child.GetContaining(name);
                ret += tmp;
            }
            return ret;
        }
        [[nodiscard]] Expected<Tree<T>> ChangeRoot(const Sequence<char>& n) const {
            TreeElement* doubleTree = ToDoubleTree();
            if (!doubleTree) return Expected<Tree<T>>();
            const TreeElement* rootInfo = doubleTree->Find(n);
            Tree<T> ret = rootInfo->self;
            Tree<T>* curr = &ret;
            Array<String> names = MakeArray<String>(ret.name);
            while (rootInfo->parent) {
                Tree<T> tmp = Tree<T>(rootInfo->parent->self.name, rootInfo->parent->self.data, Array<Tree<T>>());
                if (!names.Add(tmp.name)) return Expected<Tree<T>>();
                for (const Tree<T>& child : rootInfo->parent->self.children) {
                    if (names.Contains(child.name)) continue;
                    if (!tmp.Add(child)) return Expected<Tree<T>>();
                }
                if (!curr->Add(tmp)) return Expected<Tree<T>>();
                curr = &curr->children.At(curr->children.GetSize() - 1);
                rootInfo = rootInfo->parent;
            }
            delete doubleTree;
            return ret;
        }
        [[nodiscard]] Tree<T> GetMirror(void) const {
            Tree<T> ret = *this;
            if (!ret.children.GetSize()) return ret;
            size_t start = 0;
            size_t end = ret.children.GetSize() - 1;
            while (start < end) Swap<Tree<T>>(ret.children.At(start++), ret.children.At(end--));
            for (Tree<T>& child : ret.children) child = child.GetMirror();
            return ret;
        }
        [[nodiscard]] Pair<Tree<T>, size_t> GetFarthest(void) const {
            Pair<Tree<T>, size_t> ret = Pair<Tree<T>, size_t>(*this, 0);
            for (const Tree<T>& child : children) {
                const Pair<Tree<T>, size_t> tmp = child.GetFarthest();
                if (ret.second < tmp.second) ret = tmp;
            }
            ret.second++;
            return ret;
        }
        [[nodiscard]] Array<Tree<T>> GetPath(const Sequence<char>& target) const {
            if (name == target) return MakeArray<Tree<T>>(*this);
            for (const Tree<T>& child : children) {
                Array<Tree<T>> ret = child.GetPath(target);
                if (!ret.IsEmpty()) {
                    Array<Tree<T>> tmp = Array<Tree<T>>(ret.GetSize() + 1);
                    tmp.At(0) = *this;
                    for (size_t i = 0; i < ret.GetSize(); i++) tmp.At(i + 1) = ret.At(i);
                    return tmp;
                }
            }
            return Array<Tree<T>>();
        }
        [[nodiscard]] Array<Tree<T>> GetCenters(void) const {
            const Expected<Tree<T>> u = ChangeRoot(GetFarthest().first.name);
            if (!u.HasValue()) return Array<Tree<T>>();
            const Tree<T> v = u.Get().GetFarthest().first;
            const Array<Tree<T>> path = u.Get().GetPath(v.name);
            if (path.GetSize() % 2) {
                const Expected<Tree<T>> tmp = ChangeRoot(path.At(path.GetSize() / 2).name);
                return tmp.HasValue() ? MakeArray<Tree<T>>(tmp.Get()) : Array<Tree<T>>();
            }
            Array<Tree<T>> ret = Array<Tree<T>>(2);
            Expected<Tree<T>> tmp = ChangeRoot(path.At(path.GetSize() / 2).name);
            if (!tmp.HasValue()) return Array<Tree<T>>();
            ret.At(0) = tmp.Get();
            tmp = ChangeRoot(path.At((path.GetSize() - 1) / 2).name);
            if (!tmp.HasValue()) return Array<Tree<T>>();
            ret.At(1) = tmp.Get();
            return ret;
        }
        [[nodiscard]] Tree<T> Foreach(const Function<Tree<T>, Tree<T>>& func) const {
            Tree<T> ret = func(*this);
            for (Tree<T>& child : ret.children) child = child.Foreach(func);
            return ret;
        }
        [[nodiscard]] Expected<bool> StrictEquality(const Tree<T>& other) const {
            if (data != other.data || children.GetSize() != other.children.GetSize()) return false;
            const size_t size = children.GetSize();
            Bitmap used = Bitmap(size);
            for (size_t i = 0; i < size; i++) {
                bool foundMatch = false;
                for (size_t j = 0; j < size; j++) {
                    Expected<bool> tmp = used.Get(j);
                    if (!tmp.HasValue()) return Expected<bool>();
                    if (!tmp.Get()) {
                        tmp = children.At(i).StrictEquality(other.children.At(j));
                        if (!tmp.HasValue()) return Expected<bool>();
                        if (tmp.Get()) {
                            if (!used.Set(j, true)) return Expected<bool>();
                            foundMatch = true;
                            break;
                        }
                    }
                }
                if (!foundMatch) return false;
            }
            return true;
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            String ret = CollectionToString(padding) + name + " (" + Formatter<T>::ToString(data) + ')';
            if (children.IsEmpty()) return ret;
            ret += ": {\n";
            for (const Tree<T>& child : children) ret += child.ToString(CollectionToString(padding) + '\t') + '\n';
            return ret + padding + '}';
        }
        [[nodiscard]] bool operator==(const Tree<T>& other) const {
            const Array<Tree<T>> centers1 = GetCenters();
            const Array<Tree<T>> centers2 = other.GetCenters();
            for (const Tree<T>& center1 : centers1) {
                for (const Tree<T>& center2 : centers2) {
                    const Expected<bool> tmp = center1.StrictEquality(center2);
                    if (!tmp.HasValue()) return false;
                    if (tmp.Get()) return true;
                }
            }
            return false;
        }
        [[nodiscard]] bool operator!=(const Tree<T>& other) const {
            return !(*this == other);
        }

        private:
        [[nodiscard]] TreeElement* ToDoubleTree(void) const {
            TreeElement* ret = new TreeElement(*this, nullptr);
            for (const Tree<T>& child : children) {
                if (!ret->AddChild(child.ToDoubleTree())) {
                    delete ret;
                    return nullptr;
                }
            }
            return ret;
        }
    };
}

#endif