#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "lib/field/field.hpp"
#include "lib/strategy/strategy.hpp"

namespace BattleShipGame {

class ProbabilisticStrategy : public Strategy {
 public:
  explicit ProbabilisticStrategy(
      std::shared_ptr<AbstractOpponentField> field);

  Coord GetShootingCoords() override;

  void SetShootingResult(Coord coord, CellState result);

  void SetShootingResult(CellState result) override;

  void SetField(std::shared_ptr<AbstractOpponentField> field) override;
  void CalculateProbabilitiesForLength(
      std::size_t length, std::vector<std::vector<bool>>& valid_shots,
      std::vector<std::int64_t>& probabilities);
  bool GameWon() override;

 private:
  std::shared_ptr<AbstractOpponentField> field_;
  std::unordered_map<std::size_t, std::size_t> remaining_ships_;
  Coord last_shot_;
  bool first_call_ = true;
};

}  // namespace BattleShipGame
