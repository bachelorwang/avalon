#pragma once
#include <cstdint>
#include <utility>

namespace avalon {

template <int8_t TPlayerCount>
constexpr auto quest_failure_threshold(int8_t round)
    -> std::enable_if_t<(TPlayerCount >= 7), int8_t> {
  if (round == 3)
    return 2;
  return 1;
}

template <int8_t TPlayerCount>
constexpr auto quest_failure_threshold(int8_t round)
    -> std::enable_if_t<(TPlayerCount < 7), int8_t> {
  return 1;
}

}  // namespace avalon
