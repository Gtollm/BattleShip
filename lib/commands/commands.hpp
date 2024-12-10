#pragma once

#include <string>

#include "lib/game/game.hpp"

namespace BattleShipGame {
class Command {
 public:
  virtual ~Command() = default;

  [[nodiscard]] virtual std::string Execute() const = 0;
};

class PingCommand final : public Command {
 public:
  PingCommand() = default;

  [[nodiscard]] std::string Execute() const override;
};

class ExitCommand final : public Command {
 public:
  ExitCommand() = default;

  [[nodiscard]] std::string Execute() const override;
};

class CreateCommand final : public Command {
 private:
  Game &game_;
  Role role_;

 public:
  explicit CreateCommand(Game &game, Role role);

  [[nodiscard]] std::string Execute() const override;
};

class StartCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit StartCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class StopCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit StopCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class SetWidthCommand final : public Command {
 private:
  Game &game_;
  std::size_t width_;

 public:
  SetWidthCommand(Game &game, std::size_t width);

  [[nodiscard]] std::string Execute() const override;
};

class GetWidthCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit GetWidthCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class SetHeightCommand final : public Command {
 private:
  Game &game_;
  std::size_t height_;

 public:
  SetHeightCommand(Game &game, std::size_t height);

  [[nodiscard]] std::string Execute() const override;
};

class GetHeightCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit GetHeightCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class SetCountCommand final : public Command {
 private:
  Game &game_;
  std::size_t type_;
  std::size_t amount_;

 public:
  SetCountCommand(Game &game, std::size_t type, std::size_t amount);

  [[nodiscard]] std::string Execute() const override;
};

class GetCountCommand final : public Command {
 private:
  Game &game_;
  std::size_t type_;

 public:
  GetCountCommand(Game &game, std::size_t type_);

  [[nodiscard]] std::string Execute() const override;
};

class SetStrategyCommand final : public Command {
 private:
  Game &game_;
  std::string strategy_;

 public:
  SetStrategyCommand(Game &game, std::string strategy);

  [[nodiscard]] std::string Execute() const override;
};

class ShotXYCommand final : public Command {
 private:
  Game &game_;
  std::size_t x_;
  std::size_t y_;

 public:
  ShotXYCommand(Game &game, std::size_t x, std::size_t y);

  [[nodiscard]] std::string Execute() const override;
};

class ShotCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit ShotCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class SetResultCommand final : public Command {
 private:
  Game &game_;
  std::string result_;

 public:
  SetResultCommand(Game &game, std::string result);

  [[nodiscard]] std::string Execute() const override;
};

class FinishedCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit FinishedCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class WinCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit WinCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class LoseCommand final : public Command {
 private:
  Game &game_;

 public:
  explicit LoseCommand(Game &game);

  [[nodiscard]] std::string Execute() const override;
};

class DumpCommand final : public Command {
 private:
  Game &game_;
  std::string path_;

 public:
  DumpCommand(Game &game, std::string path);

  [[nodiscard]] std::string Execute() const override;
};

class LoadCommand final : public Command {
 private:
  Game &game_;
  std::string path_;

 public:
  LoadCommand(Game &game, std::string path);

  [[nodiscard]] std::string Execute() const override;
};
}  // namespace BattleShipGame
