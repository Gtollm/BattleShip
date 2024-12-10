#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#include "packed_concepts.hpp"
#include "packed_matrix.hpp"

namespace utils {
template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArrayMatrix<T, MatrixWidth, MatrixHeight,
                  BitWidth>::PackedArrayMatrix() = default;

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::PackedArrayMatrix(
    const std::initializer_list<std::initializer_list<T> > &data) {
  std::size_t row = 0;
  for (const auto &rowData : data) {
    std::size_t col = 0;
    for (const auto &value : rowData) {
      this->data_[row][col] = value;
      ++col;
    }
    ++row;
  }
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::PackedArrayMatrix(
    const PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth> &data)
    : data_(data) {}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth> &
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::operator=(
    const PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth> &obj) {
  this->clear();
  this->data_ = obj.data_;
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth> &
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::operator=(
    const std::initializer_list<std::initializer_list<T> > &data) {
  this->clear();
  std::size_t row = 0;
  for (const auto &rowData : data) {
    std::size_t col = 0;
    for (const auto &value : rowData) {
      this->data_[row][col] = value;
      ++col;
    }
    ++row;
  }
  return *this;
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArrayMatrix<T, MatrixWidth, MatrixHeight,
                  BitWidth>::~PackedArrayMatrix() = default;

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
PackedArray<T, MatrixWidth, BitWidth> &
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::operator[](
    std::size_t row) {
  if (row >= MatrixHeight) {
    throw std::out_of_range("Index out of range");
  }
  return this->data_[row];
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
const PackedArray<T, MatrixWidth, BitWidth> &
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::operator[](
    std::size_t row) const {
  if (row >= MatrixHeight) {
    throw std::out_of_range("Index out of range");
  }
  return this->data_[row];
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
void PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::clear() {
  for (int i = 0; i < MatrixHeight; ++i) {
    this[i].clear();
  }
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
T PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::GetElement(
    std::size_t row, std::size_t collumn) const {
  return this->data_[row][collumn];
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
std::size_t
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::Width() const {
  return MatrixWidth;
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
std::size_t
PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::Height() const {
  return MatrixHeight;
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
T PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::at(
    std::size_t row, std::size_t collumn) {
  return static_cast<T>(this->data_[row][collumn]);
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
const T PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::at(
    std::size_t row, std::size_t collumn) const {
  return static_cast<T>(this->data_[row][collumn]);
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
void PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::set(
    std::size_t row, std::size_t collumn, const T &val) {
  this->data_[row][collumn] = val;
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
void PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::SetElement(
    std::size_t row, std::size_t collumn, const T &val) {
  this->data_[row][collumn] = val;
}

template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
void PackedArrayMatrix<T, MatrixWidth, MatrixHeight, BitWidth>::SetRow(
    std::size_t row, const T &val) {
  for (std::size_t collumn = 0; collumn < MatrixWidth; ++collumn) {
    this->data_[row][collumn] = val;
  }
}
}  // namespace utils
