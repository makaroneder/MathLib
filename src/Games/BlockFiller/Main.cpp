#define SDL_MAIN_HANDLED
#include "Game.hpp"
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Collision.hpp>
#include <JSON.hpp>
#include <SDL2.cpp>
#include <iostream>

const MathLib::num_t animationSpeed = 1;
void DrawAnimation(MathLib::Renderer& renderer, Block& block, const MathLib::Video* animation, MathLib::num_t deltaTime, size_t defaultFrame) {
    if (block.initAnimation) {
        block.animation = MathLib::Animation(animation);
        block.initAnimation = false;
    }
    if (!block.animation.IsEmpty()) {
        block.animation.Draw(renderer, block.x, block.y);
        if (block.animation.Update(deltaTime * animationSpeed)) block.animation = MathLib::Animation();
    }
    else if (defaultFrame != SIZE_MAX) renderer.DrawImage(animation->AtUnsafe(defaultFrame), block.x, block.y);
}
int main(int, char**) {
    try {
        const size_t width = 800;
        const size_t height = 800;
        const size_t blockWidth = 50;
        const size_t blockHeight = 50;
        const MathLib::String path = "src/TestPrograms/BlockFiller/";
        MathLib::HostFileSystem fs;
        MathLib::JSON config;
        if (!config.LoadFromPath(fs, path + "Config.json")) MathLib::Panic("Failed to load configuration file");

        const MathLib::JSON levelsJSON = config.GetChild("levels"_M).Get("No levels provided");
        MathLib::Array<MathLib::String> levels;
        for (const MathLib::JSON& level : levelsJSON)
            (void)levels.Add(level.GetValue());
        const MathLib::Expected<MathLib::JSON> unlockJSON = config.GetChild("unlock"_M);
        const bool unlock = unlockJSON.HasValue() && unlockJSON.Get().GetValue() == "true";

        MathLib::Aseprite leftUIArrow;
        if (!leftUIArrow.LoadFromPath(fs, path + "Arrow.aseprite")) MathLib::Panic("Failed to load UI arrow sprite");
        const MathLib::Video rightUIArrow = leftUIArrow.Mirror();
        MathLib::Aseprite lockedLeftUIArrow;
        if (!lockedLeftUIArrow.LoadFromPath(fs, path + "LockedArrow.aseprite")) MathLib::Panic("Failed to load locked UI arrow sprite");
        const MathLib::Video lockedRightUIArrow = lockedLeftUIArrow.Mirror();
        const size_t leftUIArrowX = leftUIArrow.GetWidth() * 3 / 4;
        const size_t leftUIArrowY = height / 2;
        const size_t rightUIArrowX = width - rightUIArrow.GetWidth() * 3 / 4;
        const size_t rightUIArrowY = height / 2;

        MathLib::Aseprite backgroundVideo;
        if (!backgroundVideo.LoadFromPath(fs, path + "Background.aseprite")) MathLib::Panic("Failed to load background sprite");
        MathLib::Animation background = &backgroundVideo;

        MathLib::Aseprite tile;
        if (!tile.LoadFromPath(fs, path + "Tile.aseprite")) MathLib::Panic("Failed to load tile sprite");
        MathLib::Aseprite plate;
        if (!plate.LoadFromPath(fs, path + "Plate.aseprite")) MathLib::Panic("Failed to load plate sprite");
        MathLib::Aseprite wall;
        if (!wall.LoadFromPath(fs, path + "Wall.aseprite")) MathLib::Panic("Failed to load wall sprite");
        MathLib::Aseprite stop;
        if (!stop.LoadFromPath(fs, path + "Stop.aseprite")) MathLib::Panic("Failed to load stop sprite");
        MathLib::Aseprite key;
        if (!key.LoadFromPath(fs, path + "Key.aseprite")) MathLib::Panic("Failed to load key sprite");
        MathLib::Aseprite door;
        if (!door.LoadFromPath(fs, path + "Door.aseprite")) MathLib::Panic("Failed to load door sprite");

        MathLib::Aseprite upArrow;
        if (!upArrow.LoadFromPath(fs, path + "UpArrow.aseprite")) MathLib::Panic("Failed to load up arrow sprite");
        MathLib::Aseprite downArrow;
        if (!downArrow.LoadFromPath(fs, path + "DownArrow.aseprite")) MathLib::Panic("Failed to load down arrow sprite");
        MathLib::Aseprite leftArrow;
        if (!leftArrow.LoadFromPath(fs, path + "LeftArrow.aseprite")) MathLib::Panic("Failed to load left arrow sprite");
        MathLib::Aseprite rightArrow;
        if (!rightArrow.LoadFromPath(fs, path + "RightArrow.aseprite")) MathLib::Panic("Failed to load right arrow sprite");

        MathLib::Aseprite teleport;
        if (!teleport.LoadFromPath(fs, path + "Teleport.aseprite")) MathLib::Panic("Failed to load teleport sprite");
        const MathLib::Video idleTeleport = teleport.Sub(MathLib::Interval<size_t>(0, 4));
        const MathLib::Video workingTeleport = teleport.Sub(MathLib::Interval<size_t>(4, teleport.GetSize()));

        MathLib::Aseprite ball;
        if (!ball.LoadFromPath(fs, path + "Ball.aseprite")) MathLib::Panic("Failed to load ball sprite");
        const MathLib::num_t moveLag = 0.01;
        MathLib::num_t moveTime = 0;

        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Block filler", width, height);
        MathLib::num_t prevTime = MathLib::GetTime();
        int8_t dx = 0;
        int8_t dy = 0;
        Game game;
        bool loadLevel = true;
        size_t currentLevel = 0;
        size_t lastLevel = unlock ? SIZE_MAX : currentLevel;
        while (true) {
            const MathLib::num_t time = MathLib::GetTime();
            const MathLib::num_t deltaTime = time - prevTime;
            prevTime = time;
            std::cout << "FPS: " << 1 / deltaTime << std::endl;
            if (loadLevel && currentLevel < levels.GetSize()) {
                if (lastLevel < currentLevel) lastLevel = currentLevel;
                game = Game(MathLib::CSV(fs.Open(path + levels.AtUnsafe(currentLevel++), MathLib::OpenMode::Read).ReadUntil('\0')), width, height, blockWidth, blockHeight);
                dx = dy = 0;
                moveTime = 0;
                loadLevel = false;
            }
            background.Update(deltaTime * animationSpeed);
            if (!background.CopyTo(renderer)) MathLib::Panic("Failed to draw background");
            loadLevel = true;
            for (Block& block : game.blocks) {
                if (block.type != Block::Type::None && block.type != Block::Type::Wall) renderer.DrawImage(tile.AtUnsafe(0), block.x, block.y);
                switch (block.type) {
                    case Block::Type::Coin: {
                        renderer.DrawImage(plate.AtUnsafe(0), block.x, block.y);
                        loadLevel = false;
                        break;
                    }
                    case Block::Type::Filled: {
                        DrawAnimation(renderer, block, &plate, deltaTime, SIZE_MAX);
                        break;
                    }
                    case Block::Type::Wall: {
                        for (uint8_t i = 0; i < 4; i++)
                            if (block.data & (1 << i)) renderer.DrawImage(wall.AtUnsafe(i), block.x, block.y);
                        break;
                    }
                    case Block::Type::Stop: {
                        block.initAnimation = block.animation.IsEmpty();
                        DrawAnimation(renderer, block, &stop, deltaTime, SIZE_MAX);
                        break;
                    }
                    case Block::Type::Key: {
                        renderer.DrawImage(key.AtUnsafe(0), block.x, block.y);
                        break;
                    }
                    case Block::Type::Door: {
                        DrawAnimation(renderer, block, &door, deltaTime, block.data == SIZE_MAX ? door.GetSize() - 1 : 0);
                        break;
                    }
                    case Block::Type::Teleport: {
                        if (block.initAnimation) block.animation = MathLib::Animation(&workingTeleport);
                        block.initAnimation = block.animation.IsEmpty();
                        DrawAnimation(renderer, block, &idleTeleport, deltaTime, SIZE_MAX);
                        break;
                    }
                    case Block::Type::Up: {
                        DrawAnimation(renderer, block, &upArrow, deltaTime, 0);
                        break;
                    }
                    case Block::Type::Down: {
                        DrawAnimation(renderer, block, &downArrow, deltaTime, 0);
                        break;
                    }
                    case Block::Type::Left: {
                        DrawAnimation(renderer, block, &leftArrow, deltaTime, 0);
                        break;
                    }
                    case Block::Type::Right: {
                        DrawAnimation(renderer, block, &rightArrow, deltaTime, 0);
                        break;
                    }
                    default: break;
                }
            }
            moveTime -= deltaTime;
            if (!dx && !dy) moveTime = 0;
            if (moveTime <= 0) {
                game.Move(dx, dy);
                moveTime = moveLag;
            }
            const Block player = game.blocks.AtUnsafe(game.playerX, game.playerY);
            renderer.DrawImage(ball.AtUnsafe(0), player.x, player.y);
            if (currentLevel > 1) renderer.DrawImage(leftUIArrow.AtUnsafe(0), leftUIArrow.AtUnsafe(0).GetWidth() * 3 / 4, height / 2);
            if (currentLevel < levels.GetSize()) {
                const MathLib::Video* const tmp = lastLevel < currentLevel ? &lockedRightUIArrow : &rightUIArrow;
                renderer.DrawImage(tmp->AtUnsafe(0), width - tmp->AtUnsafe(0).GetWidth() * 3 / 4, height / 2);
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            if (!event.pressed) continue;
            if (event.type == MathLib::Event::Type::KeyPressed) switch (event.key) {
                case 'w': {
                    if (dx || dy) break;
                    dx = 0;
                    dy = -1;
                    break;
                }
                case 's': {
                    if (dx || dy) break;
                    dx = 0;
                    dy = 1;
                    break;
                }
                case 'a': {
                    if (dx || dy) break;
                    dx = -1;
                    dy = 0;
                    break;
                }
                case 'd': {
                    if (dx || dy) break;
                    dx = 1;
                    dy = 0;
                    break;
                }
                case 'r': {
                    --currentLevel;
                    loadLevel = true;
                    break;
                }
                default: break;
            }
            if (event.type == MathLib::Event::Type::MousePressed) {
                if (currentLevel > 1 && MathLib::CheckBoxCollision(event.mouseX, event.mouseY, leftUIArrowX, leftUIArrowY, 1, 1, leftUIArrow.GetWidth(), leftUIArrow.GetHeight())) {
                    currentLevel -= 2;
                    loadLevel = true;
                }
                else loadLevel |= currentLevel <= lastLevel && currentLevel < levels.GetSize() && MathLib::CheckBoxCollision(event.mouseX, event.mouseY, rightUIArrowX, rightUIArrowY, 1, 1, rightUIArrow.GetWidth(), rightUIArrow.GetHeight());
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}