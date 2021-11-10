#pragma once
#include <cassert>
#include "avalon/game/quest_failure_threshold.hpp"
#include "avalon/game/team_build.hpp"

namespace avalon {

template <player_count_t TCount>
bool team_build_passed(player_count_t approve) {
  assert(approve <= TCount);
  auto disapprove = TCount - approve;
  return approve > disapprove;
}

template <player_count_t TCount>
bool quest_succeed(quest_index_t quest, player_count_t approve) {
  const auto member_count = TeamBuildRequirement<TCount>::value[quest];
  assert(approve <= member_count);
  auto minimum = (member_count - quest_failure_threshold<TCount>(quest));
  return approve > minimum;
}

}  // namespace avalon
