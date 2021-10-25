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

TEST(ballot_box, reset) {
  constexpr player_count_t player_count = 5;
  BallotBox<player_count> box;
  box.reset();
  for (player_index_t i = 0; i < player_count; ++i) {
    ASSERT_EQ(Ballot::Undecided, box.ballot(i));
  }
  ASSERT_EQ(0, box.voted());
}

TEST(ballot_box, vote) {
  constexpr player_count_t player_count = 5;
  BallotBox<player_count> box;
  box.reset();
  box.vote(0, Ballot::Approve);
  EXPECT_EQ(Ballot::Approve, box.ballot(0));
  EXPECT_EQ(1, box.voted());

  box.vote(1, Ballot::Approve);
  EXPECT_EQ(Ballot::Approve, box.ballot(1));
  EXPECT_EQ(2, box.voted());

  box.vote(4, Ballot::Disapprove);
  EXPECT_EQ(Ballot::Disapprove, box.ballot(4));
  EXPECT_EQ(3, box.voted());
}

TEST(ballot_box, vote_undecided) {
  constexpr player_count_t player_count = 5;
  BallotBox<player_count> box;
  box.reset();
  box.vote(0, Ballot::Approve);
  EXPECT_EQ(Ballot::Approve, box.ballot(0));
  EXPECT_EQ(1, box.voted());

  box.vote(1, Ballot::Undecided);
  EXPECT_EQ(Ballot::Undecided, box.ballot(1));
  EXPECT_EQ(1, box.voted());

  box.vote(0, Ballot::Undecided);
  EXPECT_EQ(0, box.voted());
}
