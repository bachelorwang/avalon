#pragma once
#include <cstdint>

namespace avalon {

using player_count_t = int32_t;
using player_index_t = player_count_t;
using round_count_t = int32_t;
using round_index_t = round_count_t;
using player_id_t = uint64_t;

constexpr round_count_t round_count = 5;
constexpr player_count_t min_player_count = 7;
constexpr player_count_t max_player_count = 10;
constexpr int32_t max_voting_count = 5;
constexpr round_count_t min_quest_count = 3;

}  // namespace avalon
