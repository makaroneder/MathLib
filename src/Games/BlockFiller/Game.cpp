#include "Game.hpp"
#include <Grid.hpp>

Game::Game(void) {}
Game::Game(const MathLib::CSV& csv, size_t screenWidth, size_t screenHeight, size_t blockWidth, size_t blockHeight) : blocks(csv.GetMaxWidth(), csv.GetHeight()) {
    const size_t width = blocks.GetWidth();
    const size_t height = blocks.GetHeight();
    const MathLib::Matrix<MathLib::SingleTypePair<ssize_t>> grid = MathLib::GetGridCellsCenter(screenWidth / 2, screenHeight / 2, blockWidth, blockHeight, width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const MathLib::String typeStr = x < csv.GetWidth(y) ? csv.At(x, y) : "";
            const size_t size = typeStr.GetSize();
            const MathLib::SingleTypePair<ssize_t> position = grid.AtUnsafe(x, y);
            if (size && typeStr.AtUnsafe(0) == 'P') {
                playerX = x;
                playerY = y;
                blocks.AtUnsafe(x, y) = Block(Block::Type::Filled, position.first, position.second);
                continue;
            }
            if (size && (typeStr.AtUnsafe(0) == '+' || typeStr.AtUnsafe(0) == '-')) {
                blocks.AtUnsafe(x, y) = Block(typeStr.AtUnsafe(0) == '+' ? Block::Type::Key : Block::Type::Door, position.first, position.second);
                blocks.AtUnsafe(x, y).data = MathLib::StringToNumber(MathLib::SubString(typeStr, 1, size - 1));
                continue;
            }
            if (size && MathLib::IsDigit(typeStr.AtUnsafe(0))) {
                blocks.AtUnsafe(x, y) = Block(Block::Type::Teleport, position.first, position.second);
                blocks.AtUnsafe(x, y).data = MathLib::StringToNumber(typeStr);
                continue;
            }
            Block::Type type = Block::Type::None;
            if (size) {
                if (typeStr.AtUnsafe(0) == '_') type = Block::Type::Empty;
                if (typeStr.AtUnsafe(0) == '*') type = Block::Type::Coin;
                if (typeStr.AtUnsafe(0) == 'W') type = Block::Type::Wall;
                if (typeStr.AtUnsafe(0) == 'S') type = Block::Type::Stop;
                if (typeStr.AtUnsafe(0) == '<') type = Block::Type::Left;
                if (typeStr.AtUnsafe(0) == '>') type = Block::Type::Right;
                if (typeStr.AtUnsafe(0) == '^') type = Block::Type::Up;
                if (typeStr.AtUnsafe(0) == 'v') type = Block::Type::Down;
            }
            blocks.AtUnsafe(x, y) = Block(type, position.first, position.second);
        }
    }
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (blocks.AtUnsafe(x, y).type != Block::Type::Wall) continue;
            if (x && blocks.AtUnsafe(x - 1, y).type != Block::Type::Wall) blocks.AtUnsafe(x, y).data |= 1 << 0;
            if (x + 1 < width && blocks.AtUnsafe(x + 1, y).type != Block::Type::Wall) blocks.AtUnsafe(x, y).data |= 1 << 1;
            if (y && blocks.AtUnsafe(x, y - 1).type != Block::Type::Wall) blocks.AtUnsafe(x, y).data |= 1 << 2;
            if (y + 1 < height && blocks.AtUnsafe(x, y + 1).type != Block::Type::Wall) blocks.AtUnsafe(x, y).data |= 1 << 3;
        }
    }
}
void Game::Move(int8_t& dx, int8_t& dy) {
    if (!(dx || dy) || (ssize_t)playerX + dx < 0 || playerX + dx >= blocks.GetWidth() || (ssize_t)playerY + dy < 0 || playerY + dy >= blocks.GetHeight()) {
        dx = dy = 0;
        return;
    }
    const Block::Type type = blocks.AtUnsafe(playerX + dx, playerY + dy).type;
    switch (type) {
        case Block::Type::None:
        case Block::Type::Empty:
        case Block::Type::Filled: {
            playerX += dx;
            playerY += dy;
            break;
        }
        case Block::Type::Wall: {
            dx = dy = 0;
            break;
        }
        case Block::Type::Stop: {
            playerX += dx;
            playerY += dy;
            dx = dy = 0;
            break;
        }
        case Block::Type::Coin:
        case Block::Type::Key: {
            playerX += dx;
            playerY += dy;
            blocks.AtUnsafe(playerX, playerY).type = Block::Type::Filled;
            blocks.AtUnsafe(playerX, playerY).initAnimation |= type == Block::Type::Coin && blocks.AtUnsafe(playerX, playerY).animation.IsEmpty();
            if (type == Block::Type::Key) (void)keys.Add(blocks.AtUnsafe(playerX, playerY).data);
            break;
        }
        case Block::Type::Door: {
            const size_t key = blocks.AtUnsafe(playerX + dx, playerY + dy).data;
            if (key != SIZE_MAX && !keys.Contains(key)) {
                dx = dy = 0;
                break;
            }
            playerX += dx;
            playerY += dy;
            blocks.AtUnsafe(playerX, playerY).initAnimation |= blocks.AtUnsafe(playerX, playerY).data != SIZE_MAX;
            blocks.AtUnsafe(playerX, playerY).data = SIZE_MAX;
            break;
        }
        case Block::Type::Teleport: {
            playerX += dx;
            playerY += dy;
            const size_t teleport = blocks.AtUnsafe(playerX, playerY).data;
            const size_t width = blocks.GetWidth();
            const size_t height = blocks.GetHeight();
            blocks.AtUnsafe(playerX, playerY).initAnimation = true;
            bool found = false;
            for (size_t y = 0; y < height; y++) {
                for (size_t x = 0; x < width; x++) {
                    if ((x == playerX && y == playerY) || blocks.AtUnsafe(x, y).type != Block::Type::Teleport || blocks.AtUnsafe(x, y).data != teleport) continue;
                    playerX = x;
                    playerY = y;
                    found = true;
                    break;
                }
                if (found) break;
            }
            blocks.AtUnsafe(playerX, playerY).initAnimation = true;
            break;
        }
        case Block::Type::Left:
        case Block::Type::Right:
        case Block::Type::Up:
        case Block::Type::Down: {
            playerX += dx;
            playerY += dy;
            blocks.AtUnsafe(playerX, playerY).initAnimation |= blocks.AtUnsafe(playerX, playerY).animation.IsEmpty();
            switch (type) {
                case Block::Type::Left: {
                    dx = -1;
                    dy = 0;
                    break;
                }
                case Block::Type::Right: {
                    dx = 1;
                    dy = 0;
                    break;
                }
                case Block::Type::Up: {
                    dx = 0;
                    dy = -1;
                    break;
                }
                case Block::Type::Down: {
                    dx = 0;
                    dy = 1;
                    break;
                }
                default: break;
            }
            break;
        }
        default: break;
    }
}