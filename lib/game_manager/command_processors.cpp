#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "command_processors.hpp"
#include "lib/commands/commands.hpp"
#include "lib/game/game.hpp"

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessPing(
    Game &, const std::string &) {
  return std::move(std::make_unique<PingCommand>());
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessExit(
    Game &, const std::string &) {
  return std::move(std::make_unique<ExitCommand>());
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessCreate(
    Game &game, const std::string &role) {
  if (role == "master") {
    return std::move(std::make_unique<CreateCommand>(game, Role::MASTER));
  } else if (role == "slave") {
    return std::move(std::make_unique<CreateCommand>(game, Role::SLAVE));
  }
  return nullptr;
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessStart(

    Game &game, const std::string &) {
  return std::move(std::make_unique<StartCommand>(game));
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessStop(
    Game &game, const std::string &) {
  return std::move(std::make_unique<StopCommand>(game));
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessSetWidth(
    Game &game, const std::string &arguments) {
  std::istringstream iss(arguments);
  std::size_t width;
  if (iss >> width) {
    return std::move(std::make_unique<SetWidthCommand>(game, width));
  }
  return nullptr;
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessGetWidth(Game &game,
                                                   const std::string &) {
  return std::move(std::make_unique<GetWidthCommand>(game));
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessSetHeight(
    Game &game, const std::string &arguments) {
  std::istringstream iss(arguments);
  std::size_t height;
  if (iss >> height) {
    return std::move(std::make_unique<SetHeightCommand>(game, height));
  }
  return nullptr;
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessGetHeight(Game &game,
                                                    const std::string &) {
  return std::move(std::make_unique<GetHeightCommand>(game));
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessSetCount(
    Game &game, const std::string &arguments) {
  std::istringstream iss(arguments);
  std::size_t type;
  std::size_t amount;
  if (iss >> type >> amount) {
    return std::move(std::make_unique<SetCountCommand>(game, type, amount));
  }
  return nullptr;
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessGetCount(
    Game &game, const std::string &arguments) {
  std::istringstream iss(arguments);
  std::size_t type;
  if (iss >> type) {
    return std::move(std::make_unique<GetCountCommand>(game, type));
  }
  return nullptr;
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessSetStrategy(
    Game &game, const std::string &strategy) {
  return std::move(std::make_unique<SetStrategyCommand>(game, strategy));
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessShotXY(
    Game &game, const std::string &arguments) {
  std::istringstream iss(arguments);
  std::size_t x;
  std::size_t y;
  if (iss >> x >> y) {
    return std::move(std::make_unique<ShotXYCommand>(game, x, y));
  }
  return nullptr;
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessShot(
    Game &game, const std::string &) {
  return std::move(std::make_unique<ShotCommand>(game));
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessSetResult(
    Game &game, const std::string &arguments) {
  return std::move(std::make_unique<SetResultCommand>(game, arguments));
}

BattleShipGame::CommandPtr
BattleShipGame::CommandProcessors::ProcessFinished(Game &game,
                                                   const std::string &) {
  return std::move(std::make_unique<FinishedCommand>(game));
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessWin(
    Game &game, const std::string &) {
  return std::move(std::make_unique<WinCommand>(game));
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessLose(
    Game &game, const std::string &) {
  return std::move(std::make_unique<LoseCommand>(game));
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessDump(
    Game &game, const std::string &path) {
  return std::move(std::make_unique<DumpCommand>(game, path));
}

BattleShipGame::CommandPtr BattleShipGame::CommandProcessors::ProcessLoad(
    Game &game, const std::string &path) {
  return std::move(std::make_unique<LoadCommand>(game, path));
}
