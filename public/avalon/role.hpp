#pragma once
#include <cstddef>
#include <utility>

namespace avalon {

enum class Role : char {
  Unknown,
  Merlin,
  Percival,
  Servant,
  Mordred,
  Morcana,
  Assassin,
  Oberon,
  Minion
};

enum class Mark : char { Unknown, Evil, Good, MerlinOrMorcana };

struct Predict {
  Mark mark;
  Role real;
};

template <Role... TRoles>
using role_sequence = std::integer_sequence<Role, TRoles...>;

namespace detail {
using evil_sequence = role_sequence<Role::Mordred,
                                    Role::Morcana,
                                    Role::Assassin,
                                    Role::Minion,
                                    Role::Oberon>;

using good_sequence =
    role_sequence<Role::Merlin, Role::Percival, Role::Servant>;

template <Role... TRoles>
constexpr bool role_in(Role role, std::integer_sequence<Role, TRoles...>) {
  return ((role == TRoles) || ...);
};

}  // namespace detail

constexpr bool is_evil(Role role) {
  return detail::role_in(role, detail::evil_sequence{});
}

constexpr bool is_good(Role role) {
  return detail::role_in(role, detail::good_sequence{});
}

Mark mark_as(Role from, Role to);

}  // namespace avalon
