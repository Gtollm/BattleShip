#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "commands.hpp"
#include "lib/field/field.hpp"
#include "lib/game/game.hpp"
#include "lib/strategy/ordered_strategy/ordered_strategy.hpp"
#include "lib/strategy/probabilistic_strategy/probabilistic_strategy.hpp"

namespace BattleShipGame {
std::string PingCommand::Execute() const { return "pong"; }

std::string ExitCommand::Execute() const { exit(0); }

CreateCommand::CreateCommand(Game &game, Role role)
    : game_(game), role_(role) {}

std::string CreateCommand::Execute() const {
  this->game_.SetRole(this->role_);
  this->game_.CreateGame();
  return "ok";
}

StartCommand::StartCommand(Game &game) : game_(game) {}

std::string StartCommand::Execute() const {
  this->game_.Start();
  return "ok";
}

StopCommand::StopCommand(Game &game) : game_(game) {}

std::string StopCommand::Execute() const {
  this->game_.Stop();
  return "ok";
}

SetWidthCommand::SetWidthCommand(Game &game, std::size_t width)
    : game_(game), width_(width) {}

std::string SetWidthCommand::Execute() const {
  if (this->game_.GetWidth().has_value()) {
    return "failed";
  }
  this->game_.SetWidth(this->width_);
  return "ok";
}

GetWidthCommand::GetWidthCommand(Game &game) : game_(game) {}

std::string GetWidthCommand::Execute() const {
  std::optional<std::size_t> width = this->game_.GetWidth();
  if (width.has_value()) {
    return std::move(std::to_string(*width));
  }
  return "failed";
}

SetHeightCommand::SetHeightCommand(Game &game, std::size_t height)
    : game_(game), height_(height) {}

std::string SetHeightCommand::Execute() const {
  if (this->game_.GetHeight().has_value()) {
    return "failed";
  }
  this->game_.SetHeight(this->height_);
  return "ok";
}

GetHeightCommand::GetHeightCommand(Game &game) : game_(game) {}

std::string GetHeightCommand::Execute() const {
  std::optional<std::size_t> height = this->game_.GetHeight();
  if (height.has_value()) {
    return std::move(std::to_string(*height));
  }
  return "failed";
}

SetCountCommand::SetCountCommand(Game &game, std::size_t type,
                                 std::size_t amount)
    : game_(game), type_(type), amount_(amount) {}

std::string SetCountCommand::Execute() const {
  if (this->game_.GetShip(this->type_).has_value()) {
    return "failed";
  }
  this->game_.SetShip(this->type_, this->amount_);
  return "ok";
}

GetCountCommand::GetCountCommand(Game &game, std::size_t type)
    : game_(game), type_(type) {}

std::string GetCountCommand::Execute() const {
  std::optional<std::size_t> ship = this->game_.GetShip(this->type_);
  if (ship.has_value()) {
    return std::move(std::to_string(*ship));
  }
  return "failed";
}

SetStrategyCommand::SetStrategyCommand(Game &game, std::string strategy)
    : game_(game), strategy_(std::move(strategy)) {}

std::string SetStrategyCommand::Execute() const {
  auto &strategies = this->game_.GetStrategies();
  auto it = strategies.find(this->strategy_);
  if (it == strategies.end()) {
    if (this->strategy_ == "ordered") {
      this->game_.AddStrategy(this->strategy_,
                              std::move(std::make_shared<OrderedStrategy>(
                                  this->game_.GetOpponentField())));
    } else if (this->strategy_ == "custom") {
      this->game_.AddStrategy(this->strategy_,
                              std::make_shared<ProbabilisticStrategy>(
                                  this->game_.GetOpponentField()));
    }
    auto &strategies_new = this->game_.GetStrategies();

    it = strategies_new.find(this->strategy_);
  }
  this->game_.SetStrategy(it->second);
  return "ok";
}

ShotXYCommand::ShotXYCommand(Game &game, std::size_t x, std::size_t y)
    : game_(game), x_(x), y_(y) {}

std::string ShotXYCommand::Execute() const {
  switch (this->game_.GetShootingResult({this->x_, this->y_})) {
    case CellState::KILL:
      return "kill";
    case CellState::SHIP:
      return "hit";
    case CellState::WATER:
      return "miss";
    default:
      return "failed";
  }
}

ShotCommand::ShotCommand(Game &game) : game_(game) {}

std::string ShotCommand::Execute() const {
  const Coord coord = this->game_.GetShootingCoord();
  std::string r = std::to_string(coord.x) + " " + std::to_string(coord.y);
  return r;
}

SetResultCommand::SetResultCommand(Game &game, std::string result)
    : game_(game), result_(std::move(result)) {}

std::string SetResultCommand::Execute() const {
  if (this->result_ == "miss") {
    this->game_.SetShootingResult(CellState::WATER);
  } else if (this->result_ == "hit") {
    this->game_.SetShootingResult(CellState::SHIP);
  } else if (this->result_ == "kill") {
    this->game_.SetShootingResult(CellState::KILL);
  } else {
    return "failed";
  }

  return "ok";
}

FinishedCommand::FinishedCommand(Game &game) : game_(game) {}

std::string FinishedCommand::Execute() const {
  if (this->game_.GameFinished()) {
    return "yes";
  }
  return "no";
}

WinCommand::WinCommand(Game &game) : game_(game) {}

std::string WinCommand::Execute() const {
  if (this->game_.GameWon()) {
    return "yes";
  }
  return "no";
}

LoseCommand::LoseCommand(Game &game) : game_(game) {}

std::string LoseCommand::Execute() const {
  if (this->game_.GameLost()) {
    return "yes";
  }
  return "no";
}

bool IsShip(CellState state) {
  if (state == CellState::SHIP || state == CellState::KILL) {
    return true;
  }
  return false;
}

DumpCommand::DumpCommand(Game &game, std::string path)
    : game_(game), path_(std::move(path)) {}

std::string DumpCommand::Execute() const {
  std::ofstream fout;

  std::string line;

  fout.open(this->path_);
  fout << (*this->game_.GetWidth()) << " " << (*this->game_.GetHeight())
       << std::endl;

  for (std::size_t row = 0; row < *this->game_.GetHeight(); ++row) {
    for (std::size_t col = 0; col < *this->game_.GetWidth(); ++col) {
      if (IsShip(this->game_.GetPlayerField()->GetCellState(row, col)) &&
          (row >= 1 && !IsShip(this->game_.GetPlayerField()->GetCellState(
                           row - 1, col))) &&
          (col >= 1 && !IsShip(this->game_.GetPlayerField()->GetCellState(
                           row, col - 1)))) {
        std::string direction;
        if (row + 1 < this->game_.GetHeight() &&
            IsShip(
                this->game_.GetPlayerField()->GetCellState(row + 1, col))) {
          direction = "v";
        } else {
          direction = "h";
        }

        auto states = this->game_.GetPlayerField()->FullShip({col, row});

        std::size_t ship_size = states.size();

        fout << ship_size << " " << direction << " " << col << " " << row
             << std::endl;
      }
    }
  }
  fout.close();
  return "ok";
}

LoadCommand::LoadCommand(Game &game, std::string path)
    : game_(game), path_(std::move(path)) {}

std::string LoadCommand::Execute() const {
  std::ifstream fin;

  std::string line;

  fin.open(this->path_);
  std::size_t width, height;
  fin >> width >> height;
  this->game_.SetPlayerField(width, height);

  while (getline(fin, line)) {
    std::istringstream iss(line);
    std::size_t ship_size;
    std::string direction;
    Coord coord;
    iss >> ship_size >> direction >> coord.x >> coord.y;

    if (direction == "h") {
      for (std::size_t i = coord.x; i < coord.x + ship_size; ++i) {
        this->game_.SetCellState(coord.y, i, CellState::SHIP);
      }
    } else if (direction == "v") {
      for (std::size_t i = coord.y; i < coord.y + ship_size; ++i) {
        this->game_.SetCellState(i, coord.x, CellState::SHIP);
      }
    }
  }
  fin.close();
  return "ok";
}
}  // namespace BattleShipGame
