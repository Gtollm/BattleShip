#pragma once

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace utils {
template <typename T, std::size_t BitWidth>
class PackedVector {
 public:
  using storage_type = uint8_t;
  using size_type = std::size_t;
  using value_type = T;

 private:
  static constexpr storage_type mask =
      ((static_cast<storage_type>(1) << BitWidth) - 1);
  std::vector<storage_type> storage;
  size_type element_size;

  static size_type calculate_storage_size(size_type size) {
    return (size * BitWidth + 7) / 8;
  }

 public:
  explicit PackedVector(size_type size = 0)
      : element_size(size), storage(calculate_storage_size(size), 0) {}

  PackedVector(size_type size, T value)
      : element_size(size), storage(calculate_storage_size(size), 0) {
    for (size_type i = 0; i < size; ++i) {
      set(i, value);
    }
  }

  PackedVector(std::initializer_list<T> init)
      : element_size(init.size()),
        storage(calculate_storage_size(init.size()), 0) {
    size_type i = 0;
    for (auto value : init) {
      set(i, value);
      ++i;
    }
  }

  void resize(size_type new_size, T value = T{}) {
    if (new_size == element_size) {
      return;
    }

    size_type new_storage_size = calculate_storage_size(new_size);
    storage.resize(new_storage_size);
    size_type ol = element_size;
    element_size = new_size;
    if (new_size > ol) {
      for (size_type i = ol; i < new_size; ++i) {
        set(i, value);
      }
    }
  }

  void reserve(size_type new_capacity) {
    size_type required_size = calculate_storage_size(new_capacity);
    storage.reserve(required_size);
  }

  void push_back(T value) {
    size_type current_storage_bits = storage.size() * 8;
    size_type required_bits = (element_size + 1) * BitWidth;

    if (required_bits > current_storage_bits) {
      size_type new_storage_size = calculate_storage_size(element_size + 1);
      storage.resize(new_storage_size);
    }

    set(element_size, value);
    ++element_size;
  }

  void pop_back() {
    if (element_size > 0) {
      --element_size;
    }
  }

  void shrink_to_fit() {
    size_type required_size = calculate_storage_size(element_size);
    storage.resize(required_size);
  }

  class reference {
   public:
    using storage_type = uint8_t;

    reference(storage_type &s, size_type bit_offset, size_type bit_width)
        : storage_(s),
          bit_offset_(bit_offset),
          bit_width_(bit_width),
          mask_((static_cast<storage_type>(1) << bit_width_) - 1) {}

    operator T() const {
      storage_type value = (storage_ >> bit_offset_) & mask_;
      return static_cast<T>(value);
    }

    reference &operator=(T value) {
      storage_type value_as_uint8 = static_cast<storage_type>(value);
      value_as_uint8 &= mask_;
      storage_ &= ~(mask_ << bit_offset_);
      storage_ |= (value_as_uint8 << bit_offset_);
      return *this;
    }

   private:
    storage_type &storage_;
    size_type bit_offset_;
    size_type bit_width_;
    storage_type mask_;
  };

  reference operator[](size_type index) {
    if (index >= element_size) {
      throw std::out_of_range("Index out of range.");
    }
    size_type storage_index = (index * BitWidth) / 8;
    size_type bit_offset = (index * BitWidth) % 8;
    return reference(storage[storage_index], bit_offset, BitWidth);
  }

  T operator[](size_type index) const {
    if (index >= element_size) {
      throw std::out_of_range("Index out of range.");
    }
    size_type storage_index = (index * BitWidth) / 8;
    size_type bit_offset = (index * BitWidth) % 8;
    return static_cast<T>((storage[storage_index] >> bit_offset) & mask);
  }

  void set(size_type index, T value) {
    if (index >= element_size) {
      throw std::out_of_range("Index out of range.");
    }

    storage_type masked_value = static_cast<storage_type>(value) & mask;

    size_type storage_index = (index * BitWidth) / 8;
    size_type bit_offset = (index * BitWidth) % 8;

    storage[storage_index] &= ~(mask << bit_offset);
    storage[storage_index] |= (masked_value << bit_offset);
  }

  size_type size() const { return element_size; }

  bool empty() const { return element_size == 0; }

  void clear() {
    storage.clear();
    element_size = 0;
  }

  class iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    iterator(std::vector<storage_type> &s, size_type idx, size_type bw)
        : storage(s), index(idx), bit_width(bw) {}

    value_type operator*() const { return extract(); }

    iterator &operator++() {
      ++index;
      return *this;
    }

    iterator operator+(difference_type n) const {
      return iterator(storage, index + n, bit_width);
    }

    bool operator==(const iterator &other) const {
      return index == other.index;
    }

    bool operator!=(const iterator &other) const {
      return !(*this == other);
    }

   private:
    value_type extract() const {
      size_type storage_index = (index * bit_width) / 8;
      size_type bit_offset = (index * bit_width) % 8;
      return static_cast<value_type>((storage[storage_index] >> bit_offset) &
                                     mask);
    }

    std::vector<storage_type> &storage;
    size_type index;
    size_type bit_width;
    static constexpr storage_type mask =
        ((static_cast<storage_type>(1) << BitWidth) - 1);
  };

  iterator end() { return iterator(storage, element_size, BitWidth); }

  iterator begin() { return iterator(storage, 0, BitWidth); }

  class const_iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    const_iterator(const std::vector<storage_type> &s, size_type idx,
                   size_type bw)
        : storage(s), index(idx), bit_width(bw) {}

    value_type operator*() const { return extract(); }

    const_iterator &operator++() {
      ++index;
      return *this;
    }

    const_iterator operator+(difference_type n) const {
      return const_iterator(storage, index + n, bit_width);
    }

    bool operator==(const const_iterator &other) const {
      return index == other.index;
    }

    bool operator!=(const const_iterator &other) const {
      return !(*this == other);
    }

   private:
    value_type extract() const {
      size_type storage_index = (index * bit_width) / 8;
      size_type bit_offset = (index * bit_width) % 8;
      return static_cast<value_type>((storage[storage_index] >> bit_offset) &
                                     mask);
    }

    const std::vector<storage_type> &storage;
    size_type index;
    size_type bit_width;
    static constexpr storage_type mask =
        ((static_cast<storage_type>(1) << BitWidth) - 1);
  };

  const_iterator cbegin() const {
    return const_iterator(storage, 0, BitWidth);
  }

  const_iterator cend() const {
    return const_iterator(storage, element_size, BitWidth);
  }
};
}  // namespace utils
