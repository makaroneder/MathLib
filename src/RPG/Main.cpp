#define SDL_MAIN_HANDLED
#define ShowHitbox
#include "NPC.hpp"
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

#define LoadImage(name, path_)                  \
    DummyRenderer name = DummyRenderer(0, 0);   \
    if (!name.SetImage<TGA>() || !name.LoadFromPath(fs, path + path_)) Panic("Failed to load image")

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(void) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        SDL2Renderer renderer = SDL2Renderer("RPG", 800, 800);
        HostFileSystem fs;
        const String path = "src/TestPrograms/RPG/";
        LoadImage(playerImage, "Player.tga");
        LoadImage(blackPawnImage, "BlackPawn.tga");
        Player<num_t> player = Player<num_t>(playerImage, 100, CreateVector<num_t>(0, 0, 0));
        Array<Entity<num_t>> entities = std::vector<Entity<num_t>> {
            Entity<num_t>(blackPawnImage, 100, CreateVector<num_t>(0, 0, 0)),
        };
        Array<NPC<num_t>> npcs = std::vector<NPC<num_t>> {
            NPC<num_t>(blackPawnImage, 100, CreateVector<num_t>(0, 1, 0), std::vector<Dialog> {
                Dialog (
                    "Nareszcie jakas nowa twarz",
                    std::vector<DialogOption> {
                        DialogOption("Dzien dobry", 1),
                    }
                ),
                Dialog (
                    "Mow mi Zbyszek",
                    std::vector<DialogOption> {
                        DialogOption("No dobra", 2),
                    }
                ),
                Dialog (
                    "Wojtek naucz sie robic dobre dialogi",
                    std::vector<DialogOption> {}
                ),
            }),
        };
        const Matrix<num_t> start = renderer.GetStart<num_t>();
        const Matrix<num_t> end = renderer.GetEnd<num_t>();
        Matrix<num_t> move = CreateVector<num_t>(0, 0, 0);
        Second<num_t> prevTime = Second<num_t>(GetTime());
        size_t activeNPC = SIZE_MAX;
        while (true) {
            const Second<num_t> currTime = Second<num_t>(GetTime());
            const Second<num_t> dt = currTime - prevTime;
            prevTime = currTime;
            renderer.position = CreateVector<num_t>(0, 0, 0);
            player.position += move * 5 * dt.GetValue();
            while (GetX(player.position - renderer.position) <= GetX(start)) GetX(renderer.position) += GetX(start) * 2;
            while (GetX(player.position - renderer.position) >= GetX(end)) GetX(renderer.position) += GetX(end) * 2;
            while (GetY(player.position - renderer.position) <= GetY(start)) GetY(renderer.position) += GetY(start) * 2;
            while (GetY(player.position - renderer.position) >= GetY(end)) GetY(renderer.position) += GetY(end) * 2;
            renderer.Fill(0);
            for (const Entity<num_t>& entity : entities)
                if (!entity.Draw(renderer)) Panic("Failed to draw entity");
            for (const NPC<num_t>& npc : npcs)
                if (!npc.Draw(renderer)) Panic("Failed to draw NPC");
            if (!player.Draw(renderer)) Panic("Failed to draw player");
            renderer.position = CreateVector<num_t>(0, 0, 0);
            if (activeNPC != SIZE_MAX && !renderer.Puts<num_t>(npcs.At(activeNPC).GetDialog(player), &_binary_src_Lib_zap_light16_psf_start, CreateVector<num_t>(0, -3, 0), CreateVector<num_t>(0, 0, 0), CreateVector<size_t>(1, 1, 1), UINT32_MAX)) Panic("Failed to draw NPC dialog");
            if (!renderer.Update()) Panic("Failed to update renderer");
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
            else if (event.type == Event::Type::KeyPressed || event.type == Event::Type::KeyReleased) {
                const bool pressed = event.type == Event::Type::KeyPressed;
                switch (event.key) {
                    case 'w': {
                        GetY(move) = pressed;
                        break;
                    }
                    case 's': {
                        GetY(move) = -pressed;
                        break;
                    }
                    case 'a': {
                        GetX(move) = -pressed;
                        break;
                    }
                    case 'd': {
                        GetX(move) = pressed;
                        break;
                    }
                    case '0' ... '9': {
                        if (pressed && activeNPC != SIZE_MAX) npcs.At(activeNPC).SetOption(player, event.key - '0');
                        break;
                    }
                    case 'e': {
                        if (pressed) {
                            if (activeNPC != SIZE_MAX) activeNPC = SIZE_MAX;
                            else {
                                Matrix<num_t> diff = CreateVector<num_t>(SIZE_MAX, SIZE_MAX, 0);
                                size_t npc = SIZE_MAX;
                                for (size_t i = 0; i < npcs.GetSize(); i++) {
                                    const Matrix<num_t> tmp = player.position - npcs.At(i).position;
                                    if (tmp.GetLengthSquared() < diff.GetLengthSquared()) {
                                        diff = tmp;
                                        npc = i;
                                    }
                                }
                                if (npc != SIZE_MAX && npcs.At(npc).Collides(player.position)) activeNPC = npc;
                            }
                        }
                        break;
                    }
                }
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}