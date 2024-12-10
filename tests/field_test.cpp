#include <cstddef>
#include <gtest/gtest.h>

#include "lib/field/matrix_field/matrix_opponent_field.hpp"
#include "lib/field/matrix_field/matrix_player_field.hpp"

TEST(FieldTestSuite, EmptyFieldMatrix) {
  BattleShipGame::MatrixPlayerField field(3, 4);
  for (std::size_t row = 0; row < field.Height(); ++row) {
    for (std::size_t col = 0; col < field.Width(); ++col) {
      ASSERT_EQ(static_cast<BattleShipGame::CellState>(field[row][col]),
                BattleShipGame::CellState::UNSPECIFIED);
    }
  }
}

TEST(FieldTestSuite, SetShipsFieldMatrix) {
  BattleShipGame::MatrixPlayerField field(10, 10);
  std::vector<std::size_t> ships{1, 1, 1, 1};
  field.SetShips(ships);
  ASSERT_TRUE(field.FillWithShips());
  bool res = true;
  for (std::size_t row = 0; row < field.Height(); ++row) {
    for (std::size_t col = 0; col < field.Width(); ++col) {
      res =
          res && (static_cast<BattleShipGame::CellState>(field[row][col]) ==
                  BattleShipGame::CellState::WATER);
    }
  }
  ASSERT_FALSE(res);
}
