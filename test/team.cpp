#include <gtest/gtest.h>
#include <avalon/game/team_build.hpp>
using namespace avalon;

TEST(team, valid) {
  Team<5> team;
  team.count = 2;
  team.members[0] = 0;
  team.members[1] = 1;
  ASSERT_TRUE(team.valid(0));
}

TEST(team, invalid_wrong_count) {
  Team<5> team;
  team.count = 3;
  team.members[0] = 0;
  team.members[1] = 1;
  team.members[2] = 2;
  ASSERT_FALSE(team.valid(0));
}

TEST(team, invalid_wrong_member) {
  Team<5> team;
  team.count = 3;
  team.members[0] = 0;
  team.members[1] = 1;
  team.members[2] = 1;
  ASSERT_FALSE(team.valid(0));
  team.members[2] = 5;
  ASSERT_FALSE(team.valid(0));
  team.members[2] = -1;
  ASSERT_FALSE(team.valid(0));
}
