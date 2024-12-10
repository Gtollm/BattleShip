#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "lib/packed_data/packed_array.hpp"
#include "lib/packed_data/packed_matrix.hpp"

TEST(PackedArrayMatrixTestSuite, NonEmptyPackedArrayMatrix) {
  utils::PackedArrayMatrix<std::uint8_t, 4, 4, 2> p_matx = {{1}};

  ASSERT_EQ(p_matx[0][0], 1);
}

TEST(PackedArrayMatrixTestSuite, IndexErrorPackedArrayMatrix) {
  utils::PackedArrayMatrix<std::uint8_t, 0, 0, 4> p_matx;

  ASSERT_ANY_THROW(p_matx[0]);
}

TEST(PackedArrayMatrixTestSuite, NineValuePackedArrayMatrix) {
  utils::PackedArrayMatrix<std::uint8_t, 3, 3, 4> p_matx = {
      {3, 1, 0}, {0, 1, 2}, {2, 1, 3}};

  ASSERT_EQ(p_matx[0][0], 3);
  ASSERT_EQ(p_matx[0][1], 1);
  ASSERT_EQ(p_matx[0][2], 0);
  ASSERT_EQ(p_matx[1][0], 0);
  ASSERT_EQ(p_matx[1][1], 1);
  ASSERT_EQ(p_matx[1][2], 2);
  ASSERT_EQ(p_matx[2][0], 2);
  ASSERT_EQ(p_matx[2][1], 1);
  ASSERT_EQ(p_matx[2][2], 3);
}

TEST(PackedArrayMatrixTestSuite, GetRowOfPackedArrayMatrix) {
  utils::PackedArrayMatrix<std::uint8_t, 3, 2, 4> p_matx = {
      {3, 1, 0}, {0, 1, 2}, {2, 1, 3}};
  utils::PackedArray<std::uint8_t, 3, 2> p_arr = {3, 1, 0};

  ASSERT_EQ(p_matx[0], p_arr);
}
