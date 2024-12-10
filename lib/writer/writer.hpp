#pragma once

#include <concepts>
#include <fstream>
#include <ostream>
#include <string>
#include <type_traits>

#include "lib/field/field.hpp"

namespace BattleShipGame {
class Writer {
 public:
  virtual ~Writer() = default;

  virtual void OutputCommandResult(const std::string &data) = 0;
};

template <typename Stream>
concept OStreamable = std::derived_from<Stream, std::ostream>;

template <OStreamable Stream>
class TemplatedWriter : public Writer {
 private:
  std::ostream &stream_;

 public:
  explicit TemplatedWriter(std::ostream &stream);

  void OutputCommandResult(const std::string &data) override;
};

using ConsoleWriter = TemplatedWriter<std::ostream>;
using FileWriter = TemplatedWriter<std::ofstream>;

}  // namespace BattleShipGame

#include "writer.tpp"
