#include "Bullet.hpp"
#include "../Shared/Map.hpp"
#include <FileSystem/FileSystem.hpp>
#include <WindowManager.hpp>
#include <DummyRenderer.hpp>
#include <Defer.hpp>
#include <iostream>

const MathLib::String inputPath = "Data/AAAI/";
MathLib::Aseprite LoadTexture(MathLib::FileSystem& fs, const MathLib::String& path) {
    MathLib::Aseprite ret;
    if (!ret.LoadFromPath(fs, inputPath + path) || ret.IsEmpty()) MathLib::Panic("Failed to load "_M + path);
    return ret;
}
void AddBullet(MathLib::Array<Bullet>& bullets, const Bullet& bullet) {
    for (Bullet& tmp : bullets) {
        if (tmp.IsValid()) continue;
        tmp = bullet;
        return;
    }
    if (!bullets.Add(bullet)) MathLib::Panic("Failed to add bullet");
}
void Main(int, char**, MathLib::FileSystem& fs, MathLib::WindowManager& windowManager) {
    const MathLib::String mapPath = "bin/Map.bin";
    const MathLib::Image ui = LoadTexture(fs, "UI.aseprite").AtUnsafe(0);
    const MathLib::Image cornerLeftTop = LoadTexture(fs, "Corner.aseprite").AtUnsafe(0);
    const MathLib::Image cornerLeftBottom = cornerLeftTop.RotateUpsideDown();
    const MathLib::Image cornerRightTop = cornerLeftTop.Mirror();
    const MathLib::Image cornerRightBottom = cornerLeftBottom.Mirror();
    const MathLib::Aseprite playerIdle = LoadTexture(fs, "PlayerIdle.aseprite");
    const MathLib::Aseprite playerMovement = LoadTexture(fs, "PlayerMovement.aseprite");
    MathLib::Animation playerMovementAnimation = MathLib::Animation(&playerMovement);
    const MathLib::Aseprite bulletRight = LoadTexture(fs, "Bullet.aseprite");
    const MathLib::Video bulletLeft = bulletRight.Mirror();
    const MathLib::Video bulletDown = bulletRight.SwapXY();
    const MathLib::Video bulletUp = bulletDown.RotateUpsideDown();
    const size_t width = ui.GetWidth();
    const size_t height = ui.GetHeight();
    MathLib::Renderer* renderer = windowManager.MakeRenderer("AAAI", width, height);
    if (!renderer) MathLib::Panic("Failed to create renderer");
    defer(delete renderer);
    MathLib::DummyRenderer mapRenderer = MathLib::DummyRenderer(320, 320);
    const size_t uiPadX = (width - mapRenderer.GetWidth()) / 2;
    const size_t uiPadY = (height - mapRenderer.GetHeight()) / 2;
    const size_t centerX = width / 2;
    const size_t centerY = height / 2;
    const ssize_t cellSize = 32;
    const float speedX = 10;
    const float speedY = 10;
    int8_t moveX = 0;
    int8_t moveY = 0;
    float offsetX = 0;
    float offsetY = 0;
    MathLib::Array<Block> blocks;
    LoadMap(fs, mapPath, blocks);
    MathLib::Dictionary<MathLib::String, MathLib::Aseprite> cachedTextures;
    MathLib::Array<Bullet> bullets;
    float prevTime = MathLib::GetTime();
    while (true) {
        const float time = MathLib::GetTime();
        const float deltaTime = time - prevTime;
        std::cout << "FPS: " << 1 / deltaTime << std::endl;
        prevTime = time;
        if (!renderer->CopyFromBuffer(ui)) MathLib::Panic("Failed to draw UI");
        mapRenderer.Fill(0);
        DrawMap(mapRenderer, cellSize, offsetX, offsetY, blocks, fs, inputPath, cachedTextures);
        for (Bullet& bullet : bullets)
            if (!bullet.Draw(mapRenderer, offsetX, offsetY, deltaTime)) bullet = Bullet();
        if (!renderer->CopyImage(mapRenderer, centerX, centerY)) MathLib::Panic("Failed to draw map");
        renderer->DrawImage(cornerLeftTop, uiPadX - 2 + cornerLeftTop.GetWidth() / 2, uiPadY - 2 + cornerLeftTop.GetHeight() / 2);
        renderer->DrawImage(cornerRightTop, width - uiPadX - 5 + cornerLeftTop.GetWidth() / 2, uiPadY - 2 + cornerLeftTop.GetHeight() / 2);
        renderer->DrawImage(cornerLeftBottom, uiPadX - 2 + cornerLeftTop.GetWidth() / 2, height - uiPadY - 5 + cornerLeftTop.GetHeight() / 2);
        renderer->DrawImage(cornerRightBottom, width - uiPadX - 5 + cornerLeftTop.GetWidth() / 2, height - uiPadY - 5 + cornerLeftTop.GetHeight() / 2);
        if (moveX || moveY) {
            offsetX += speedX * moveX * deltaTime;
            offsetY += speedY * moveY * deltaTime;
            playerMovementAnimation.Draw(*renderer, centerX, centerY);
            playerMovementAnimation.Update(deltaTime);
        }
        else {
            renderer->DrawImage(playerIdle.AtUnsafe(0), centerX, centerY);
            playerMovementAnimation.Reset();
        }
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::Quit) break;
        else if (event.type == MathLib::Event::Type::KeyPressed) switch (event.key) {
            case 'w': {
                moveY = event.pressed ? -1 : 0;
                break;
            }
            case 's': {
                moveY = event.pressed ? 1 : 0;
                break;
            }
            case 'a': {
                moveX = event.pressed ? -1 : 0;
                break;
            }
            case 'd': {
                moveX = event.pressed ? 1 : 0;
                break;
            }
            case 'i': {
                if (event.pressed) AddBullet(bullets, Bullet(&bulletUp, offsetX, offsetY, Bullet::Type::Up));
                break;
            }
            case 'k': {
                if (event.pressed) AddBullet(bullets, Bullet(&bulletDown, offsetX, offsetY, Bullet::Type::Down));
                break;
            }
            case 'j': {
                if (event.pressed) AddBullet(bullets, Bullet(&bulletLeft, offsetX, offsetY, Bullet::Type::Left));
                break;
            }
            case 'l': {
                if (event.pressed) AddBullet(bullets, Bullet(&bulletRight, offsetX, offsetY, Bullet::Type::Right));
                break;
            }
            default: break;
        }
    }
}