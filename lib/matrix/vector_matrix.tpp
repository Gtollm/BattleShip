#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <vector>

#include "vector_matrix.hpp"

namespace utils {
template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container>::VectorMatrix() {}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container>::VectorMatrix(std::size_t height,
                                         std::size_t width) {
  this->data_.resize(height);
  for (std::size_t y = 0; y < height; ++y) {
    this->data_[y].resize(width);
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container>::VectorMatrix(
    const std::initializer_list<std::initializer_list<T> > &data) {
  std::size_t row = 0;
  this->data_.resize(data.size(), Container<T>());
  for (const auto &row_data : data) {
    std::size_t col = 0;
    this->data_[row].resize(row_data.size());
    for (const auto &value : row_data) {
      this->data_[row][col] = value;
      ++col;
    }
    ++row;
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container>::VectorMatrix(
    const Container<Container<T> > &data)
    : data_(data) {}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container> &VectorMatrix<T, Container>::operator=(
    const VectorMatrix<T, Container> &obj) {
  this->clear();
  this->data_ = obj.data_;
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container> &VectorMatrix<T, Container>::operator=(
    const std::initializer_list<std::initializer_list<T> > &data) {
  std::size_t row = 0;
  this->data_.resize(data.size());
  for (const auto &row_data : data) {
    std::size_t col = 0;
    this->data_[row].resize(row_data.size());
    for (const auto &value : row_data) {
      this->data_[row][col] = value;
      ++col;
    }
    ++row;
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
VectorMatrix<T, Container>::~VectorMatrix() {}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
void VectorMatrix<T, Container>::Extend(Direction dr, std::size_t size) {
  switch (dr) {
    case Direction::DOWN:
      ExtendDown(size);
      break;
    case Direction::UP:
      ExtendUp(size);
      break;
    case Direction::RIGHT:
      ExtendRight(size);
      break;
    case Direction::LEFT:
      ExtendLeft(size);
      break;
    case Direction::ALL:
      ExtendLeft(size);
      ExtendRight(size);
      ExtendDown(size);
      ExtendUp(size);
      break;
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
Container<T> &VectorMatrix<T, Container>::operator[](std::size_t row) {
  if (row >= this->data_.size()) {
    throw std::out_of_range("Index out of range");
  }
  return this->data_[row];
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
const Container<T> &VectorMatrix<T, Container>::operator[](
    std::size_t row) const {
  if (row >= this->data_.size()) {
    throw std::out_of_range("Index out of range");
  }
  return this->data_[row];
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
void VectorMatrix<T, Container>::clear() {
  this->data_.clear();
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
Container<T> VectorMatrix<T, Container>::GetRow(std::size_t row) const {
  return this->data_[row];
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
Container<T> VectorMatrix<T, Container>::GetCollumn(
    std::size_t collumn) const {
  Container<T> res;
  for (std::size_t j = 0; j < this->data_[collumn].size(); ++j) {
    res[j] = this->data_[j][collumn];
  }
  return res;
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
T VectorMatrix<T, Container>::GetElement(std::size_t row,
                                         std::size_t collumn) const {
  return this->data_[row][collumn];
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
std::size_t VectorMatrix<T, Container>::Width() const {
  if (!this->data_.empty()) {
    return this->data_[0].size();
  }
  return 0;
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
std::size_t VectorMatrix<T, Container>::Height() const {
  return this->data_.size();
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
T VectorMatrix<T, Container>::at(std::size_t row, std::size_t collumn) {
  return this->data_[row][collumn];
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
const T VectorMatrix<T, Container>::at(std::size_t row,
                                       std::size_t collumn) const {
  return this->data_[row][collumn];
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
void VectorMatrix<T, Container>::set(std::size_t row, std::size_t collumn,
                                     const T &val) {
  this->data_[row][collumn] = val;
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
void VectorMatrix<T, Container>::SetElement(std::size_t row,
                                            std::size_t collumn,
                                            const T &val) {
  this->data_[row][collumn] = val;
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
void VectorMatrix<T, Container>::SetRow(std::size_t row, const T &val) {
  for (std::size_t collumn = 0; collumn < this->data_[0].size(); ++collumn) {
    this->data_[row][collumn] = val;
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
inline void VectorMatrix<T, Container>::ExtendRight(std::size_t size) {
  for (int i = 0; i < this->data_.size(); ++i) {
    this->data_[i].resize(this->data_[i].size() + size);
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
inline void VectorMatrix<T, Container>::ExtendUp(std::size_t size) {
  this->data_.resize(
      this->data_.size() + size,
      Container<T>(this->data_.size() > 0 ? this->data_[0].size() : 0));
  std::rotate(this->data_.begin(), this->data_.end() - size,
              this->data_.end());
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
inline void VectorMatrix<T, Container>::ExtendLeft(std::size_t size) {
  for (int i = 0; i < this->data_.size(); ++i) {
    this->data_[i].resize(this->data_[i].size() + size);
    std::rotate(this->data_[i].begin(), this->data_[i].end() - size,
                this->data_[i].end());
  }
}

template <typename T, template <typename> class Container>
  requires IsVectorLikeContainer<Container<T> >
inline void VectorMatrix<T, Container>::ExtendDown(std::size_t size) {
  this->data_.resize(
      this->data_.size() + size,
      Container<T>(this->data_.size() > 0 ? this->data_[0].size() : 0));
}
}  // namespace utils
