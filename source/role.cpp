#include "avalon/role.hpp"
#include <cassert>

namespace avalon {

inline bool is_oberon(Role role) {
  return Role::Oberon == role;
}

Mark mark_as(Role from, Role to) {
  auto as = Mark::Unknown;
  switch (from) {
    case Role::Merlin: {
      if (is_evil(to) && Role::Mordred != to) {
        as = Mark::Evil;
      }
    } break;
    case Role::Mordred:
    case Role::Assassin:
    case Role::Morcana:
    case Role::Minion: {
      if (is_evil(to) && Role::Oberon != to) {
        as = Mark::Evil;
      }
    } break;
    case Role::Percival: {
      if (Role::Merlin == to || Role::Morcana == to)
        as = Mark::MerlinOrMorcana;
    } break;
    default:
      break;
      return as;
  }
  return as;
}

}  // namespace avalon
