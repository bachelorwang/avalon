#pragma once
#include "avalon/game/quest_failure_threshold.hpp"
#include "avalon/game/team_build_threshold.hpp"

namespace avalon {

template <player_count_t TCount>
constexpr bool team_build_passed(round_index_t round, int32_t approve) {
  return approve >= team_build_threshold<TCount>::value[round];
}

}  // namespace avalon
