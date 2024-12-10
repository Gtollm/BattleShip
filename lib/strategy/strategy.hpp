#pragma once

#include "lib/field/field.hpp"

namespace BattleShipGame {
class Strategy {
 public:
  virtual ~Strategy() = default;

  virtual Coord GetShootingCoords() = 0;

  virtual void SetShootingResult(CellState hit) = 0;

  virtual void SetField(std::shared_ptr<AbstractOpponentField> field) = 0;

  virtual bool GameWon() = 0;
};
}  // namespace BattleShipGame
