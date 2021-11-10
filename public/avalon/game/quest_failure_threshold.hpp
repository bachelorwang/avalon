#pragma once
#include "avalon/def.hpp"
#include <utility>

namespace avalon {

template <player_count_t TCount>
constexpr auto quest_failure_threshold(quest_index_t quest)
    -> std::enable_if_t<(TCount >= 7), int32_t> {
  if (quest == 3)
    return 2;
  return 1;
}

template <player_count_t TCount>
constexpr auto quest_failure_threshold(quest_index_t quest)
    -> std::enable_if_t<(TCount < 7), int32_t> {
  return 1;
}

}  // namespace avalon
