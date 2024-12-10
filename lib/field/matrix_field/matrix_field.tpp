#pragma once

#include <iostream>
#include <queue>

#include "lib/field/field.hpp"
#include "matrix_field.hpp"

namespace BattleShipGame {
template <typename T, MatrixBased Container>
MatrixField<T, Container>::MatrixField(std::size_t width, std::size_t height)
    : data_(Container(height, width)), width_(width), height_(height) {}

template <typename T, MatrixBased Container>
auto MatrixField<T, Container>::operator[](std::size_t row) {
  return data_[row];
}

template <typename T, MatrixBased Container>
MatrixField<T, Container>::~MatrixField() = default;

template <typename T, MatrixBased Container>
CellState MatrixField<T, Container>::GetCellState(std::size_t row,
                                                  std::size_t col) const {
  return this->data_[row][col];
}

template <typename T, MatrixBased Container>
void MatrixField<T, Container>::SetCellState(std::size_t row,
                                             std::size_t col,
                                             CellState state) {
  this->data_[row][col] = state;
}

template <typename T, MatrixBased Container>
std::vector<Coord> MatrixField<T, Container>::Neighbours(Coord coord) const {
  std::vector<Coord> neighbours;
  if (coord.y > 0) {
    neighbours.emplace_back(coord.x, coord.y - 1);
  }
  if (coord.y < this->height_ - 1) {
    neighbours.emplace_back(coord.x, coord.y + 1);
  }
  if (coord.x > 0) {
    neighbours.emplace_back(coord.x - 1, coord.y);
  }
  if (coord.x < this->width_ - 1) {
    neighbours.emplace_back(coord.x + 1, coord.y);
  }
  return neighbours;
}
template <typename T, MatrixBased Container>
std::vector<Coord> MatrixField<T, Container>::FullShip(Coord coord) const {
  std::vector<Coord> Ship;
  if (static_cast<CellState>(this->data_[coord.y][coord.x]) !=
          CellState::SHIP &&
      static_cast<CellState>(this->data_[coord.y][coord.x]) !=
          CellState::KILL) {
    return Ship;
  }

  std::queue<Coord> queue;
  queue.push(coord);

  while (!queue.empty()) {
    Coord current = queue.front();
    queue.pop();

    if (std::find(Ship.begin(), Ship.end(), current) == Ship.end()) {
      Ship.push_back(current);

      for (auto [nx, ny] : this->Neighbours(current)) {
        {
          if (nx >= 0 && nx < this->width_ && ny >= 0 &&
              ny < this->height_ &&
              (this->data_[ny][nx] == CellState::SHIP ||
               this->data_[ny][nx] == CellState::KILL) &&
              std::find(Ship.begin(), Ship.end(), Coord{nx, ny}) ==
                  Ship.end()) {
            queue.emplace(nx, ny);
          }
        }
      }
    }
  }
  return Ship;
}

template <typename T, MatrixBased Container>

void MatrixField<T, Container>::SetWidth(std::size_t width) {
  this->width_ = width;
}

template <typename T, MatrixBased Container>

void MatrixField<T, Container>::SetHeight(std::size_t height) {
  this->height_ = height;
}

template <typename T, MatrixBased Container>

void MatrixField<T, Container>::SetShips(
    const std::vector<std::size_t> &ships) {
  this->ships_ = ships;
}

template <typename T, MatrixBased Container>
std::size_t MatrixField<T, Container>::Width() const {
  return this->width_;
}

template <typename T, MatrixBased Container>
std::size_t MatrixField<T, Container>::Height() const {
  return this->height_;
}

template <typename T, MatrixBased Container>

const std::vector<std::size_t> &MatrixField<T, Container>::GetShips() const {
  return this->ships_;
}

template <typename T, MatrixBased Container>
bool MatrixField<T, Container>::ShipsLeft() const {
  for (std::size_t i = 0; i < this->height_; ++i) {
    for (std::size_t j = 0; j < this->width_; ++j) {
      if (this->data_[i][j] == CellState::SHIP ||
          this->data_[i][j] == CellState::KILL) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace BattleShipGame
