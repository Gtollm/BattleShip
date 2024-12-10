#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "game.hpp"
#include "lib/field/field.hpp"
#include "lib/field/matrix_field/matrix_opponent_field.hpp"
#include "lib/field/matrix_field/matrix_player_field.hpp"
#include "lib/strategy/probabilistic_strategy/probabilistic_strategy.hpp"

namespace BattleShipGame {

void Game::Start() {
  if (!was_strated_) {
    std::vector<std::size_t> ships;
    for (auto it : this->num_ships_) {
      ships.push_back(it.value_or(0));
    }
    this->my_field_ =
        std::make_shared<MatrixPlayerField>(*this->width_, *this->height_);
    this->my_field_->SetShips(ships);
    this->my_field_->FillWithShips();
    this->opponent_field_ =
        std::make_shared<MatrixOpponentField>(*this->width_, *this->height_);
    this->opponent_field_->SetShips(ships);
    if (!this->strategy_) {
      auto str =
          std::make_shared<ProbabilisticStrategy>(this->opponent_field_);
      this->AddStrategy("custom", str);
      this->SetStrategy(str);
    } else {
      this->strategy_->SetField(this->opponent_field_);
    }
  }
  this->was_strated_ = true;
  this->stopped_ = false;
}
void Game::Stop() { this->stopped_ = true; }

void Game::SetWidth(std::size_t width) {
  if (!was_strated_) {
    this->width_ = width;
  }
}

void Game::SetHeight(std::size_t height) {
  if (!was_strated_) {
    this->height_ = height;
  }
}

void Game::SetRole(Role role) {
  if (!was_strated_) {
    this->role_ = role;
  }
}

void Game::SetPlayerField(std::size_t width, std::size_t height) {
  this->height_ = height;
  this->width_ = width;
  this->my_field_ = std::make_shared<MatrixPlayerField>(width, height);
}

void Game::SetCellState(std::size_t row, std::size_t col, CellState state) {
  this->my_field_->SetCellState(row, col, state);
}

std::optional<std::size_t> Game::GetWidth() const { return this->width_; }

std::optional<std::size_t> Game::GetHeight() const { return this->height_; }

void Game::SetShips(const std::vector<std::size_t>& ships) {
  for (std::size_t i = 0; i < this->num_ships_.size(); ++i) {
    this->num_ships_[i] = ships[i];
  }
}

void Game::SetShip(std::size_t length, std::size_t ships) {
  this->num_ships_[length - 1] = ships;
}

std::vector<std::optional<std::size_t>> Game::GetShips() const {
  return this->num_ships_;
}

std::optional<std::size_t> Game::GetShip(std::size_t length) const {
  return this->num_ships_[length - 1];
}

const std::unordered_map<std::string, std::shared_ptr<Strategy>>&
Game::GetStrategies() const {
  return this->used_strategies_;
}

void Game::AddStrategy(const std::string& name,
                       std::shared_ptr<Strategy> strategy) {
  this->used_strategies_[name] = strategy;
}

void Game::SetStrategy(std::shared_ptr<Strategy> strategy) {
  this->strategy_ = std::move(strategy);
}

bool Game::GameFinished() const {
  return this->GameLost() || this->GameWon();
}

bool Game::GameWon() const {
  if (!this->GameLost() && this->strategy_->GameWon()) {
    return true;
  }

  return false;
}

bool Game::GameLost() const {
  if (!this->my_field_) {
    return false;
  }
  return !(this->my_field_->ShipsLeft());
}

Coord Game::GetShootingCoord() const {
  return this->strategy_->GetShootingCoords();
}

CellState Game::GetShootingResult(std::size_t row, std::size_t col) const {
  return this->my_field_->GetCellState(row, col);
}

CellState Game::GetShootingResult(Coord coord) const {
  return this->GetShootingResult(coord.y, coord.x);
}

std::shared_ptr<AbstractPlayerField> Game::GetPlayerField() {
  return this->my_field_;
}

std::shared_ptr<AbstractOpponentField> Game::GetOpponentField() {
  return this->opponent_field_;
}

void Game::CreateGame() {
  if (this->role_ == Role::MASTER) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_size(50, 300);
    this->SetHeight(distrib_size(gen));
    this->SetWidth(distrib_size(gen));
    std::uniform_int_distribution<> distrib_count(
        5, (*this->height_) * (*this->width_) / 1000);
    this->SetShip(1, distrib_count(gen));
    this->SetShip(2, distrib_count(gen));
    this->SetShip(3, distrib_count(gen));
    this->SetShip(4, distrib_count(gen));
  } else if (this->role_ == Role::SLAVE) {
  }
}

void Game::SetShootingResult(CellState hit) {
  return this->strategy_->SetShootingResult(hit);
}

void Game::CreateField() {
  this->my_field_ =
      std::make_unique<MatrixPlayerField>(*this->width_, *this->height_);
  this->my_field_->FillWithShips();
}
};  // namespace BattleShipGame
