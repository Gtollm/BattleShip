#include <cstddef>

#include "matrix_player_field.hpp"

namespace BattleShipGame {
MatrixPlayerField::~MatrixPlayerField() = default;
MatrixPlayerField::MatrixPlayerField(std::size_t width, std::size_t height)
    : MatrixField(width, height) {
  for (std::size_t row = 0; row < this->height_; ++row) {
    for (std::size_t col = 0; col < this->width_; ++col) {
      this->data_[row][col] = CellState::UNSPECIFIED;
    }
  }
}
bool MatrixPlayerField::FillWithShips() {
  this->free_rows_.resize(this->height_);
  for (std::size_t r = 0; r < this->height_; ++r) {
    this->free_rows_[r].emplace_back(0, this->width_ - 1);
  }

  this->free_cols_.resize(this->width_);
  for (std::size_t c = 0; c < this->width_; ++c) {
    this->free_cols_[c].emplace_back(0, this->height_ - 1);
  }

  std::vector<std::pair<std::size_t, std::size_t> > sorted_ships;
  for (std::size_t i = 0; i < this->ships_.size(); ++i) {
    if (this->ships_[i] > 0) {
      sorted_ships.emplace_back(i + 1, this->ships_[i]);
    }
  }
  std::sort(sorted_ships.begin(), sorted_ships.end(),
            [](const std::pair<std::size_t, std::size_t> &a,
               const std::pair<std::size_t, std::size_t> &b) {
              return a.first > b.first;
            });

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0,
                                          std::numeric_limits<int>::max());

  for (const auto &[length, count] : sorted_ships) {
    for (std::size_t c = 0; c < count; ++c) {
      bool placed = false;
      int max_attempts = 10'000;
      while (!placed && max_attempts-- > 0) {
        std::vector<std::pair<std::size_t, std::size_t> >
            possible_horizontal;
        for (std::size_t r = 0; r < this->height_; ++r) {
          for (const auto &[fst, snd] : free_rows_[r]) {
            for (std::size_t start = fst; start + length + 2 <= snd + 1;
                 ++start) {
              possible_horizontal.emplace_back(r, start);
            }
          }
        }

        std::vector<std::pair<std::size_t, std::size_t> > possible_vertical;
        for (std::size_t c = 0; c < this->width_; ++c) {
          for (const auto &[fst, snd] : free_cols_[c]) {
            for (std::size_t start = fst; start + length + 2 <= snd + 1;
                 ++start) {
              possible_vertical.emplace_back(start, c);
            }
          }
        }

        std::size_t total_horizontal = possible_horizontal.size();
        std::size_t total_vertical = possible_vertical.size();
        if (total_horizontal + total_vertical == 0) {
          break;
        }

        std::size_t index =
            distrib(gen) % (total_horizontal + total_vertical);
        if (index < total_horizontal) {
          std::size_t r = possible_horizontal[index].first;
          std::size_t startC = possible_horizontal[index].second;
          if (CanPlaceHorizontal(r, startC, length)) {
            PlaceHorizontal(r, startC, length);
            placed = true;
          }
        } else {
          index -= total_horizontal;
          std::size_t startR = possible_vertical[index].first;
          std::size_t c = possible_vertical[index].second;
          if (CanPlaceVertical(startR, c, length)) {
            PlaceVertical(startR, c, length);
            placed = true;
          }
        }
      }
      if (!placed) {
        return false;
      }
    }
  }
  for (auto &row : free_rows_) {
    row.clear();
    row.shrink_to_fit();
  }
  free_rows_.clear();
  free_rows_.resize(0);
  for (auto &col : free_cols_) {
    col.clear();
    col.shrink_to_fit();
  }
  free_cols_.clear();
  free_cols_.resize(0);
  UndefToWater();
  return true;
}

void MatrixPlayerField::PlaceHorizontal(std::size_t r, std::size_t c,
                                        std::size_t length) {
  for (std::size_t col = c; col < c + length; ++col) {
    this->data_[r][col] = CellState::SHIP;
  }

  for (std::size_t row = r; row <= r + 2; ++row) {
    for (std::size_t col = c; col <= c + length + 1; ++col) {
      if (row - 1 >= 0 && row - 1 < this->height_ && col - 1 >= 0 &&
          col - 1 < this->width_) {
        if (this->data_[row - 1][col - 1] != CellState::SHIP) {
          this->data_[row - 1][col - 1] = CellState::WATER;
        }
      }
    }
  }

  UpdateFreeRows(r, c, length);
  for (std::size_t col = c - 1; col <= c + length; ++col) {
    if (col >= 0 && col < this->width_) {
      UpdateFreeCols(col, r, length);
    }
  }
}

