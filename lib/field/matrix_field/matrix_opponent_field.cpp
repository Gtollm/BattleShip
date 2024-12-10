#include <cstddef>

#include "matrix_opponent_field.hpp"

namespace BattleShipGame {
MatrixOpponentField::MatrixOpponentField(std::size_t width,
                                         std::size_t height)
    : MatrixField(width, height) {
  for (std::size_t row = 0; row < this->height_; ++row) {
    for (std::size_t col = 0; col < this->width_; ++col) {
      this->data_[row][col] = CellState::UNSPECIFIED;
    }
  }
}

MatrixOpponentField::~MatrixOpponentField() = default;

inline void MatrixOpponentField::ZeroAfterSinked(const Coord &coord) {
  std::vector sinked = this->FullShip(coord);
  for (const auto it : sinked) {
    for (const auto n : this->Neighbours(it)) {
      this->data_[n.y][n.x] = CellState::WATER;
    }
    this->data_[it.y][it.x] = CellState::WATER;
  }
}

inline void MatrixOpponentField::RecordShot(std::size_t x, std::size_t y,
                                            CellState result) {
  if (result != CellState::KILL) {
    this->data_[y][x] = result;
  }
  if (result == CellState::KILL) {
    auto ship = this->FullShip({x, y});
    for (const auto part : ship) {
      this->data_[part.y][part.x] = result;
    }
  }
}

inline void MatrixOpponentField::RecordShot(const Coord &coord,
                                            CellState result) {
  this->RecordShot(coord.x, coord.y, result);
}
}  // namespace BattleShipGame
