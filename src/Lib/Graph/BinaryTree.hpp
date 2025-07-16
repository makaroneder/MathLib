#ifndef MathLib_Graph_BinaryTree_H
#define MathLib_Graph_BinaryTree_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    template <typename T>
    struct BinaryTree : Printable {
        BinaryTree(void) : data(T()), left(nullptr), right(nullptr) {
            EmptyBenchmark
        }
        BinaryTree(const T& data) : data(data), left(nullptr), right(nullptr) {
            EmptyBenchmark
        }
        ~BinaryTree(void) {
            StartBenchmark
            if (left) delete left;
            if (right) delete right;
            EndBenchmark
        }
        [[nodiscard]] bool IsEmpty(void) const {
            StartAndReturnFromBenchmark(!left && !right);
        }
        [[nodiscard]] BinaryTree<T>* Recreate(void) const {
            StartBenchmark
            BinaryTree<T>* ret = new BinaryTree<T>(data);
            if (!ret) ReturnFromBenchmark(nullptr);
            if (left) ret->left = left->Recreate();
            if (right) ret->right = right->Recreate();
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] T GetData(void) const {
            StartAndReturnFromBenchmark(data);
        }
        [[nodiscard]] T& GetData(void) {
            StartAndReturnFromBenchmark(data);
        }
        [[nodiscard]] BinaryTree<T>* Get(bool left_) {
            StartAndReturnFromBenchmark(left_ ? left : right);
        }
        void Set(BinaryTree<T>* child, bool left_) {
            StartBenchmark
            BinaryTree<T>*& target = left_ ? left : right;
            if (target) delete target;
            target = child;
            EndBenchmark
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            StartAndReturnFromBenchmark("{\n"_M +
                padding + "\tData: " + MathLib::ToString(data) + '\n' +
                (left ? ('\t'_M + padding + "Left: "_M + left->ToString('\t'_M + padding) + '\n') : "") +
                (right ? ('\t'_M + padding + "Right: "_M + right->ToString('\t'_M + padding) + '\n') : "") +
            padding + '}');
        }

        private:
        T data;
        BinaryTree<T>* left;
        BinaryTree<T>* right;
    };
}

#endif