void MatrixPlayerField::PlaceVertical(std::size_t r, std::size_t c,
                                      std::size_t length) {
  for (std::size_t row = r; row < r + length; ++row) {
    this->data_[row][c] = CellState::SHIP;
  }

  for (std::int64_t row = r; row <= r + length + 1; ++row) {
    for (std::int64_t col = c; col <= c + 2; ++col) {
      if (row - 1 >= 0 && row - 1 < this->height_ && col - 1 >= 0 &&
          col - 1 < this->width_) {
        if (this->data_[row - 1][col - 1] != CellState::SHIP) {
          this->data_[row - 1][col - 1] = CellState::WATER;
        }
      }
    }
  }

  UpdateFreeCols(c, r, length);
  for (std::size_t row = r; row <= r + length + 1; ++row) {
    if (row - 1 >= 0 && row - 1 < this->height_) {
      UpdateFreeRows(row - 1, c, length);
    }
  }
}

void MatrixPlayerField::UpdateFreeRows(std::size_t r, std::size_t c,
                                       std::size_t length) {
  std::vector<std::pair<std::size_t, std::size_t> > new_segments;
  for (const auto &seg : free_rows_[r]) {
    if (seg.second < c - 1 || seg.first > c + length) {
      new_segments.push_back(seg);
    } else {
      if (seg.first < c - 1) {
        new_segments.emplace_back(seg.first, c - 2);
      }
      if (seg.second > c + length) {
        new_segments.emplace_back(c + length + 1, seg.second);
      }
    }
  }
  MergeFreeSegments(new_segments);
  free_rows_[r] = std::move(new_segments);
}

void MatrixPlayerField::UpdateFreeCols(std::size_t c, std::size_t r,
                                       std::size_t length) {
  std::vector<std::pair<std::size_t, std::size_t> > new_segments;
  for (const auto &seg : free_cols_[c]) {
    if (seg.second < r - 1 || seg.first > r + length) {
      new_segments.push_back(seg);
    } else {
      if (seg.first < r - 1) {
        new_segments.emplace_back(seg.first, r - 2);
      }
      if (seg.second > r + length) {
        new_segments.emplace_back(r + length + 1, seg.second);
      }
    }
  }
  MergeFreeSegments(new_segments);
  free_cols_[c] = std::move(new_segments);
}

void MatrixPlayerField::MergeFreeSegments(
    std::vector<std::pair<std::size_t, std::size_t> > &segments) {
  if (segments.empty()) return;

  std::sort(segments.begin(), segments.end());
  std::vector<std::pair<std::size_t, std::size_t> > merged;
  auto current = segments[0];
  for (size_t i = 1; i < segments.size(); ++i) {
    if (segments[i].first <= current.second + 1) {
      current.second = std::max(current.second, segments[i].second);
    } else {
      merged.push_back(current);
      current = segments[i];
    }
  }
  merged.push_back(current);
  segments = std::move(merged);
}

bool MatrixPlayerField::CanPlaceHorizontal(std::size_t r, std::size_t c,
                                           std::size_t length) {
  if (c + length > this->width_) {
    return false;
  }
  for (std::size_t row = r; row <= r + 2; ++row) {
    for (std::size_t col = c; col <= c + length + 1; ++col) {
      if (row - 1 >= 0 && row - 1 < this->height_ && col - 1 >= 0 &&
          col - 1 < this->width_) {
        if (this->data_[row - 1][col - 1] == CellState::SHIP ||
            this->data_[row - 1][col - 1] == CellState::WATER) {
          return false;
        }
      }
    }
  }
  return true;
}

bool MatrixPlayerField::CanPlaceVertical(std::size_t r, std::size_t c,
                                         std::size_t length) {
  if (r + length > this->height_) {
    return false;
  }
  for (std::size_t row = r; row <= r + length + 1; ++row) {
    for (std::size_t col = c; col <= c + 2; ++col) {
      if (row - 1 >= 0 && row - 1 < this->height_ && col - 1 >= 0 &&
          col - 1 < this->width_) {
        if (this->data_[row - 1][col - 1] == CellState::SHIP ||
            this->data_[row - 1][col - 1] == CellState::WATER) {
          return false;
        }
      }
    }
  }
  return true;
}

void MatrixPlayerField::UndefToWater() {
  for (std::size_t y = 0; y < this->height_; ++y) {
    for (std::size_t x = 0; x < this->width_; ++x) {
      if (this->data_[y][x] == CellState::UNSPECIFIED) {
        this->data_[y][x] = CellState::WATER;
      }
    }
  }
}
}  // namespace BattleShipGame
