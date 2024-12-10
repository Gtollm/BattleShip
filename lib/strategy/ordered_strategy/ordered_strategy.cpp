#include <memory>

#include "lib/field/field.hpp"
#include "lib/strategy/ordered_strategy/ordered_strategy.hpp"

namespace BattleShipGame {
OrderedStrategy::OrderedStrategy(
    std::shared_ptr<AbstractOpponentField> field)
    : field_(field) {}

Coord OrderedStrategy::GetShootingCoords() {
  if (this->last_shot_.x + 1 < this->field_->Width()) {
    ++this->last_shot_.x;
  } else if (this->last_shot_.y + 1 < this->field_->Height()) {
    this->last_shot_.x = 0;
    ++this->last_shot_.y;
  }
  return this->last_shot_;
}

void OrderedStrategy::SetShootingResult(CellState hit) {
  this->field_->RecordShot(this->last_shot_, hit);
}

void OrderedStrategy::SetField(
    std::shared_ptr<AbstractOpponentField> field) {
  this->field_ = field;
}

bool OrderedStrategy::GameWon() {
  if (this->last_shot_.x == this->field_->Width() - 1 &&
      this->last_shot_.y == this->field_->Height() - 1) {
    return true;
  }
  return false;
}
}  // namespace BattleShipGame
