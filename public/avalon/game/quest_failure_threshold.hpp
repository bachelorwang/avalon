#pragma once
#include "avalon/def.hpp"
#include <utility>

namespace avalon {

template <player_count_t TCount>
constexpr auto quest_failure_threshold(round_index_t round)
    -> std::enable_if_t<(TCount >= 7), int32_t> {
  if (round == 3)
    return 2;
  return 1;
}

template <player_count_t TCount>
constexpr auto quest_failure_threshold(round_index_t round)
    -> std::enable_if_t<(TCount < 7), int32_t> {
  return 1;
}

}  // namespace avalon
