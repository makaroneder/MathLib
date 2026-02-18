#include <Libc/HostFileSystem.hpp>
#include <SingleTypePair.hpp>
#include <Math/Matrix.hpp>
#include <Bitmap.hpp>
#include <CSV.hpp>
#include <iostream>

struct Block;
struct Block : MathLib::Comparable<Block>, MathLib::Printable {
    MathLib::Bitmap possibleValues;

    Block(void) {}
    Block(size_t size) : possibleValues(size) {
        possibleValues.Fill(true);
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        MathLib::String ret = MathLib::CollectionToString(padding) + '{';
        bool prev = false;
        for (size_t i = 0; i < possibleValues.GetSize(); i++) {
            if (!possibleValues.AtUnsafe(i)) continue;
            if (prev) ret += ", ";
            ret += MathLib::ToString(i + 1, 10);
            prev = true;
        }
        return ret + '}';
    }
    bool IsPossible(size_t value) const {
        return possibleValues.At(value - 1);
    }
    bool Set(size_t value) {
        possibleValues.Fill(false);
        return possibleValues.Set(value - 1, true);
    }
    size_t GetMinSet(size_t start) const {
        const size_t size = possibleValues.GetSize();
        for (size_t i = start; i < size; i++)
            if (possibleValues.AtUnsafe(i)) return i + 1;
        return size + 1;
    }
    size_t GetPossibleValueCount(void) const {
        const size_t size = possibleValues.GetSize();
        size_t ret = 0;
        for (size_t i = 0; i < size; i++) ret += possibleValues.AtUnsafe(i);
        return ret;
    }
    Block operator-(const Block& block) const {
        Block ret = *this;
        for (size_t j = 0; j < possibleValues.GetSize(); j++)
            if (block.possibleValues.AtUnsafe(j)) (void)ret.possibleValues.SetUnsafe(j, false);
        return ret;
    }
    Block& operator-=(const Block& block) {
        for (size_t j = 0; j < possibleValues.GetSize(); j++)
            if (block.possibleValues.AtUnsafe(j)) (void)possibleValues.SetUnsafe(j, false);
        return *this;
    }

