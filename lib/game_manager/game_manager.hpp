#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "command_processors.hpp"
#include "lib/commands/commands.hpp"
#include "lib/game/game.hpp"
#include "lib/writer/writer.hpp"

namespace BattleShipGame {
struct CommandKey {
  CommandKey(const std::string &name, std::size_t num_arguments)
      : name(name), num_arguments(num_arguments) {}
  std::string name;
  std::size_t num_arguments;
};

struct CommandKeyHash {
  size_t operator()(const CommandKey &key) const;
};

struct CommandKeyEqual {
  bool operator()(const CommandKey &lhs, const CommandKey &rhs) const;
};

class GameManager {
 public:
  GameManager(Game &game);

  std::string ProcessInput(const std::string &input);
  void ProcessCommand(const std::string &input);

  template <OStreamable stream>
  void SetWriter(const TemplatedWriter<stream> &writer) {
    this->writer_ = std::make_unique<TemplatedWriter<stream> >(writer);
  }

 private:
  std::unique_ptr<Command> CreateCommand(const std::string &command);

  void RegisterCommand(const std::string &commandName,
                       std::size_t num_arguments, CommandFactory factory);

  void GenerateOutput(const std::string &response) const;

  Game &game_;
  std::unique_ptr<Writer> writer_;
  std::unordered_map<CommandKey, CommandFactory, CommandKeyHash,
                     CommandKeyEqual>
      commands_;
};
}  // namespace BattleShipGame
