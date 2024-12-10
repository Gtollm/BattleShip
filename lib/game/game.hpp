#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "lib/field/field.hpp"
#include "lib/strategy/strategy.hpp"

namespace BattleShipGame {
enum class Role { MASTER, SLAVE };

class Game {
 private:
  std::shared_ptr<Strategy> strategy_;
  std::shared_ptr<AbstractPlayerField> my_field_;
  std::shared_ptr<AbstractOpponentField> opponent_field_;
  std::optional<std::size_t> width_;
  std::optional<std::size_t> height_;
  std::vector<std::optional<std::size_t>> num_ships_ =
      std::vector<std::optional<std::size_t>>(4);
  std::unordered_map<std::string, std::shared_ptr<Strategy>>
      used_strategies_;

  Role role_;
  bool was_strated_ = false;
  bool stopped_ = false;

 public:
  void Start();

  void Stop();

  void SetWidth(std::size_t width);

  void SetHeight(std::size_t height);

  void SetPlayerField(std::size_t width, std::size_t height);

  void SetCellState(std::size_t row, std::size_t col, CellState state);

  void SetRole(Role role);

  [[nodiscard]] std::optional<std::size_t> GetWidth() const;

  [[nodiscard]] std::optional<std::size_t> GetHeight() const;

  void SetShips(const std::vector<std::size_t>& ships);

  void SetShip(std::size_t length, std::size_t ships);

  [[nodiscard]] std::vector<std::optional<std::size_t>> GetShips() const;

  [[nodiscard]] std::optional<std::size_t> GetShip(std::size_t length) const;

  [[nodiscard]] const std::unordered_map<std::string,
                                         std::shared_ptr<Strategy>>&
  GetStrategies() const;

  void AddStrategy(const std::string& name,
                   std::shared_ptr<Strategy> strategy);

  void SetStrategy(std::shared_ptr<Strategy> strategy);

  [[nodiscard]] bool GameFinished() const;

  [[nodiscard]] bool GameWon() const;

  [[nodiscard]] bool GameLost() const;

  [[nodiscard]] Coord GetShootingCoord() const;

  [[nodiscard]] CellState GetShootingResult(std::size_t row,
                                            std::size_t col) const;

  [[nodiscard]] CellState GetShootingResult(Coord coord) const;

  [[nodiscard]] std::shared_ptr<AbstractPlayerField> GetPlayerField();

  [[nodiscard]] std::shared_ptr<AbstractOpponentField> GetOpponentField();

  void CreateGame();

  void SetShootingResult(CellState hit);

 private:
  void CreateField();
};
}  // namespace BattleShipGame
