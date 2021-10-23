#pragma once
#include "avalon/def.hpp"

namespace avalon {

template <player_count_t>
struct team_build_requirement;

template <>
struct team_build_requirement<5> {
  static constexpr player_count_t max_size = 3;
  static constexpr player_count_t value[round_count] = {2, 3, 2, 3, 3};
};

template <>
struct team_build_requirement<6> {
  static constexpr player_count_t max_size = 4;
  static constexpr player_count_t value[round_count] = {2, 3, 4, 3, 4};
};

template <>
struct team_build_requirement<7> {
  static constexpr player_count_t max_size = 4;
  static constexpr player_count_t value[round_count] = {2, 3, 3, 4, 4};
};

template <>
struct team_build_requirement<8> {
  static constexpr player_count_t max_size = 5;
  static constexpr player_count_t value[round_count] = {3, 4, 4, 5, 5};
};

template <>
struct team_build_requirement<9> {
  static constexpr player_count_t max_size = 5;
  static constexpr player_count_t value[round_count] = {3, 4, 4, 5, 5};
};

template <>
struct team_build_requirement<10> {
  static constexpr player_count_t max_size = 5;
  static constexpr player_count_t value[round_count] = {3, 4, 4, 5, 5};
};

}  // namespace avalon
