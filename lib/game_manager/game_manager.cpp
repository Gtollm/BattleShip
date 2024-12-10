#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "game_manager.hpp"
#include "lib/commands/commands.hpp"
#include "lib/game_manager/command_processors.hpp"
#include "lib/writer/writer.hpp"

namespace BattleShipGame {

constexpr std::size_t kGoldenRatioConstant = 0x9e3779b9;

size_t hash_combine(size_t h1, size_t h2) {
  return h1 ^ (h2 + kGoldenRatioConstant + (h1 << 6) + (h1 >> 2));
}

size_t CommandKeyHash::operator()(const CommandKey &key) const {
  size_t h1 = std::hash<std::string>{}(key.name);
  size_t h2 = std::hash<std::size_t>{}(key.num_arguments);
  return hash_combine(h1, h2);
}

bool CommandKeyEqual::operator()(const CommandKey &lhs,
                                 const CommandKey &rhs) const {
  return lhs.name == rhs.name && lhs.num_arguments == rhs.num_arguments;
}

GameManager::GameManager(Game &game) : game_(game) {
  RegisterCommand("ping", 0, CommandProcessors::ProcessPing);
  RegisterCommand("exit", 0, CommandProcessors::ProcessExit);
  RegisterCommand("create", 1, CommandProcessors::ProcessCreate);
  RegisterCommand("start", 0, CommandProcessors::ProcessStart);
  RegisterCommand("set width", 1, CommandProcessors::ProcessSetWidth);
  RegisterCommand("get width", 0, CommandProcessors::ProcessGetWidth);
  RegisterCommand("set height", 1, CommandProcessors::ProcessSetHeight);
  RegisterCommand("get height", 0, CommandProcessors::ProcessGetHeight);
  RegisterCommand("set count", 2, CommandProcessors::ProcessSetCount);
  RegisterCommand("get count", 1, CommandProcessors::ProcessGetCount);
  RegisterCommand("set strategy", 1, CommandProcessors::ProcessSetStrategy);
  RegisterCommand("shot", 2, CommandProcessors::ProcessShotXY);
  RegisterCommand("shot", 0, CommandProcessors::ProcessShot);
  RegisterCommand("set result", 1, CommandProcessors::ProcessSetResult);
  RegisterCommand("finished", 0, CommandProcessors::ProcessFinished);
  RegisterCommand("win", 0, CommandProcessors::ProcessWin);
  RegisterCommand("lose", 0, CommandProcessors::ProcessLose);
  RegisterCommand("dump", 1, CommandProcessors::ProcessDump);
  RegisterCommand("load", 1, CommandProcessors::ProcessLoad);
}

std::string GameManager::ProcessInput(const std::string &input) {
  std::unique_ptr<Command> cmd = CreateCommand(input);
  if (cmd) {
    return cmd->Execute();
  }
  return "failed";
}

void GameManager::ProcessCommand(const std::string &input) {
  std::unique_ptr<Command> cmd = CreateCommand(input);
  if (cmd) {
    std::string command_output = cmd->Execute();
    GenerateOutput(command_output);
  }
}

std::string Join(const std::vector<std::string> &vec,
                 const std::string &delim) {
  std::stringstream result;
  auto it = vec.begin();
  result << *it++;
  for (; it != vec.end(); it++) {
    result << delim;
    result << *it;
  }
  return result.str();
}

std::unique_ptr<Command> GameManager::CreateCommand(
    const std::string &input) {
  std::istringstream iss(input);

  std::vector<std::string> tokens{std::istream_iterator<std::string>(iss),
                                  std::istream_iterator<std::string>()};
  std::string command_name;
  std::unique_ptr<Command> command;
  std::string left = "";
  if (tokens.size() > 0) {
    command_name = tokens[0];
  }
  if (tokens.size() > 1) {
    left = Join({tokens.begin() + 1, tokens.end()}, " ");
  }

  auto it = this->commands_.find({command_name, tokens.size() - 1});
  if (it != this->commands_.end()) {
    command = it->second(this->game_, left);
  }
  if (!command && tokens.size() > 1) {
    command_name = Join({tokens.begin(), tokens.begin() + 2}, " ");
    if (tokens.size() > 2) {
      left = Join({tokens.begin() + 2, tokens.end()}, " ");
    } else {
      left = "";
    }
    auto it = this->commands_.find({command_name, tokens.size() - 2});
    if (it != this->commands_.end()) {
      command = it->second(this->game_, left);
    }
  }
  return std::move(command);
}

void GameManager::RegisterCommand(const std::string &command_name,
                                  std::size_t num_arguments,
                                  CommandFactory factory) {
  CommandKey key{command_name, num_arguments};
  this->commands_[key] = std::move(factory);
}

void GameManager::GenerateOutput(const std::string &response) const {
  this->writer_->OutputCommandResult(response);
}

}  // namespace BattleShipGame
