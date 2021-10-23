#include <gtest/gtest.h>
#include <avalon/game.hpp>

using namespace avalon;

TEST(game, quest_failure_threshold) {
  ASSERT_EQ(1, quest_failure_threshold<5>(0));
  ASSERT_EQ(1, quest_failure_threshold<7>(0));
  ASSERT_EQ(1, quest_failure_threshold<6>(1));
  ASSERT_EQ(1, quest_failure_threshold<8>(1));
  ASSERT_EQ(1, quest_failure_threshold<7>(2));
  ASSERT_EQ(1, quest_failure_threshold<9>(2));
  ASSERT_EQ(1, quest_failure_threshold<8>(4));
  ASSERT_EQ(1, quest_failure_threshold<10>(4));
}

TEST(game, quest_failure_threshold_t4_player_lt_7) {
  ASSERT_EQ(1, quest_failure_threshold<5>(3));
  ASSERT_EQ(1, quest_failure_threshold<6>(3));
}

TEST(game, quest_failure_threshold_t4_player_ge_7) {
  ASSERT_EQ(2, quest_failure_threshold<7>(3));
  ASSERT_EQ(2, quest_failure_threshold<8>(3));
  ASSERT_EQ(2, quest_failure_threshold<9>(3));
  ASSERT_EQ(2, quest_failure_threshold<10>(3));
}
