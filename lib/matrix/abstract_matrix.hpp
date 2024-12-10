#pragma once

#include <cstddef>
#include <initializer_list>

namespace utils {
template <typename T>
class AbstractMatrix {
 public:
  using value_type = T;

  virtual AbstractMatrix &operator=(
      const std::initializer_list<std::initializer_list<T> > &data) = 0;

  virtual ~AbstractMatrix() = default;

  virtual void clear() = 0;

  virtual T GetElement(std::size_t row, std::size_t collumn) const = 0;

  virtual T at(std::size_t row, std::size_t collumn) = 0;

  virtual const T at(std::size_t row, std::size_t collumn) const = 0;

  virtual void set(std::size_t row, std::size_t collumn, const T &val) = 0;

  virtual void SetElement(std::size_t row, std::size_t collumn,
                          const T &val) = 0;

  virtual void SetRow(std::size_t row, const T &val) = 0;

  virtual std::size_t Width() const = 0;

  virtual std::size_t Height() const = 0;
};
}  // namespace utils
