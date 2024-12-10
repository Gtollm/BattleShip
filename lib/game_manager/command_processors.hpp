#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "lib/commands/commands.hpp"
#include "lib/game/game.hpp"

namespace BattleShipGame {
using CommandPtr = std::unique_ptr<Command>;
using CommandFactory =
    std::function<CommandPtr(Game &, const std::string &)>;

namespace CommandProcessors {
CommandPtr ProcessPing(Game &, const std::string &);
CommandPtr ProcessExit(Game &, const std::string &);
CommandPtr ProcessCreate(Game &game, const std::string &role);
CommandPtr ProcessStart(Game &game, const std::string &);
CommandPtr ProcessStop(Game &game, const std::string &);
CommandPtr ProcessSetWidth(Game &game, const std::string &arguments);
CommandPtr ProcessGetWidth(Game &game, const std::string &);
CommandPtr ProcessSetHeight(Game &game, const std::string &arguments);
CommandPtr ProcessGetHeight(Game &game, const std::string &);
CommandPtr ProcessSetCount(Game &game, const std::string &arguments);
CommandPtr ProcessGetCount(Game &game, const std::string &arguments);
CommandPtr ProcessSetStrategy(Game &game, const std::string &arguments);
CommandPtr ProcessShotXY(Game &game, const std::string &arguments);
CommandPtr ProcessShot(Game &game, const std::string &);
CommandPtr ProcessSetResult(Game &game, const std::string &arguments);
CommandPtr ProcessFinished(Game &game, const std::string &);
CommandPtr ProcessWin(Game &game, const std::string &);
CommandPtr ProcessLose(Game &game, const std::string &);
CommandPtr ProcessDump(Game &game, const std::string &path);
CommandPtr ProcessLoad(Game &game, const std::string &path);
}  // namespace CommandProcessors
}  // namespace BattleShipGame
