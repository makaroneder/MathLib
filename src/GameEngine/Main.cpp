#include "GameEngine.hpp"
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Preprocesor.hpp>
#include <SDL2.hpp>
#include <iostream>

/// @brief Creates wall
/// @tparam T Type of number
/// @param data Data about the wall
/// @return Wall
template <typename T>
constexpr Object<T> MakeWall(std::vector<T> data) {
    return Object<T>(Cuboid<T>(CreateVector<T>(data.at(0), data.at(1), data.at(2)), CreateVector<T>(data.at(3), data.at(4), data.at(5))), MetrePerSecond<T>(0), data.at(6));
}
/// @brief Creates entity
/// @tparam T Type of number
/// @param data Data about the entity
/// @return Entity
template <typename T>
constexpr Entity<T> MakeEntity(std::vector<T> data) {
    return Entity<T>(Cuboid<T>(CreateVector<T>(data.at(0), data.at(1), data.at(2)), CreateVector<T>(data.at(3), data.at(4), data.at(5))), MetrePerSecond<T>(data.at(6)), data.at(7));
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) throw std::runtime_error("No input files provided");
        Node* root = Tokenize(Preproces(argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        std::vector<num_t> player;
        std::vector<std::vector<std::vector<std::vector<num_t>>>> levelsData;
        State state = State({
            BuiltinFunction("InitGameEngine", [&player, &levelsData](std::vector<const Node*> args) -> Node* {
                const std::vector<std::complex<num_t>> tmp = args[0]->ToNumber();
                for (const std::complex<num_t>& val : tmp) {
                    if (val.imag() != 0) return new Node(Node::Type::Constant, "1");
                    player.push_back(val.real());
                }
                const std::vector<const Node*> tmp1 = CommaToArray(args[1]->left);
                for (auto& node : tmp1) {
                    const std::vector<const Node*> tmp2 = CommaToArray(node->left);
                    std::vector<std::vector<std::vector<num_t>>> objs1;
                    for (auto& val1 : tmp2) {
                        const std::vector<const Node*> tmp3 = CommaToArray(val1->left);
                        std::vector<std::vector<num_t>> objs2;
                        for (auto& val2 : tmp3) {
                            const std::vector<std::complex<num_t>> tmp3 = val2->ToNumber();
                            std::vector<num_t> obj;
                            for (const std::complex<num_t>& val : tmp3) {
                                if (val.imag() != 0) return nullptr;
                                obj.push_back(val.real());
                            }
                            objs2.push_back(obj);
                        }
                        objs1.push_back(objs2);
                    }
                    levelsData.push_back(objs1);
                }
                return new Node(Node::Type::Constant, "0");
            }),
        }, {}, {
            Variable("black", "0x000000ff"),
            Variable("white", "0xffffffff"),
            Variable("red", "0xff0000ff"),
            Variable("green", "0x00ff00ff"),
            Variable("blue", "0x0000ffff"),
            Variable("gray", "0x808080ff"),
            Variable("yellow", "0xffff00ff"),
        });
        Node* optimizedRoot = Optimize(root, state);
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        delete root;
        std::vector<Level<num_t>> levels;
        for (const std::vector<std::vector<std::vector<num_t>>>& level : levelsData) {
            std::vector<Object<num_t>> walls;
            std::vector<Entity<num_t>> entities;
            for (const std::vector<num_t>& wall : level.at(1)) walls.push_back(MakeWall<num_t>(wall));
            for (const std::vector<num_t>& entity : level.at(2)) entities.push_back(MakeEntity<num_t>(entity));
            levels.push_back(Level<num_t>(MakeWall<num_t>(level.at(0).at(0)), walls, entities));
        }
        GameEngine<num_t> engine = GameEngine<num_t>(Object<num_t>(Cuboid<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(player.at(0), player.at(1), player.at(2))), MetrePerSecond<num_t>(player.at(3)), player.at(4)), levels);
        Matrix<num_t> moveVector = CreateVector<num_t>(0, 0, 0);
        SDL2Renderer renderer = SDL2Renderer("Game Engine", 800, 800);
        while (true) {
            if (!engine.Draw(renderer)) throw std::runtime_error("Failed to render level");
            if (!renderer.Update()) throw std::runtime_error("Failed to update UI");
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
            else if (event.type == Event::Type::KeyPressed || event.type == Event::Type::KeyReleased) {
                const bool set = (event.type == Event::Type::KeyPressed);
                switch (event.data.key) {
                    case 'w': {
                        GetY(moveVector) = set;
                        break;
                    }
                    case 's': {
                        GetY(moveVector) = -set;
                        break;
                    }
                    case 'a': {
                        GetX(moveVector) = -set;
                        break;
                    }
                    case 'd': {
                        GetX(moveVector) = set;
                        break;
                    }
                }
            }
            const LevelStatus status = engine.Update(Second<num_t>(1), moveVector);
            if (status == LevelStatus::Error) throw std::runtime_error("Failed to update game");
            else if (status == LevelStatus::GameOver) {
                std::cout << "You lost" << std::endl;
                break;
            }
            else if (status == LevelStatus::Victory) {
                std::cout << "You won" << std::endl;
                break;
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}