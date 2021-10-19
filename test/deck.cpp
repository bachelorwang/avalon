#include <gtest/gtest.h>
#include <avalon/deck.hpp>

using namespace avalon;

TEST(deck, d5) {
  constexpr int8_t player_count = 5;
  auto deck = create_deck<player_count, Role::Merlin, Role::Assassin>();
  ASSERT_EQ(GoodNum<player_count>::value, good_role_count(deck));
  ASSERT_EQ(EvilNum<player_count>::value, evil_role_count(deck));
}
