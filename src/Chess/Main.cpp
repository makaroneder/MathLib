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
const size_t width = 384;
template <typename T>
T ToCoordinate(MathLib::Renderer& renderer, size_t x) {
    return ((T)x * 2 - 7) * (T)width / (16 * renderer.pointMultiplier);
}
template <typename T>
MathLib::Matrix<T> ToPoint(MathLib::Renderer& renderer, size_t x, size_t y) {
    return MathLib::CreateVector<T>(ToCoordinate<T>(renderer, x), ToCoordinate<T>(renderer, y), 0);
}
void DrawBoard(MathLib::Renderer& renderer, const MathLib::Video& pieces, const MathLib::Matrix<uint8_t>& board) {
    for (size_t y = 0; y < 8; y++) {
        for (size_t x = 0; x < 8; x++) {
            const uint8_t piece = board.At(x, y);
            if (piece != (uint8_t)Piece::None)
                renderer.DrawImage<MathLib::num_t>(pieces.At(piece), ToPoint<MathLib::num_t>(renderer, x, y));
        }
    }
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Chess", width, width);
        MathLib::HostFileSystem fs;
        MathLib::Aseprite boardImage;
        if (!boardImage.LoadFromPath(fs, "src/TestPrograms/Chess/Board2.aseprite"_M)) MathLib::Panic("Failed to load board");
        MathLib::Aseprite pieces;
        if (!pieces.LoadFromPath(fs, "src/TestPrograms/Chess/Piece.aseprite"_M)) MathLib::Panic("Failed to load pieces");
        renderer.Fill(0);
        renderer.DrawImage<MathLib::num_t>(boardImage.At(0), MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        MathLib::Matrix<uint8_t> board = MathLib::Matrix<uint8_t>(8, 8);
        board.Fill((uint8_t)Piece::None);
        for (uint8_t i = 0; i < 2; i++) {
            for (uint8_t j = 0; j < 2; j++) {
                board.At(j ? 0 : (7 - 0), 7 * !i) = (uint8_t)Piece::BlackTower - (uint8_t)Piece::BlackOffset * i;
                board.At(j ? 1 : (7 - 1), 7 * !i) = (uint8_t)Piece::BlackHorse - (uint8_t)Piece::BlackOffset * i;
                board.At(j ? 2 : (7 - 2), 7 * !i) = (uint8_t)Piece::BlackBishop - (uint8_t)Piece::BlackOffset * i;
            }
            for (uint8_t j = 0; j < 8; j++)
                board.At(j,  1 + 5 * !i) = (uint8_t)Piece::BlackPawn - (uint8_t)Piece::BlackOffset * i;
            board.At(3, 7 * !i) = (uint8_t)Piece::BlackQueen - (uint8_t)Piece::BlackOffset * i;
            board.At(4, 7 * !i) = (uint8_t)Piece::BlackKing - (uint8_t)Piece::BlackOffset * i;
        }

        DrawBoard(renderer, pieces, board);
        while (true) {
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