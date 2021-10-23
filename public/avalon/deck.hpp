#pragma once
#include <cstdint>
#include "avalon/def.hpp"
#include "avalon/role.hpp"

namespace avalon {

template <player_count_t TCount, typename TMustWith = std::integer_sequence<Role>>
struct Deck {
  static constexpr TMustWith musts = {};
  Role roles[TCount];
};

template <player_count_t TCount>
struct EvilNum;

template <>
struct EvilNum<5> {
  static constexpr player_count_t value = 2;
};

template <>
struct EvilNum<6> {
  static constexpr player_count_t value = 2;
};

template <>
struct EvilNum<7> {
  static constexpr player_count_t value = 3;
};

template <>
struct EvilNum<8> {
  static constexpr player_count_t value = 3;
};

template <>
struct EvilNum<9> {
  static constexpr player_count_t value = 3;
};

template <>
struct EvilNum<10> {
  static constexpr player_count_t value = 4;
};

template <player_count_t TCount>
struct GoodNum {
  static constexpr player_count_t value = TCount - EvilNum<TCount>::value;
};

namespace detail {

template <player_count_t TCount, typename TMustWith, Role... TRoles>
player_count_t count_role(Deck<TCount, TMustWith> deck,
                  std::integer_sequence<Role, TRoles...> sequence) {
  player_count_t count = 0;
  for (size_t i = 0; i < TCount; ++i) {
    auto role = deck.roles[i];
    if (detail::role_in(role, sequence)) {
      ++count;
    }
  }
  return count;
}

template <Role... TRoles, Role... TRoleSet>
constexpr player_count_t count_role(std::integer_sequence<Role, TRoles...> sequence,
                            std::integer_sequence<Role, TRoleSet...> set) {
  player_count_t count = ((detail::role_in(TRoles, set) ? 1 : 0) + ...);
  return count;
}

}  // namespace detail

template <player_count_t TCount, typename TMustWith>
player_count_t good_role_count(Deck<TCount, TMustWith> deck) {
  return detail::count_role(deck, detail::good_sequence{});
}

template <player_count_t TCount, typename TMustWith>
player_count_t evil_role_count(Deck<TCount, TMustWith> deck) {
  return detail::count_role(deck, detail::evil_sequence{});
}

template <player_count_t TCount, Role... TMusts>
constexpr Deck<TCount, std::integer_sequence<Role, TMusts...>> create_deck() {
  Deck<TCount, std::integer_sequence<Role, TMusts...>> deck;
  static_assert(sizeof...(TMusts) <= TCount);
  player_count_t top = 0;
  ((deck.roles[top++] = TMusts), ...);
  constexpr player_count_t ghole =
      GoodNum<TCount>::value -
      detail::count_role(role_sequence<TMusts...>{}, detail::good_sequence{});
  static_assert(ghole >= 0);
  for (player_count_t i = 0; i < ghole; ++i) {
    deck.roles[top++] = Role::Servant;
  }
  constexpr player_count_t ehole =
      EvilNum<TCount>::value -
      detail::count_role(role_sequence<TMusts...>{}, detail::evil_sequence{});
  static_assert(ehole >= 0);
  for (player_count_t i = 0; i < ehole; ++i) {
    deck.roles[top++] = Role::Minion;
  }
  return deck;
}

}  // namespace avalon
