#define SDL_MAIN_HANDLED
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <SDL2.cpp>
#include <iostream>

enum class Piece : uint8_t {
    WhitePawn = 0,
    WhiteHorse,
    WhiteTower,
    WhiteBishop,
    WhiteKing,
    WhiteQueen,
    BlackOffset,
    BlackPawn = BlackOffset,
    BlackHorse,
    BlackTower,
    BlackBishop,
    BlackKing,
    BlackQueen,
    None,
};
size_t normalizedWidth;
size_t normalizedHeight;
void DrawBoard(MathLib::Renderer& renderer, const MathLib::Video& pieces, const MathLib::Matrix<uint8_t>& board) {
    const size_t halfOfNormalizedWidth = normalizedWidth / 2;
    const size_t halfOfNormalizedHeight = normalizedHeight / 2;
    for (size_t y = 0; y < 8; y++) {
        for (size_t x = 0; x < 8; x++) {
            const uint8_t piece = board.At(x, y);
            if (piece != (uint8_t)Piece::None)
                renderer.DrawImage(pieces.At(piece), x * normalizedWidth + halfOfNormalizedWidth, y * normalizedHeight + halfOfNormalizedHeight);
        }
    }
}
struct Move;
struct Move : MathLib::Comparable<Move>, MathLib::Printable {
    uint8_t startX;
    uint8_t startY;
    uint8_t endX;
    uint8_t endY;

    Move(void) : startX(UINT8_MAX), startY(UINT8_MAX), endX(UINT8_MAX), endY(UINT8_MAX) {}
    Move(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY) : startX(startX), startY(startY), endX(endX), endY(endY) {}
    [[nodiscard]] bool IsValid(void) const {
        return startX != UINT8_MAX && startY != UINT8_MAX && endX != UINT8_MAX && endY != UINT8_MAX;
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        return MathLib::CollectionToString(padding) + PairToString(startX, startY) + " -> " + PairToString(endX, endY);
    }

    protected:
    [[nodiscard]] virtual bool Equals(const Move& other) const override {
        return startX == other.startX && startY == other.startY && endX == other.endX && endY == other.endY;
    }

