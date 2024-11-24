#define SDL_MAIN_HANDLED
// #define ShowHitbox
#include "NPC.hpp"
#include "Quest.hpp"
#include "Ending.hpp"
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

#define LoadImage(name, path_)                                  \
    MathLib::DummyRenderer name = MathLib::DummyRenderer(0, 0); \
    if (!name.SetImage<MathLib::TGA>() || !name.LoadFromPath(fs, path + path_)) MathLib::Panic("Failed to load image")

void SetFirstVisibleQuest(size_t& quest, const MathLib::Array<Quest>& quests, bool incrementFirst) {
    bool first = true;
    while (true) {
        if (incrementFirst) quest = (quest + 1) % quests.GetSize();
        else incrementFirst = true;
        if (!quests.At(quest).IsHidden()) return;
        else if (!quest && !first) {
            quest = SIZE_MAX;
            return;
        }
        first = false;
    }
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(void) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("RPG", 800, 800);
        MathLib::HostFileSystem fs;
        const MathLib::String path = "src/TestPrograms/RPG/";
        LoadImage(playerImage, "Player.tga");
        LoadImage(oldManImage, "OldMan.tga");
        LoadImage(blackPawnImage, "BlackPawn.tga");
        Player<MathLib::num_t> player = Player<MathLib::num_t>(playerImage, 100, MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        MathLib::Array<Entity<MathLib::num_t>> entities = std::vector<Entity<MathLib::num_t>> {};
        MathLib::Array<NPC<MathLib::num_t>> npcs = std::vector<NPC<MathLib::num_t>> {
            NPC<MathLib::num_t>(oldManImage, 100, MathLib::CreateVector<MathLib::num_t>(0, 1, 0), std::vector<Dialog> {
                Dialog ( // 0
                    "Jak ja dawno nie widzialem kostek\nMyslalem, ze juz wszyscy wymarliscie",
                    std::vector<DialogOption> {
                        DialogOption("A jednak przezylem, chociaz nic nie pamietam\nMozesz mi jakos pomoc odzyskac pamiec?", 2),
                        DialogOption("Opowiedz mi historie o tym jak wymarly kostki", 1),
                    }
                ),
                Dialog ( // 1
                    "Dawno temu byla wielka wojna miedzy wszystkimi grami\nTo wlasnie wtedy kostki byly najwazniejsze\nAle przez nagly sojusz Chin i jakiejs armii robotow poprostu zniknely",
                    std::vector<DialogOption> {
                        DialogOption("...", 0),
                    }
                ),
                Dialog ( // 2
                    "Pewnie w Ameryce znajdziesz kogos kto ci pomoze, ale dostanie sie tam jest praktycznie niemozliwe\nMoze poszukasz beretu mojej martwej zony i znajdziesz odpowiedzi na swoje po drodze?",
                    std::vector<DialogOption> {
                        DialogOption("Pomoge ci", 3),
                        DialogOption("Nie moge ci pomoc", 4),
                    }
                ),
                Dialog ( // 3
                    "Do zobaczenia kostko",
                    std::vector<DialogOption> {}
                ),
                Dialog ( // 4
                    "Rozumiem",
                    std::vector<DialogOption> {}
                ),
            }, std::vector<NPCTrigger> {
                NPCTrigger(NPCTrigger::Type::Ending, 4, 0),
            }),
            NPC<MathLib::num_t>(blackPawnImage, 100, MathLib::CreateVector<MathLib::num_t>(0, 6, 0), std::vector<Dialog> {
                Dialog ( // 0
                    "Odejdz",
                    std::vector<DialogOption> {}
                ),
            }, std::vector<NPCTrigger> {}),
        };
        MathLib::Array<Quest> quests = std::vector<Quest> {
            Quest("Berecik", "Nie majac zadnej wizji na odzyskanie pamieci postanowilem pomoc starszemu\npanu poszukujacego skradzionego beretu jego martwej zony", std::vector<QuestStep> {
                QuestStep("Pogadaj z dziadkiem", AbsoluteDialogIndex(0, 3), true),
                QuestStep("Znajdz beret dziadka", AbsoluteDialogIndex(SIZE_MAX, SIZE_MAX), false),
            }),
            Quest("Amerykanski sen", "Wedlug dziadka w Ameryce znajde pomoc\nProblem w tym ze nie wiem jak sie tam dostac", std::vector<QuestStep> {
                QuestStep("Pogadaj z dziadkiem", AbsoluteDialogIndex(0, 2), true),
                QuestStep("Znajdz sposob na dostanie sie do Ameryki", AbsoluteDialogIndex(SIZE_MAX, SIZE_MAX), false),
            }),
        };
        const Ending endings[] = {
            // TODO: Ending dziadka
        };
        const MathLib::Matrix<MathLib::num_t> start = renderer.GetStart<MathLib::num_t>();
        const MathLib::Matrix<MathLib::num_t> end = renderer.GetEnd<MathLib::num_t>();
        MathLib::Matrix<MathLib::num_t> move = MathLib::CreateVector<MathLib::num_t>(0, 0, 0);
        MathLib::Second<MathLib::num_t> prevTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
        size_t activeNPC = SIZE_MAX;
        size_t currentShownQuest = SIZE_MAX;
        while (true) {
            const MathLib::Second<MathLib::num_t> currTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
            const MathLib::Second<MathLib::num_t> dt = currTime - prevTime;
            prevTime = currTime;
            renderer.position = MathLib::CreateVector<MathLib::num_t>(0, 0, 0);
            if (activeNPC == SIZE_MAX && currentShownQuest == SIZE_MAX) player.position += move * 5 * dt.GetValue();
            while (GetX(player.position - renderer.position) <= GetX(start)) GetX(renderer.position) += GetX(start) * 2;
            while (GetX(player.position - renderer.position) >= GetX(end)) GetX(renderer.position) += GetX(end) * 2;
            while (GetY(player.position - renderer.position) <= GetY(start)) GetY(renderer.position) += GetY(start) * 2;
            while (GetY(player.position - renderer.position) >= GetY(end)) GetY(renderer.position) += GetY(end) * 2;
            renderer.Fill(0);
            for (const Entity<MathLib::num_t>& entity : entities)
                if (!entity.Draw(renderer)) MathLib::Panic("Failed to draw entity");
            for (const NPC<MathLib::num_t>& npc : npcs)
                if (!npc.Draw(renderer)) MathLib::Panic("Failed to draw NPC");
            if (!player.Draw(renderer)) MathLib::Panic("Failed to draw player");
            renderer.position = MathLib::CreateVector<MathLib::num_t>(0, 0, 0);
            if (activeNPC != SIZE_MAX && !renderer.Puts<MathLib::num_t>(npcs.At(activeNPC).GetDialog(player), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, -2, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to draw NPC dialog");
            if (currentShownQuest != SIZE_MAX && !renderer.Puts<MathLib::num_t>(quests.At(currentShownQuest).ToString(), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, 0, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to draw quest box");
            if (!renderer.Update()) MathLib::Panic("Failed to update renderer");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed) {
                switch (event.key) {
                    case 'w': {
                        GetY(move) = event.pressed;
                        break;
                    }
                    case 's': {
                        GetY(move) = -event.pressed;
                        break;
                    }
                    case 'a': {
                        GetX(move) = -event.pressed;
                        break;
                    }
                    case 'd': {
                        GetX(move) = event.pressed;
                        break;
                    }
                    case 'q': {
                        if (event.pressed) {
                            if (currentShownQuest == SIZE_MAX) {
                                currentShownQuest = 0;
                                SetFirstVisibleQuest(currentShownQuest, quests, false);
                            }
                            else currentShownQuest = SIZE_MAX;
                        }
                        break;
                    }
                    case '0' ... '9': {
                        if (event.pressed && activeNPC != SIZE_MAX && currentShownQuest == SIZE_MAX) {
                            const NPCTrigger trigger = npcs.At(activeNPC).SetOption(player, event.key - '0');
                            switch (trigger.type) {
                                case NPCTrigger::Type::Fight: {
                                    // TODO:
                                    break;
                                }
                                case NPCTrigger::Type::Ending: {
                                    // TODO:
                                    break;
                                }
                                default: break;
                            }
                            player.AddHeardDialog(AbsoluteDialogIndex(activeNPC, npcs.At(activeNPC).GetDialogIndex()));
                            for (Quest& quest : quests) quest.Check(player);
                        }
                        break;
                    }
                    case 'e': {
                        if (event.pressed) {
                            if (currentShownQuest != SIZE_MAX) SetFirstVisibleQuest(currentShownQuest, quests, true);
                            else {
                                if (activeNPC != SIZE_MAX) activeNPC = SIZE_MAX;
                                else {
                                    MathLib::Matrix<MathLib::num_t> diff = MathLib::CreateVector<MathLib::num_t>(SIZE_MAX, SIZE_MAX, 0);
                                    size_t npc = SIZE_MAX;
                                    for (size_t i = 0; i < npcs.GetSize(); i++) {
                                        const MathLib::Matrix<MathLib::num_t> tmp = player.position - npcs.At(i).position;
                                        if (tmp.GetLengthSquared() < diff.GetLengthSquared()) {
                                            diff = tmp;
                                            npc = i;
                                        }
                                    }
                                    if (npc != SIZE_MAX && npcs.At(npc).Collides(player.position)) activeNPC = npc;
                                }
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