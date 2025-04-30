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
        
            TreeElement(const Tree<T>& self, TreeElement* parent) : self(self), parent(parent) {
                EmptyBenchmark
            }
            ~TreeElement(void) {
                StartBenchmark
                for (TreeElement*& child : children) delete child;
                EndBenchmark
            }
            [[nodiscard]] const TreeElement* Find(const Sequence<char>& name) const {
                StartBenchmark
                if (self.name == name) ReturnFromBenchmark(this);
                for (const TreeElement* const& child : children) {
                    const TreeElement* tmp = child->Find(name);
                    if (tmp) ReturnFromBenchmark(tmp);
                }
                ReturnFromBenchmark(nullptr);
            }
            [[nodiscard]] bool AddChild(TreeElement* child) {
                StartBenchmark
                if (!child) ReturnFromBenchmark(false);
                child->parent = this;
                ReturnFromBenchmark(children.Add(child));
            }
        };
        String name;
        T data;
        Array<Tree<T>> children;

        Tree(void) {
            EmptyBenchmark
        }
        Tree(const Sequence<char>& name, const T& data) : name(CollectionToString(name)), data(data) {
            EmptyBenchmark
        }
        Tree(const Sequence<char>& name, const T& data, const Array<Tree<T>>& children) : name(CollectionToString(name)), data(data), children(children) {
            EmptyBenchmark
        }
        [[nodiscard]] Tree<T> AddNamePostfix(const Sequence<char>& namePostfix) const {
            StartBenchmark
            Tree<T> ret = *this;
            ret.name += namePostfix;
            for (Tree<T>& child : ret.children) child = child.AddNamePostfix(namePostfix);
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] bool Add(const Tree<T>& tree, const Sequence<char>& namePostfix = ""_M) {
            StartAndReturnFromBenchmark(children.Add(tree.AddNamePostfix(namePostfix)));
        }
        [[nodiscard]] Array<Tree<T>> GetContaining(const Sequence<char>& name) const {
            StartBenchmark
            Array<Tree<T>> ret;
            for (const Tree<T>& child : children) {
                if (child.name == name && !ret.Add(*this)) ReturnFromBenchmark(Array<Tree<T>>());
                const Array<Tree<T>> tmp = child.GetContaining(name);
                ret += tmp;
            }
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Expected<Tree<T>> ChangeRoot(const Sequence<char>& n) const {
            StartBenchmark
            TreeElement* doubleTree = ToDoubleTree();
            if (!doubleTree) ReturnFromBenchmark(Expected<Tree<T>>());
            const TreeElement* rootInfo = doubleTree->Find(n);
            Tree<T> ret = rootInfo->self;
            Tree<T>* curr = &ret;
            Array<String> names = MakeArray<String>(ret.name);
            while (rootInfo->parent) {
                Tree<T> tmp = Tree<T>(rootInfo->parent->self.name, rootInfo->parent->self.data, Array<Tree<T>>());
                if (!names.Add(tmp.name)) ReturnFromBenchmark(Expected<Tree<T>>());
                for (const Tree<T>& child : rootInfo->parent->self.children) {
                    if (names.Contains(child.name)) continue;
                    if (!tmp.Add(child)) ReturnFromBenchmark(Expected<Tree<T>>());
                }
                if (!curr->Add(tmp)) ReturnFromBenchmark(Expected<Tree<T>>());
                curr = &curr->children.At(curr->children.GetSize() - 1);
                rootInfo = rootInfo->parent;
            }
            delete doubleTree;
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Tree<T> GetMirror(void) const {
            StartBenchmark
            Tree<T> ret = *this;
            if (!ret.children.GetSize()) ReturnFromBenchmark(ret);
            size_t start = 0;
            size_t end = ret.children.GetSize() - 1;
            while (start < end) Swap<Tree<T>>(ret.children.At(start++), ret.children.At(end--));
            for (Tree<T>& child : ret.children) child = child.GetMirror();
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Pair<Tree<T>, size_t> GetFarthest(void) const {
            StartBenchmark
            Pair<Tree<T>, size_t> ret = Pair<Tree<T>, size_t>(*this, 0);
            for (const Tree<T>& child : children) {
                const Pair<Tree<T>, size_t> tmp = child.GetFarthest();
                if (ret.second < tmp.second) ret = tmp;
            }
            ret.second++;
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Array<Tree<T>> GetPath(const Sequence<char>& target) const {
            StartBenchmark
            if (name == target) ReturnFromBenchmark(MakeArray<Tree<T>>(*this));
            for (const Tree<T>& child : children) {
                Array<Tree<T>> ret = child.GetPath(target);
                if (!ret.IsEmpty()) {
                    Array<Tree<T>> tmp = Array<Tree<T>>(ret.GetSize() + 1);
                    tmp.At(0) = *this;
                    for (size_t i = 0; i < ret.GetSize(); i++) tmp.At(i + 1) = ret.At(i);
                    ReturnFromBenchmark(tmp);
                }
            }
            ReturnFromBenchmark(Array<Tree<T>>());
        }
        [[nodiscard]] Array<Tree<T>> GetCenters(void) const {
            StartBenchmark
            const Expected<Tree<T>> u = ChangeRoot(GetFarthest().first.name);
            if (!u.HasValue()) ReturnFromBenchmark(Array<Tree<T>>());
            const Tree<T> v = u.Get().GetFarthest().first;
            const Array<Tree<T>> path = u.Get().GetPath(v.name);
            if (path.GetSize() % 2) {
                const Expected<Tree<T>> tmp = ChangeRoot(path.At(path.GetSize() / 2).name);
                ReturnFromBenchmark(tmp.HasValue() ? MakeArray<Tree<T>>(tmp.Get()) : Array<Tree<T>>());
            }
            else {
                Array<Tree<T>> ret = Array<Tree<T>>(2);
                Expected<Tree<T>> tmp = ChangeRoot(path.At(path.GetSize() / 2).name);
                if (!tmp.HasValue()) ReturnFromBenchmark(Array<Tree<T>>());
                ret.At(0) = tmp.Get();
                tmp = ChangeRoot(path.At((path.GetSize() - 1) / 2).name);
                if (!tmp.HasValue()) ReturnFromBenchmark(Array<Tree<T>>());
                ret.At(1) = tmp.Get();
                ReturnFromBenchmark(ret);
            }
        }
        [[nodiscard]] Tree<T> Foreach(const Function<Tree<T>, Tree<T>>& func) const {
            StartBenchmark
            Tree<T> ret = func(*this);
            for (Tree<T>& child : ret.children) child = child.Foreach(func);
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Expected<bool> StrictEquality(const Tree<T>& other) const {
            StartBenchmark
            if (data != other.data || children.GetSize() != other.children.GetSize()) ReturnFromBenchmark(Expected<bool>(false));
            const size_t size = children.GetSize();
            Bitmap used = Bitmap(size);
            for (size_t i = 0; i < size; i++) {
                bool foundMatch = false;
                for (size_t j = 0; j < size; j++) {
                    Expected<bool> tmp = used.Get(j);
                    if (!tmp.HasValue()) ReturnFromBenchmark(Expected<bool>());
                    if (!tmp.Get()) {
                        tmp = children.At(i).StrictEquality(other.children.At(j));
                        if (!tmp.HasValue()) ReturnFromBenchmark(Expected<bool>());
                        if (tmp.Get()) {
                            if (!used.Set(j, true)) ReturnFromBenchmark(Expected<bool>());
                            foundMatch = true;
                            break;
                        }
                    }
                }
                if (!foundMatch) ReturnFromBenchmark(Expected<bool>(false));
            }
            ReturnFromBenchmark(Expected<bool>(true));
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            StartBenchmark
            String ret = CollectionToString(padding) + name + " (" + MathLib::ToString(data) + ')';
            if (children.GetSize()) {
                ret += ": {\n";
                for (const Tree<T>& child : children) ret += child.ToString(CollectionToString(padding) + '\t') + '\n';
                ReturnFromBenchmark(ret + padding + '}');
            }
            else ReturnFromBenchmark(ret);
        }
        [[nodiscard]] bool operator==(const Tree<T>& other) const {
            StartBenchmark
            const Array<Tree<T>> centers1 = GetCenters();
            const Array<Tree<T>> centers2 = other.GetCenters();
            for (const Tree<T>& center1 : centers1) {
                for (const Tree<T>& center2 : centers2) {
                    const Expected<bool> tmp = center1.StrictEquality(center2);
                    if (!tmp.HasValue()) ReturnFromBenchmark(false);
                    if (tmp.Get()) ReturnFromBenchmark(true);
                }
            }
            ReturnFromBenchmark(false);
        }
        [[nodiscard]] bool operator!=(const Tree<T>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }

        private:
        [[nodiscard]] TreeElement* ToDoubleTree(void) const {
            StartBenchmark
            TreeElement* ret = new TreeElement(*this, nullptr);
            for (const Tree<T>& child : children) {
                if (!ret->AddChild(child.ToDoubleTree())) {
                    delete ret;
                    ReturnFromBenchmark(nullptr);
                }
            }
            ReturnFromBenchmark(ret);
        }
    };
}

#endif