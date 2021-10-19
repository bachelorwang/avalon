#include <gtest/gtest.h>
#include <avalon/role.hpp>
using namespace avalon;

TEST(role, unknown) {
  ASSERT_FALSE(is_good<Role::Unknown>::value);
  ASSERT_FALSE(is_evil<Role::Unknown>::value);
}

TEST(role, evil) {
  ASSERT_FALSE(is_evil<Role::Merlin>::value);
  ASSERT_FALSE(is_evil<Role::Percival>::value);
  ASSERT_FALSE(is_evil<Role::Servant>::value);
  ASSERT_TRUE(is_evil<Role::Mordred>::value);
  ASSERT_TRUE(is_evil<Role::Morcana>::value);
  ASSERT_TRUE(is_evil<Role::Assassin>::value);
  ASSERT_TRUE(is_evil<Role::Oberon>::value);
  ASSERT_TRUE(is_evil<Role::Minion>::value);
}

TEST(role, good) {
  ASSERT_TRUE(is_good<Role::Merlin>::value);
  ASSERT_TRUE(is_good<Role::Percival>::value);
  ASSERT_TRUE(is_good<Role::Servant>::value);
  ASSERT_FALSE(is_good<Role::Mordred>::value);
  ASSERT_FALSE(is_good<Role::Morcana>::value);
  ASSERT_FALSE(is_good<Role::Assassin>::value);
  ASSERT_FALSE(is_good<Role::Oberon>::value);
  ASSERT_FALSE(is_good<Role::Minion>::value);
}
