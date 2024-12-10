#pragma once

#include <lib/field/field.hpp>
#include <memory>

#include "lib/strategy/strategy.hpp"

namespace BattleShipGame {
class OrderedStrategy final : public Strategy {
 public:
  OrderedStrategy(std::shared_ptr<AbstractOpponentField> field);
  ~OrderedStrategy() override = default;

  Coord GetShootingCoords() override;

  void SetShootingResult(CellState hit) override;

  void SetField(std::shared_ptr<AbstractOpponentField> field) override;

  bool GameWon() override;

 private:
  Coord last_shot_;
  std::shared_ptr<AbstractOpponentField> field_;
};
}  // namespace BattleShipGame
