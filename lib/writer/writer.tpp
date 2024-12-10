#pragma once

#include <string>

#include "lib/field/field.hpp"
#include "writer.hpp"

namespace BattleShipGame {

template <OStreamable Stream>
TemplatedWriter<Stream>::TemplatedWriter(std::ostream &stream)
    : stream_(stream) {}

template <OStreamable Stream>
void TemplatedWriter<Stream>::OutputCommandResult(const std::string &data) {
  this->stream_ << data << std::endl;
}

}  // namespace BattleShipGame
