#pragma once

#include "lib/field/field.hpp"
#include "lib/matrix/vector_matrix.hpp"
#include "matrix_field.hpp"

namespace BattleShipGame {
class MatrixPlayerField
    : public AbstractPlayerField,
      public MatrixField<CellState, utils::PackedVectorMatrix<CellState> > {
 public:
  MatrixPlayerField(std::size_t width, std::size_t height);

  ~MatrixPlayerField() override;

  bool FillWithShips() override;

 private:
  std::vector<std::vector<std::pair<std::size_t, std::size_t> > > free_rows_;
  std::vector<std::vector<std::pair<std::size_t, std::size_t> > > free_cols_;

  void PlaceHorizontal(std::size_t r, std::size_t c, std::size_t length);

  void PlaceVertical(std::size_t r, std::size_t c, std::size_t length);

  void UpdateFreeRows(std::size_t r, std::size_t c, std::size_t length);

  void UpdateFreeCols(std::size_t c, std::size_t r, std::size_t length);

  static void MergeFreeSegments(
      std::vector<std::pair<std::size_t, std::size_t> > &segments);

  bool CanPlaceHorizontal(std::size_t r, std::size_t c, std::size_t length);

  bool CanPlaceVertical(std::size_t r, std::size_t c, std::size_t length);

  void UndefToWater();
};
}  // namespace BattleShipGame
