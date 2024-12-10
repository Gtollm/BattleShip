#pragma once

#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <vector>

#include "lib/matrix/abstract_matrix.hpp"
#include "lib/matrix/vector_matrix.hpp"
#include "packed_array.hpp"
#include "packed_concepts.hpp"
#include "packed_vector.hpp"

namespace utils {
template <typename T, std::size_t MatrixWidth, std::size_t MatrixHeight,
          std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
class PackedArrayMatrix : public AbstractMatrix<T> {
 public:
  PackedArrayMatrix();

  PackedArrayMatrix(
      const std::initializer_list<std::initializer_list<T> > &data);

  PackedArrayMatrix(const PackedArrayMatrix &data);

  PackedArrayMatrix &operator=(const PackedArrayMatrix &obj);

  PackedArrayMatrix &operator=(
      const std::initializer_list<std::initializer_list<T> > &data) override;

  ~PackedArrayMatrix() override;

  void clear() override;

  PackedArray<T, MatrixWidth, BitWidth> &operator[](std::size_t row);

  const PackedArray<T, MatrixWidth, BitWidth> &operator[](
      std::size_t row) const;

  T GetElement(std::size_t row, std::size_t collumn) const override;

  [[nodiscard]] std::size_t Width() const override;

  [[nodiscard]] std::size_t Height() const override;

  T at(std::size_t row, std::size_t collumn) override;

  const T at(std::size_t row, std::size_t collumn) const override;

  void set(std::size_t row, std::size_t collumn, const T &val) override;

  void SetElement(std::size_t row, std::size_t collumn,
                  const T &val) override;

  void SetRow(std::size_t row, const T &val) override;

  using iterator = typename std::array<PackedArray<T, MatrixWidth, BitWidth>,
                                       MatrixHeight>::iterator;
  using const_iterator =
      typename std::array<PackedArray<T, MatrixWidth, BitWidth>,
                          MatrixHeight>::const_iterator;

  iterator begin() { return this->data_.begin(); }

  iterator end() { return this->data_.end(); }

  const_iterator begin() const { return this->data_.begin(); }

  const_iterator end() const { return this->data_.end(); }

  const_iterator cbegin() const { return this->data_.cbegin(); }

  const_iterator cend() const { return this->data_.cend(); }

 private:
  std::array<PackedArray<T, MatrixWidth, BitWidth>, MatrixHeight> data_;
};

template <typename T>
struct PackedVectorWrapper {
  template <std::size_t BitWidth = 2>
  using type = PackedVector<T, BitWidth>;
};

template <typename T>
using PackedVectorAlias = typename PackedVectorWrapper<T>::template type<>;
template <Valid2Bit T>
using PackedVectorMatrix = VectorMatrix<T, PackedVectorAlias>;
}  // namespace utils

#include "packed_matrix.tpp"
