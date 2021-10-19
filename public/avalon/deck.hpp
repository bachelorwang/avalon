#pragma once
#include <cstdint>
#include "avalon/role.hpp"

namespace avalon {

template <int8_t TCount, typename TMustWith = std::integer_sequence<Role>>
struct Deck {
  static constexpr TMustWith musts = {};
  Role roles[TCount];
};

template <int8_t TCount>
struct EvilNum;

template <>
struct EvilNum<5> {
  static constexpr int8_t value = 2;
};

template <>
struct EvilNum<6> {
  static constexpr int8_t value = 2;
};

template <>
struct EvilNum<7> {
  static constexpr int8_t value = 3;
};

template <>
struct EvilNum<8> {
  static constexpr int8_t value = 3;
};

template <>
struct EvilNum<9> {
  static constexpr int8_t value = 3;
};

template <>
struct EvilNum<10> {
  static constexpr int8_t value = 4;
};

template <int8_t TCount>
struct GoodNum {
  static constexpr int8_t value = TCount - EvilNum<TCount>::value;
};

namespace detail {

template <int8_t TCount, typename TMustWith, Role... TRoles>
int8_t count_role(Deck<TCount, TMustWith> deck,
                  std::integer_sequence<Role, TRoles...> sequence) {
  int8_t count = 0;
  for (size_t i = 0; i < TCount; ++i) {
    auto role = deck.roles[i];
    if (detail::role_in(role, sequence)) {
      ++count;
    }
  }
  return count;
}

template <Role... TRoles, Role... TRoleSet>
constexpr int8_t count_role(std::integer_sequence<Role, TRoles...> sequence,
                            std::integer_sequence<Role, TRoleSet...> set) {
  int8_t count = ((detail::role_in(TRoles, set) ? 1 : 0) + ...);
  return count;
}

}  // namespace detail

template <int8_t TCount, typename TMustWith>
int8_t good_role_count(Deck<TCount, TMustWith> deck) {
  return detail::count_role(deck, detail::good_sequence{});
}

template <int8_t TCount, typename TMustWith>
int8_t evil_role_count(Deck<TCount, TMustWith> deck) {
  return detail::count_role(deck, detail::evil_sequence{});
}

template <int8_t TCount, Role... TMusts>
constexpr Deck<TCount, std::integer_sequence<Role, TMusts...>> create_deck() {
  Deck<TCount, std::integer_sequence<Role, TMusts...>> deck;
  static_assert(sizeof...(TMusts) <= TCount);
  int8_t top = 0;
  ((deck.roles[top++] = TMusts), ...);
  constexpr int8_t ghole =
      GoodNum<TCount>::value -
      detail::count_role(role_sequence<TMusts...>{}, detail::good_sequence{});
  static_assert(ghole >= 0);
  for (int8_t i = 0; i < ghole; ++i) {
    deck.roles[top++] = Role::Servant;
  }
  constexpr int8_t ehole =
      EvilNum<TCount>::value -
      detail::count_role(role_sequence<TMusts...>{}, detail::evil_sequence{});
  static_assert(ehole >= 0);
  for (int8_t i = 0; i < ehole; ++i) {
    deck.roles[top++] = Role::Minion;
  }
  return deck;
}

}  // namespace avalon
