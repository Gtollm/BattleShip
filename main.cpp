
#include <iostream>
#include <string>

#include "lib/game/game.hpp"
#include "lib/game_manager/game_manager.hpp"
#include "lib/writer/writer.hpp"

int main() {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  while (true) {
    std::string cmd;
    std::getline(std::cin, cmd);
    manager.ProcessCommand(cmd);
  }

  return 0;
}
