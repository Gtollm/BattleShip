#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "lib/packed_data/packed_vector.hpp"

TEST(PackedVectorTestSuite, EmptyPackedVector) {
  utils::PackedVector<std::uint8_t, 2> p_vec{};

  ASSERT_TRUE(p_vec.empty());
}

TEST(PackedVectorTestSuite, NonEmptyPackedVector) {
  utils::PackedVector<std::uint8_t, 2> p_vec{1};

  ASSERT_FALSE(p_vec.empty());
}

TEST(PackedVectorTestSuite, IndexErrorPackedVector) {
  utils::PackedVector<std::uint8_t, 2> p_vec{};

  ASSERT_ANY_THROW(p_vec[0]);
}

TEST(PackedVectorTestSuite, SixValuePackedVector) {
  utils::PackedVector<std::uint8_t, 4> p_vec = {1, 0, 2, 0, 1, 3};

  ASSERT_EQ(p_vec.size(), 6);
  ASSERT_EQ(p_vec[0], 1);
  ASSERT_EQ(p_vec[1], 0);
  ASSERT_EQ(p_vec[2], 2);
  ASSERT_EQ(p_vec[3], 0);
  ASSERT_EQ(p_vec[4], 1);
  ASSERT_EQ(p_vec[5], 3);
}

TEST(PackedVectorTestSuite, IteratorPackedVector) {
  utils::PackedVector<std::uint8_t, 2> p_vec = {1, 0, 2, 0, 1, 3};
  std::vector<std::uint8_t> vec = {1, 0, 2, 0, 1, 3};
  std::size_t i = 0;
  ASSERT_EQ(p_vec.size(), vec.size());
  for (auto it = p_vec.begin(); it != p_vec.end(); ++i, ++it) {
    ASSERT_EQ(*it, vec[i]);
  }
}

TEST(PackedVectorTestSuite, ChangePackedVector) {
  utils::PackedVector<std::uint8_t, 2> p_vec = {1, 0, 2, 0, 1, 3};

  ASSERT_EQ(p_vec.size(), 6);
  ASSERT_EQ(p_vec[0], 1);
  ASSERT_EQ(p_vec[1], 0);
  ASSERT_EQ(p_vec[2], 2);
  p_vec[2] = 1;
  ASSERT_EQ(p_vec[2], 1);
  ASSERT_EQ(p_vec[3], 0);
  ASSERT_EQ(p_vec[4], 1);
  ASSERT_EQ(p_vec[5], 3);
  p_vec[5] = 0;
  ASSERT_EQ(p_vec[5], 0);
}
