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

template <Role TRole>
struct is_evil {
  static constexpr bool value = detail::role_in(TRole, detail::evil_sequence{});
};

template <Role TRole>
struct is_good {
  static constexpr bool value = detail::role_in(TRole, detail::good_sequence{});
};

}  // namespace avalon
