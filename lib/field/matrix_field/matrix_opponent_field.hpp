#pragma once

#include "lib/field/field.hpp"
#include "matrix_field.hpp"

namespace BattleShipGame {
class MatrixOpponentField final
    : public AbstractOpponentField,
      public MatrixField<CellState, utils::PackedVectorMatrix<CellState> > {
 public:
  MatrixOpponentField(std::size_t width, std::size_t height);

  ~MatrixOpponentField() override;

  void ZeroAfterSinked(const Coord &coord) override;

  void RecordShot(std::size_t x, std::size_t y, CellState result) override;

  void RecordShot(const Coord &coord, CellState result) override;
};
}  // namespace BattleShipGame
