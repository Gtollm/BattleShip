#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace BattleShipGame {
enum class CellState : std::uint8_t {
  UNSPECIFIED = 0,
  WATER = 1,
  SHIP = 2,
  KILL = 3
};

struct Coord {
  std::size_t x;
  std::size_t y;

  Coord() : x(0), y(0) {}

  Coord(std::size_t x, std::size_t y) : x(x), y(y) {}

  bool operator==(const Coord &other) const {
    return x == other.x && y == other.y;
  }
};

class AbstractField {
 public:
  virtual ~AbstractField() = default;

  [[nodiscard]] virtual CellState GetCellState(std::size_t row,
                                               std::size_t col) const = 0;

  virtual void SetCellState(std::size_t row, std::size_t col,
                            CellState state) = 0;

  virtual std::vector<Coord> Neighbours(Coord coord) const = 0;

  virtual std::vector<Coord> FullShip(Coord coord) const = 0;

  virtual void SetWidth(std::size_t width) = 0;

  virtual void SetHeight(std::size_t height) = 0;

  virtual void SetShips(const std::vector<std::size_t> &ships) = 0;

  [[nodiscard]] virtual std::size_t Width() const = 0;

  [[nodiscard]] virtual std::size_t Height() const = 0;

  [[nodiscard]] virtual const std::vector<std::size_t> &GetShips() const = 0;

  [[nodiscard]] virtual bool ShipsLeft() const = 0;
};

class AbstractPlayerField : public virtual AbstractField {
 public:
  virtual ~AbstractPlayerField() = default;

  virtual bool FillWithShips() = 0;
};

class AbstractOpponentField : public virtual AbstractField {
 public:
  virtual ~AbstractOpponentField() = default;

  virtual void ZeroAfterSinked(const Coord &coord) = 0;

  virtual void RecordShot(std::size_t x, std::size_t y,
                          CellState result) = 0;
  virtual void RecordShot(const Coord &coord, CellState result) = 0;
};
}  // namespace BattleShipGame
