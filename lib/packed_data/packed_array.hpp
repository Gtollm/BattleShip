
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>

#include "packed_concepts.hpp"

namespace utils {
template <Valid2Bit T, std::size_t Size, std::size_t BitWidth>
  requires ValidBitWidth<BitWidth>
class PackedArray {
  using storage_type = uint8_t;
  static constexpr size_t storage_size = (Size * BitWidth + 7) / 8;
  std::array<storage_type, storage_size> storage{};

 private:
  class iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    iterator(storage_type *s, size_t idx) : storage_(s), index_(idx) {}

    value_type operator*() const {
      size_t storage_index = (index_ * BitWidth) / 8;
      size_t bit_offset = (index_ * BitWidth) % 8;
      return static_cast<T>((storage_[storage_index] >> bit_offset) & mask);
    }

    iterator &operator++() {
      ++index_;
      return *this;
    }

    iterator operator+(difference_type n) const {
      return iterator(storage_, index_ + n);
    }

    bool operator==(const iterator &other) const {
      return index_ == other.index_;
    }

    bool operator!=(const iterator &other) const {
      return !(*this == other);
    }

   private:
    storage_type *storage_;
    size_t index_;
  };

  class const_iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    const_iterator(const storage_type *s, size_t idx)
        : storage_(s), index_(idx) {}

    value_type operator*() const {
      size_t storage_index = (index_ * BitWidth) / 8;
      size_t bit_offset = (index_ * BitWidth) % 8;
      return (storage_[storage_index] >> bit_offset) & mask;
    }

    const_iterator &operator++() {
      ++index_;
      return *this;
    }

    const_iterator operator+(difference_type n) const {
      return const_iterator(storage_, index_ + n);
    }

    bool operator==(const const_iterator &other) const {
      return index_ == other.index_;
    }

    bool operator!=(const const_iterator &other) const {
      return !(*this == other);
    }

   private:
    const storage_type *storage_;
    size_t index_;
  };

 public:
  PackedArray() = default;

  PackedArray(std::initializer_list<T> init) {
    if (init.size() != Size) {
      throw std::invalid_argument(
          "Initializer list size does not match array size.");
    }
    std::fill(storage.begin(), storage.end(), 0);
    size_t i = 0;
    for (auto value : init) {
      set(i, value);
      ++i;
    }
  }

  class reference {
   public:
    using storage_type = uint8_t;

    reference(storage_type &s, size_t bit_offset, size_t bit_width)
        : storage_(s),
          bit_offset_(bit_offset),
          bit_width_(bit_width),
          mask_((static_cast<uint8_t>(1) << bit_width_) - 1) {}

    operator T() const {
      return static_cast<T>((storage_ >> bit_offset_) & mask_);
    }

    reference &operator=(T value) {
      std::uint8_t value_as_uint8 = static_cast<uint8_t>(value);
      value_as_uint8 &= mask_;
      storage_ &= ~(mask_ << bit_offset_);
      storage_ |= (value_as_uint8 << bit_offset_);
      return *this;
    }

   private:
    storage_type &storage_;
    size_t bit_offset_;
    size_t bit_width_;
    uint8_t mask_;
  };

  const T operator[](size_t index) const {
    if (index >= Size) {
      throw std::out_of_range("Index out of range.");
    }
    return extract(index);
  }

  reference operator[](size_t index) {
    if (index >= Size) {
      throw std::out_of_range("Index out of range.");
    }
    size_t storage_index = (index * BitWidth) / 8;
    size_t bit_offset = (index * BitWidth) % 8;
    return reference(storage[storage_index], bit_offset, BitWidth);
  }

  size_t size() const { return Size; }

  bool empty() const { return Size == 0; }

  T front() const { return this->operator[](0); }

  T back() const { return this->operator[](Size - 1); }

  void set(size_t index, T value) {
    if (index >= Size) {
      throw std::out_of_range("Index out of range.");
    }
    value &= mask;
    size_t storage_index = (index * BitWidth) / 8;
    size_t bit_offset = (index * BitWidth) % 8;
    storage[storage_index] &= ~(mask << bit_offset);
    storage[storage_index] |= (value << bit_offset);
  }

  iterator begin() { return iterator(storage.data(), 0); }

  iterator end() { return iterator(storage.data(), Size); }

  const_iterator begin() const { return const_iterator(storage.data(), 0); }

  const_iterator end() const { return const_iterator(storage.data(), Size); }

  const_iterator cbegin() const { return const_iterator(storage.data(), 0); }

  const_iterator cend() const {
    return const_iterator(storage.data(), Size);
  }

  template <Valid2Bit T_oth, std::size_t Size_oth, std::size_t BitWidth_oth>
    requires ValidBitWidth<BitWidth_oth>
  inline bool operator==(
      const PackedArray<T_oth, Size_oth, BitWidth_oth> &other) const {
    if (this->size() != other.size()) {
      return false;
    }
    for (int i = 0; i < Size; ++i) {
      if (this->operator[](i) != other[i]) {
        return false;
      }
    }
    return true;
  }

  void clear() {
    for (std::size_t i = 0; i < Size; ++i) {
      this->operator[](i) = T{};
    }
  }

 private:
  T extract(size_t index) const {
    size_t storage_index = (index * BitWidth) / 8;
    size_t bit_offset = (index * BitWidth) % 8;
    return static_cast<T>((storage[storage_index] >> bit_offset) & mask);
  }

  static constexpr storage_type mask =
      (static_cast<storage_type>(1) << BitWidth) - 1;
};
}  // namespace utils
// namespace utils
