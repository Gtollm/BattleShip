#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "lib/packed_data/packed_matrix.hpp"
#include "lib/packed_data/packed_vector.hpp"

TEST(PackedVectorMatrixTestSuite, NonEmptyPackedVectorMatrix) {
  utils::PackedVectorMatrix<std::uint8_t> p_matx = {{1}};
  std::cout << "KKK" << std::endl;
  ASSERT_EQ(p_matx[0][0], 1);
}

TEST(PackedVectorMatrixTestSuite, IndexErrorPackedVectorMatrix) {
  utils::PackedVectorMatrix<std::uint8_t> p_matx{};

  ASSERT_ANY_THROW(p_matx[0]);
}

TEST(PackedVectorMatrixTestSuite, NineValuePackedVectorMatrix) {
  utils::PackedVectorMatrix<std::uint8_t> p_matx = {
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