    private:
    [[nodiscard]] static MathLib::String PairToString(uint8_t x, uint8_t y) {
        return '('_M + MathLib::Formatter<uint8_t>::ToString(x) + ", " + MathLib::Formatter<uint8_t>::ToString(y) + ')';
    }
};
MathLib::Array<Move> ParsePGN(MathLib::Readable& file, const MathLib::Matrix<uint8_t>& startBoard) {
    while (true) {
        char chr;
        if (!file.Read<char>(chr)) return MathLib::Array<Move>();
        if (chr == '\n') break;
        if (chr != '[') return MathLib::Array<Move>();
        if (file.ReadUntil(']').IsEmpty()) return MathLib::Array<Move>();
        if (!file.Read<char>(chr)) return MathLib::Array<Move>();
        if (chr != '\n') return MathLib::Array<Move>();
    }
    MathLib::Matrix<uint8_t> board = startBoard;
    const MathLib::String moves = file.ReadUntil('\0');
    size_t i = 0;
    while (true) {
        while (MathLib::IsDigit(moves.At(i))) i++;
        if (moves.At(i) == '-' || moves.At(i) == '/') break;
        if (moves.At(i++) != '.') return MathLib::Array<Move>();
        MathLib::SkipWhiteSpace(moves, i);
        MathLib::Array<MathLib::String> move;
        while (!MathLib::IsDigit(moves.At(i))) {
            MathLib::String str;
            while (moves.At(i) == '-' || moves.At(i) == '=' || MathLib::IsAlphaDigit(moves.At(i))) str += moves.At(i++);
            if (moves.At(i) == '+' || moves.At(i) == '#') i++;
            MathLib::SkipWhiteSpace(moves, i);
            if (moves.At(i) == '$') {
                i++;
                while (MathLib::IsDigit(moves.At(i))) i++;
                MathLib::SkipWhiteSpace(moves, i);
            }
            (void)move.Add(str);
        }
        bool white = true;
        for (const MathLib::String& str : move) {
            const size_t size = str.GetSize();
            if (str.At(0) == 'O') std::cout << str << std::endl;
            else {
                char type = 'P';
                size_t next = 1;
                switch (str.At(0)) {
                    case 'K':
                    case 'Q':
                    case 'R':
                    case 'B':
                    case 'N':
                    case 'P': {
                        type = str.At(0);
                        break;
                    }
                    default: next = 0;
                }
                size_t x = SIZE_MAX;
                size_t y = SIZE_MAX;
                bool capture = false;
                if (next < size - 2) {
                    for (size_t i = next; i < size - 2; i++) {
                        if (MathLib::IsDigit(str.At(i))) y = (size_t)'8' - str.At(i);
                        if (IsBetween(str.At(i), 'a', 'h')) x = (size_t)str.At(i) - 'a';
                        if (str.At(i) == 'x') capture = true;
                    }
                }
                const size_t endX = (size_t)str.At(size - 2) - 'a';
                const size_t endY = (size_t)'8' - str.At(size - 1);
                switch (type) {
                    case 'P': {
                        if (capture) {
                            if (y == SIZE_MAX) {
                                if (white) y = endY - 1;
                                else y = endY + 1;
                            }
                            if (x == SIZE_MAX) {
                                const Piece piece = white ? Piece::WhitePawn : Piece::BlackPawn;
                                if (endX && board.At(endX - 1, y) == (uint8_t)piece) x = endX - 1;
                                if (endX != 7 && board.At(endX + 1, y) == (uint8_t)piece) x = endX + 1;
                            }
                        }
                        else {
                            if (x == SIZE_MAX) x = endX;
                            if (y == SIZE_MAX) {
                                if (white) {
                                    if (endY == 4 || endY == 5) y = 6;
                                    else y = endY + 1;
                                }
                                else {
                                    if (endY == 2 || endY == 3) y = 1;
                                    else y = endY - 1;
                                }
                            }
                        }
                        break;
                    }
                    case 'K': {
                        const Piece piece = white ? Piece::WhiteKing : Piece::BlackKing;
                        if (x == SIZE_MAX || y == SIZE_MAX) {
                            for (int8_t dy = -1; dy <= 1; dy++) {
                                if (dy == -1 && !endY) continue;
                                if (dy == 1 && endY == 7) continue;
                                for (int8_t dx = -1; dx <= 1; dx++) {
                                    if (dx == -1 && !endX) continue;
                                    if (dx == 1 && endX == 7) continue;
                                    if (!dx && !dy) continue;
                                    if (board.At(endX + dx, endY + dy) == (uint8_t)piece) {
                                        if (x == SIZE_MAX) x = endX + dx;
                                        if (y == SIZE_MAX) y = endY + dy;
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case 'Q': break;
                    case 'R': {
                        const Piece piece = white ? Piece::WhiteTower : Piece::BlackTower;
                        for (uint8_t i = 0; i < 8; i++) {
                            if (board.At(i, endY) == (uint8_t)piece) {
                                if (x == SIZE_MAX) x = i;
                                break;
                            }
                            if (board.At(endX, i) == (uint8_t)piece) {
                                if (y == SIZE_MAX) y = i;
                                break;
                            }
                        }
                        break;
                    }
                    case 'B': break;
                    case 'N': break;
                }
                const Move move = Move(x, y, endX, endY);
                if (move.IsValid()) {
                    board.At(move.endX, move.endY) = board.At(move.startX, move.startY);
                    board.At(move.startX, move.startY) = (uint8_t)Piece::None;
                }
                std::cout << type << ' ' << move;
                if (capture) std::cout << " (capture)";
                std::cout << std::endl;
            }
            white = !white;
        }
    }
    return MathLib::Array<Move>();
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::HostFileSystem fs;
        MathLib::Aseprite boardImage;
        if (!boardImage.LoadFromPath(fs, "src/TestPrograms/Chess/Board.aseprite"_M)) MathLib::Panic("Failed to load board");
        normalizedWidth = boardImage.GetWidth() / 8;
        normalizedHeight = boardImage.GetHeight() / 8;
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Chess", boardImage.GetWidth(), boardImage.GetHeight());
        MathLib::Aseprite pieces;
        if (!pieces.LoadFromPath(fs, "src/TestPrograms/Chess/Piece.aseprite"_M)) MathLib::Panic("Failed to load pieces");
        MathLib::Matrix<uint8_t> board = MathLib::Matrix<uint8_t>(8, 8);
        board.Fill((uint8_t)Piece::None);
        for (uint8_t i = 0; i < 2; i++) {
            for (uint8_t j = 0; j < 2; j++) {
                board.At(j ? 0 : (7 - 0), 7 * i) = (uint8_t)Piece::BlackTower - (uint8_t)Piece::BlackOffset * i;
                board.At(j ? 1 : (7 - 1), 7 * i) = (uint8_t)Piece::BlackHorse - (uint8_t)Piece::BlackOffset * i;
                board.At(j ? 2 : (7 - 2), 7 * i) = (uint8_t)Piece::BlackBishop - (uint8_t)Piece::BlackOffset * i;
            }
            for (uint8_t j = 0; j < 8; j++)
                board.At(j,  1 + 5 * i) = (uint8_t)Piece::BlackPawn - (uint8_t)Piece::BlackOffset * i;
            board.At(3, 7 * i) = (uint8_t)Piece::BlackQueen - (uint8_t)Piece::BlackOffset * i;
            board.At(4, 7 * i) = (uint8_t)Piece::BlackKing - (uint8_t)Piece::BlackOffset * i;
        }
        do {
            MathLib::File file = fs.Open("src/TestPrograms/Chess/1.pgn"_M, MathLib::OpenMode::Read);
            (void)ParsePGN(file, board);
            return 0;
        } while (false);
        #ifdef Debug
        MathLib::num_t prev = MathLib::GetTime();
        #endif
        while (true) {
            #ifdef Debug
            const MathLib::num_t curr = MathLib::GetTime();
            std::cout << 1 / (curr - prev) << std::endl;
            prev = curr;
            #endif
            if (!renderer.CopyFromBuffer(boardImage.At(0))) MathLib::Panic("Failed to draw background image");
            DrawBoard(renderer, pieces, board);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            if (renderer.GetEvent().type == MathLib::Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}