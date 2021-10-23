#include <gtest/gtest.h>
#include <avalon/deck.hpp>

using namespace avalon;

template <player_count_t TCount>
void test_deck() {
  auto deck = create_deck<TCount, Role::Merlin, Role::Assassin>();
  ASSERT_EQ(GoodNum<TCount>::value, good_role_count(deck));
  ASSERT_EQ(EvilNum<TCount>::value, evil_role_count(deck));
}

TEST(deck, count) {
  test_deck<5>();
  test_deck<6>();
  test_deck<7>();
  test_deck<8>();
  test_deck<9>();
  test_deck<10>();
}
