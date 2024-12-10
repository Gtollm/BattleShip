
#include "probabilistic_strategy.hpp"

namespace BattleShipGame {
ProbabilisticStrategy::ProbabilisticStrategy(
    std::shared_ptr<AbstractOpponentField> field)
    : field_(field) {}

Coord ProbabilisticStrategy::GetShootingCoords() {
  if (this->first_call_) {
    std::vector<std::size_t> ships = this->field_->GetShips();
    for (auto length : ships) {
      remaining_ships_[length]++;
    }
    first_call_ = false;
  }
  std::vector<std::int64_t> probabilities(
      this->field_->Width() * this->field_->Height(), 0);
  for (auto& [length, count] : remaining_ships_) {
    for (std::size_t row = 0; row < this->field_->Height(); ++row) {
      std::vector<bool> mask(this->field_->Width(), true);
      for (std::size_t col = 0; col < this->field_->Width(); ++col) {
        CellState state = field_->GetCellState(row, col);
        if (state == CellState::KILL || state == CellState::WATER) {
          mask[col] = 1;
        }
      }
      std::vector<std::int64_t> prefix(this->field_->Width(), 0);
      for (std::size_t i = 0; i < this->field_->Width(); ++i) {
        prefix[i] = mask[i] + (i > 0 ? prefix[i - 1] : 0);
      }
      for (std::size_t start = 0; start <= this->field_->Width() - length;
           ++start) {
        if (prefix[start + length - 1] -
                (start > 0 ? prefix[start - 1] : 0) ==
            length) {
          for (std::size_t offset = 0; offset < length; ++offset) {
            probabilities[row * this->field_->Width() + start + offset]++;
          }
        }
      }
    }
    for (std::size_t col = 0; col < this->field_->Width(); ++col) {
      std::vector<std::int64_t> mask(this->field_->Height(), 0);
      for (std::size_t row = 0; row < this->field_->Height(); ++row) {
        if (field_->GetCellState(row, col) == CellState::WATER ||
            field_->GetCellState(row, col) == CellState::SHIP) {
          mask[row] = 1;
        }
      }
      std::vector<std::int64_t> prefix(this->field_->Height(), 0);
      for (std::size_t i = 0; i < this->field_->Height(); ++i) {
        prefix[i] = mask[i] + (i > 0 ? prefix[i - 1] : 0);
      }
      for (std::size_t start = 0; start <= this->field_->Height() - length;
           ++start) {
        if (prefix[start + length - 1] -
                (start > 0 ? prefix[start - 1] : 0) ==
            length) {
          for (std::size_t offset = 0; offset < length; ++offset) {
            probabilities[(start + offset) * this->field_->Width() + col]++;
          }
        }
      }
    }
  }
  int max_prob = -1;
  Coord best(-1, -1);
  for (std::size_t row = 0; row < this->field_->Height(); ++row) {
    for (std::size_t col = 0; col < this->field_->Width(); ++col) {
      if (field_->GetCellState(row, col) == CellState::WATER &&
          probabilities[row * this->field_->Width() + col] > max_prob) {
        max_prob = probabilities[row * this->field_->Width() + col];
        best = Coord(row, col);
      }
    }
  }
  this->last_shot_ = best;
  return best;
}

void ProbabilisticStrategy::SetShootingResult(Coord coord,
                                              CellState result) {
  field_->RecordShot(coord.x, coord.y, result);
  if (result == CellState::KILL) {
    std::vector<Coord> killed_ship = field_->FullShip(coord);
    if (remaining_ships_.find(killed_ship.size()) !=
        remaining_ships_.end()) {
      remaining_ships_[killed_ship.size()]--;
      if (remaining_ships_[killed_ship.size()] == 0) {
        remaining_ships_.erase(killed_ship.size());
      }
    }
  }
}

void ProbabilisticStrategy::SetShootingResult(CellState result) {
  SetShootingResult(last_shot_, result);
}

void ProbabilisticStrategy::SetField(
    std::shared_ptr<AbstractOpponentField> field) {
  this->field_ = field;
}

bool ProbabilisticStrategy::GameWon() {
  if (this->first_call_) {
    std::vector<std::size_t> ships = this->field_->GetShips();
    for (auto length : ships) {
      this->remaining_ships_[length]++;
    }
    this->first_call_ = false;
  }

  std::size_t sm = 0;
  for (auto it : remaining_ships_) {
    sm += it.second;
  }
  return (sm == 0);
}
}  // namespace BattleShipGame