    protected:
    [[nodiscard]] virtual bool Equals(const Block& other) const override {
        return possibleValues == other.possibleValues;
    }
};
namespace MathLib {
    MakeFormatter(Block, self, padding, { return self.ToString(padding); });
}
MathLib::String SudokuToCSVString(const MathLib::Matrix<Block>& sudoku) {
    const size_t width = sudoku.GetWidth();
    const size_t height = sudoku.GetHeight();
    MathLib::String ret;
    for (size_t j = 0; j < height; j++) {
        if (j) ret += '\n';
        for (size_t i = 0; i < width; i++) {
            if (i) ret += ',';
            const Block block = sudoku.AtUnsafe(i, j);
            ret += block.GetPossibleValueCount() == 1 ? MathLib::ToString(block.GetMinSet(0), 10) : "0";
        }
    }
    return ret;
}
void RestrictInternal(MathLib::Matrix<Block>& sudoku, size_t i, size_t j, MathLib::Array<size_t>& cache) {
    if (cache.Contains(MathLib::SingleTypePair<size_t>(i, j))) return;
    (void)cache.Add(i);
    (void)cache.Add(j);
    const size_t width = sudoku.GetWidth();
    const size_t height = sudoku.GetHeight();
    const size_t boxX = MathLib::Sqrt(width);
    const size_t boxY = MathLib::Sqrt(height);
    const Block block = sudoku.AtUnsafe(i, j);
    for (size_t y = 0; y < height; y++) {
        if (y == j) continue;
        sudoku.AtUnsafe(i, y) -= block;
        if (sudoku.AtUnsafe(i, y).GetPossibleValueCount() == 1) RestrictInternal(sudoku, i, y, cache);
    }
    for (size_t x = 0; x < width; x++) {
        if (x == i) continue;
        sudoku.AtUnsafe(x, j) -= block;
        if (sudoku.AtUnsafe(x, j).GetPossibleValueCount() == 1) RestrictInternal(sudoku, x, j, cache);
    }
    const size_t bx = i / boxX;
    const size_t by = j / boxY;
    for (size_t y = 0; y < boxY; y++) {
        const size_t iy = boxY * by + y;
        if (iy == j) continue;
        for (size_t x = 0; x < boxX; x++) {
            const size_t ix = boxX * bx + x;
            if (ix == i) continue;
            sudoku.AtUnsafe(ix, iy) -= block;
            if (sudoku.AtUnsafe(ix, iy).GetPossibleValueCount() == 1) RestrictInternal(sudoku, ix, iy, cache);
        }
    }
}
void Restrict(MathLib::Matrix<Block>& sudoku, size_t i, size_t j) {
    MathLib::Array<size_t> cache;
    RestrictInternal(sudoku, i, j, cache);
}
MathLib::Matrix<Block> GetPossibleValues(const MathLib::Matrix<size_t>& sudoku) {
    const size_t width = sudoku.GetWidth();
    const size_t height = sudoku.GetHeight();
    const size_t size = MathLib::Ceil(MathLib::Sqrt(width * height));
    MathLib::Matrix<Block> ret = MathLib::Matrix<Block>(width, height);
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            ret.AtUnsafe(i, j) = Block(size);
            const size_t tmp = sudoku.AtUnsafe(i, j);
            if (tmp) ret.AtUnsafe(i, j).Set(tmp);
        }
    }
    for (size_t j = 0; j < height; j++)
        for (size_t i = 0; i < width; i++)
            if (ret.AtUnsafe(i, j).GetPossibleValueCount() == 1) Restrict(ret, i, j);
    return ret;
}
void SetForcedOptions(MathLib::Matrix<Block>& sudoku) {
    const size_t width = sudoku.GetWidth();
    const size_t height = sudoku.GetHeight();
    const size_t boxX = MathLib::Sqrt(width);
    const size_t boxY = MathLib::Sqrt(height);
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            Block block = sudoku.AtUnsafe(i, j);
            if (block.GetPossibleValueCount() == 1) continue;
            for (size_t y = 0; y < height; y++) {
                if (y == j) continue;
                const Block tmp = sudoku.AtUnsafe(i, y);
                if (tmp.GetPossibleValueCount() == 1) block -= tmp;
            }
            if (block.GetPossibleValueCount() == 1) {
                sudoku.AtUnsafe(i, j) = block;
                Restrict(sudoku, i, j);
                SetForcedOptions(sudoku);
                return;
            }
            block = sudoku.AtUnsafe(i, j);
            for (size_t x = 0; x < width; x++) {
                if (x == i) continue;
                const Block tmp = sudoku.AtUnsafe(x, j);
                if (tmp.GetPossibleValueCount() == 1) block -= tmp;
            }
            if (block.GetPossibleValueCount() == 1) {
                sudoku.AtUnsafe(i, j) = block;
                Restrict(sudoku, i, j);
                SetForcedOptions(sudoku);
                return;
            }
            block = sudoku.AtUnsafe(i, j);
            const size_t bx = i / boxX;
            const size_t by = j / boxY;
            for (size_t y = 0; y < boxY; y++) {
                const size_t iy = boxY * by + y;
                if (iy == j) continue;
                for (size_t x = 0; x < boxX; x++) {
                    const size_t ix = boxX * bx + x;
                    if (ix == i) continue;
                    const Block tmp = sudoku.AtUnsafe(ix, iy);
                    if (tmp.GetPossibleValueCount() == 1) block -= tmp;
                }
            }
            if (block.GetPossibleValueCount() == 1) {
                sudoku.AtUnsafe(i, j) = block;
                Restrict(sudoku, i, j);
                SetForcedOptions(sudoku);
                return;
            }
        }
    }
}
bool IsValid(const MathLib::Matrix<Block>& sudoku) {
    for (const Block& block : sudoku)
        if (!block.GetPossibleValueCount()) return false;
    return true;
}
bool BruteforceInternal(MathLib::Matrix<Block>& sudoku, size_t i, size_t j, bool check) {
    if (check) {
        SetForcedOptions(sudoku);
        if (!IsValid(sudoku)) return false;
    }
    if (j == sudoku.GetHeight()) return true;
    const Block block = sudoku.AtUnsafe(i, j);
    size_t nx = i + 1;
    size_t ny = j;
    if (nx == sudoku.GetWidth()) {
        nx = 0;
        ny++;
    }
    const size_t options = block.GetPossibleValueCount();
    const bool newCheck = options != 1;
    size_t prev = 0;
    for (size_t x = 0; x < options; x++) {
        prev = block.GetMinSet(prev);
        MathLib::Matrix<Block> newSudoku = sudoku;
        newSudoku.AtUnsafe(i, j).Set(prev);
        if (newCheck) Restrict(newSudoku, i, j);
        if (BruteforceInternal(newSudoku, nx, ny, newCheck)) {
            sudoku = newSudoku;
            return true;
        }
    }
    return false;
}
bool Bruteforce(MathLib::Matrix<Block>& sudoku) {
    return BruteforceInternal(sudoku, 0, 0, true);
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::HostFileSystem fs;
        const MathLib::CSV csv = MathLib::CSV(fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'));
        const size_t height = csv.GetHeight();
        const size_t maxWidth = csv.GetMaxWidth();
        MathLib::Matrix<size_t> sudoku = MathLib::Matrix<size_t>(maxWidth, height);
        for (size_t y = 0; y < height; y++) {
            const size_t width = csv.GetWidth(y);
            for (size_t x = 0; x < width; x++) sudoku.AtUnsafe(x, y) = MathLib::StringToNumber(csv.At(x, y));
            for (size_t x = width; x < maxWidth; x++) sudoku.AtUnsafe(x, y) = 0;
        }
        MathLib::Matrix<Block> parsedSudoku = GetPossibleValues(sudoku);
        const MathLib::num_t time = MathLib::GetTime();
        Bruteforce(parsedSudoku);
        std::cout << MathLib::GetTime() - time << std::endl;
        std::cout << SudokuToCSVString(parsedSudoku) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}