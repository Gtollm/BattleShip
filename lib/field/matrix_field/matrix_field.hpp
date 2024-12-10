#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>

#include "lib/field/field.hpp"
#include "lib/matrix/abstract_matrix.hpp"
#include "lib/packed_data/packed_matrix.hpp"

namespace BattleShipGame {
template <typename T>
concept MatrixBased = requires {
  std::is_base_of_v<utils::AbstractMatrix<typename T::value_type>, T>;
};

template <typename T, MatrixBased Container>
class MatrixField : public virtual AbstractField {
 public:
  auto operator[](std::size_t row);

  MatrixField(std::size_t width, std::size_t height);

  virtual ~MatrixField();

  [[nodiscard]] CellState GetCellState(std::size_t row,
                                       std::size_t col) const override;

  void SetCellState(std::size_t row, std::size_t col,
                    CellState state) override;

  std::vector<Coord> Neighbours(Coord coord) const override;

  std::vector<Coord> FullShip(Coord coord) const override;

  void SetWidth(std::size_t width) override;

  void SetHeight(std::size_t height) override;

  void SetShips(const std::vector<std::size_t> &ships) override;

  [[nodiscard]] std::size_t Width() const override;

  [[nodiscard]] std::size_t Height() const override;

  [[nodiscard]] const std::vector<std::size_t> &GetShips() const override;

  [[nodiscard]] bool ShipsLeft() const override;

 protected:
  Container data_;
  std::size_t width_;
  std::size_t height_;
  std::vector<std::size_t> ships_;
};

}  // namespace BattleShipGame

#include "matrix_field.tpp"
