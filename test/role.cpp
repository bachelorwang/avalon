#include <gtest/gtest.h>
#include <avalon/role.hpp>
using namespace avalon;

TEST(role, unknown) {
  ASSERT_FALSE(is_good(Role::Unknown));
  ASSERT_FALSE(is_evil(Role::Unknown));
}

TEST(role, evil) {
  ASSERT_FALSE(is_evil(Role::Merlin));
  ASSERT_FALSE(is_evil(Role::Percival));
  ASSERT_FALSE(is_evil(Role::Servant));
  ASSERT_TRUE(is_evil(Role::Mordred));
  ASSERT_TRUE(is_evil(Role::Morcana));
  ASSERT_TRUE(is_evil(Role::Assassin));
  ASSERT_TRUE(is_evil(Role::Oberon));
  ASSERT_TRUE(is_evil(Role::Minion));
}

TEST(role, good) {
  ASSERT_TRUE(is_good(Role::Merlin));
  ASSERT_TRUE(is_good(Role::Percival));
  ASSERT_TRUE(is_good(Role::Servant));
  ASSERT_FALSE(is_good(Role::Mordred));
  ASSERT_FALSE(is_good(Role::Morcana));
  ASSERT_FALSE(is_good(Role::Assassin));
  ASSERT_FALSE(is_good(Role::Oberon));
  ASSERT_FALSE(is_good(Role::Minion));
}

template <Role TFrom, Mark TAs, Role TRole>
void mark_as_check_impl() {
  ASSERT_EQ(TAs, mark_as(TFrom, TRole));
}

template <Role TFrom, Mark TAs, Role... TRoles>
void mark_as_check() {
  (mark_as_check_impl<TFrom, TAs, TRoles>(), ...);
}

TEST(role, mark_as_from_minion) {
  mark_as_check<Role::Minion, Mark::Evil, Role::Morcana, Role::Assassin,
                Role::Minion>();
  mark_as_check<Role::Minion, Mark::Unknown, Role::Merlin, Role::Percival,
                Role::Servant>();
  mark_as_check<Role::Minion, Mark::Unknown, Role::Oberon>();
  mark_as_check<Role::Minion, Mark::Evil, Role::Mordred>();
}

TEST(role, mark_as_from_mordred) {
  mark_as_check<Role::Mordred, Mark::Evil, Role::Morcana, Role::Assassin,
                Role::Minion>();
  mark_as_check<Role::Mordred, Mark::Unknown, Role::Merlin, Role::Percival,
                Role::Servant>();
  mark_as_check<Role::Mordred, Mark::Unknown, Role::Oberon>();
}

TEST(role, mark_as_from_assassin) {
  mark_as_check<Role::Assassin, Mark::Evil, Role::Morcana, Role::Minion>();
  mark_as_check<Role::Assassin, Mark::Unknown, Role::Merlin, Role::Percival,
                Role::Servant>();
  mark_as_check<Role::Assassin, Mark::Unknown, Role::Oberon>();
  mark_as_check<Role::Assassin, Mark::Evil, Role::Mordred>();
}

TEST(role, mark_as_from_morcana) {
  mark_as_check<Role::Morcana, Mark::Evil, Role::Assassin, Role::Minion>();
  mark_as_check<Role::Morcana, Mark::Unknown, Role::Merlin, Role::Percival,
                Role::Servant>();
  mark_as_check<Role::Morcana, Mark::Unknown, Role::Oberon>();
  mark_as_check<Role::Morcana, Mark::Evil, Role::Mordred>();
}

TEST(role, mark_as_from_oberon) {
  mark_as_check<Role::Oberon, Mark::Unknown, Role::Morcana, Role::Assassin,
                Role::Minion>();
  mark_as_check<Role::Oberon, Mark::Unknown, Role::Merlin, Role::Percival,
                Role::Servant>();
  mark_as_check<Role::Oberon, Mark::Unknown, Role::Mordred>();
}

TEST(role, mark_as_from_servant) {
  mark_as_check<Role::Servant, Mark::Unknown, Role::Morcana, Role::Assassin,
                Role::Minion>();
  mark_as_check<Role::Servant, Mark::Unknown, Role::Merlin, Role::Percival,
                Role::Servant>();
  mark_as_check<Role::Servant, Mark::Unknown, Role::Oberon>();
  mark_as_check<Role::Servant, Mark::Unknown, Role::Mordred>();
}

TEST(role, mark_as_from_merlin) {
  mark_as_check<Role::Merlin, Mark::Evil, Role::Morcana, Role::Assassin,
                Role::Minion>();
  mark_as_check<Role::Merlin, Mark::Unknown, Role::Percival, Role::Servant>();
  mark_as_check<Role::Merlin, Mark::Evil, Role::Oberon>();
  mark_as_check<Role::Merlin, Mark::Unknown, Role::Mordred>();
}

TEST(role, mark_as_from_percival) {
  mark_as_check<Role::Percival, Mark::Unknown, Role::Assassin, Role::Minion>();
  mark_as_check<Role::Percival, Mark::Unknown, Role::Oberon>();
  mark_as_check<Role::Percival, Mark::Unknown, Role::Mordred>();
  mark_as_check<Role::Percival, Mark::MerlinOrMorcana, Role::Merlin,
                Role::Morcana>();
}
