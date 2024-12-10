#pragma once

#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <vector>

#include "abstract_matrix.hpp"

namespace utils {
enum class Direction { RIGHT, LEFT, DOWN, UP, ALL };

template <typename T>
concept IsVectorLikeContainer =
    requires(T a, typename T::value_type value, std::size_t i) {
      typename T::value_type;
      { a.push_back(value) };
      { a.pop_back() };
      { a.resize(i) };
      { a.reserve(i) };
      { a.begin() };
      { a.end() };
      { a[i] };
    };

template <typename T, template <typename> class Container = std::vector>
  requires IsVectorLikeContainer<Container<T> >
class VectorMatrix : public AbstractMatrix<T> {
 public:
  VectorMatrix();

  VectorMatrix(std::size_t height, std::size_t width);

  VectorMatrix(const std::initializer_list<std::initializer_list<T> > &data);

  explicit VectorMatrix(const Container<Container<T> > &data);

  VectorMatrix &operator=(const VectorMatrix &obj);

  VectorMatrix &operator=(
      const std::initializer_list<std::initializer_list<T> > &data) override;

  ~VectorMatrix() override;

  void clear() override;

  void Extend(Direction dr, std::size_t size = 1);

  Container<T> &operator[](std::size_t row);

  const Container<T> &operator[](std::size_t row) const;

  Container<T> GetRow(std::size_t row) const;

  Container<T> GetCollumn(std::size_t collumn) const;

  T GetElement(std::size_t row, std::size_t collumn) const override;

  std::size_t Width() const override;

  std::size_t Height() const override;

  T at(std::size_t row, std::size_t collumn) override;

  const T at(std::size_t row, std::size_t collumn) const override;

  void set(std::size_t row, std::size_t collumn, const T &val) override;

  void SetElement(std::size_t row, std::size_t collumn,
                  const T &val) override;

  void SetRow(std::size_t row, const T &val) override;

  using iterator = typename std::vector<Container<T> >::iterator;
  using const_iterator = typename std::vector<Container<T> >::const_iterator;

  iterator begin() { return this->data_.begin(); }

  iterator end() { return this->data_.end(); }

  const_iterator begin() const { return this->data_.begin(); }

  const_iterator end() const { return this->data_.end(); }

  const_iterator cbegin() const { return this->data_.cbegin(); }

  const_iterator cend() const { return this->data_.cend(); }

 private:
  std::vector<Container<T> > data_;

  inline void ExtendRight(std::size_t size);

  inline void ExtendLeft(std::size_t size);

  inline void ExtendUp(std::size_t size);

  inline void ExtendDown(std::size_t size);
};
}  // namespace utils

#include "vector_matrix.tpp"
