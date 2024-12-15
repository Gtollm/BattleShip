#include "lib/field/matrix_field/matrix_field.hpp"
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
  utils::VectorMatrix<std::int64_t> probabilities(this->field_->Height(),
                                                  this->field_->Width());

  for (auto& [length, count] : remaining_ships_) {
    for (std::size_t row = 0; row < this->field_->Height(); ++row) {
      std::vector<bool> mask(this->field_->Width(), false);
      for (std::size_t col = 0; col < this->field_->Width(); ++col) {
        if (field_->GetCellState(row, col) == CellState::UNSPECIFIED ||
            field_->GetCellState(row, col) == CellState::SHIP) {
          mask[col] = true;
        }
      }
      for (std::size_t start = 0; start <= this->field_->Width() - length;
           ++start) {
        bool valid = true;
        for (std::size_t i = start; i < start + length; ++i) {
          if (!mask[i]) {
            valid = false;
            break;
          }
        }
        if (valid) {
          for (std::size_t i = start; i < start + length; ++i) {
            probabilities[row][i]++;
          }
        }
      }
    }
    for (std::size_t col = 0; col < this->field_->Width(); ++col) {
      std::vector<bool> mask(this->field_->Height(), false);
      for (std::size_t row = 0; row < this->field_->Height(); ++row) {
        if (field_->GetCellState(row, col) == CellState::UNSPECIFIED ||
            field_->GetCellState(row, col) == CellState::SHIP) {
          mask[row] = true;
        }
      }
      for (std::size_t start = 0; start <= this->field_->Height() - length;
           ++start) {
        bool valid = true;
        for (std::size_t i = start; i < start + length; ++i) {
          if (!mask[i]) {
            valid = false;
            break;
          }
        }
        if (valid) {
          for (std::size_t i = start; i < start + length; ++i) {
            ++probabilities[i][col];
          }
        }
      }
    }
  }
  for (std::size_t i = 0; i < this->field_->Height(); ++i) {
    for (std::size_t j = 0; j < this->field_->Width(); ++j) {
      if (field_->GetCellState(i, j) == CellState::SHIP) {
        for (auto& [x, y] : this->field_->Neighbours({j, i})) {
          if (this->field_->GetCellState(y, x) == CellState::UNSPECIFIED) {
            probabilities[y][x] = std::numeric_limits<std::int64_t>::max();
          }
        }
        probabilities[i][j] = 0;
      } else if (field_->GetCellState(i, j) == CellState::KILL) {
        for (auto& [x_ship, y_ship] : this->field_->FullShip({j, i})) {
          for (auto& [x, y] : this->field_->Neighbours({x_ship, y_ship})) {
            probabilities[y][x] = 0;
          }
        }
      }
    }
  }

  std::int64_t max_prob = 0;
  Coord best(0, 0);
  for (std::size_t row = 0; row < this->field_->Height(); ++row) {
    for (std::size_t col = 0; col < this->field_->Width(); ++col) {
      if (field_->GetCellState(row, col) == CellState::UNSPECIFIED &&
          probabilities[row][col] > max_prob) {
        max_prob = probabilities[row][col];
        best = Coord(col, row);
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
  this->first_call_ = true;
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
