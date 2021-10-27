#include <gtest/gtest.h>
#include <avalon/game.hpp>
#include <avalon/game/game_status.hpp>

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

TEST(game, team_build) {
  ASSERT_TRUE(team_build_passed<5>(5));
  ASSERT_TRUE(team_build_passed<5>(4));
  ASSERT_TRUE(team_build_passed<5>(3));
  ASSERT_FALSE(team_build_passed<5>(2));

  ASSERT_TRUE(team_build_passed<6>(4));
  ASSERT_FALSE(team_build_passed<6>(3));
}

TEST(game, quest_succeed) {
  ASSERT_TRUE(quest_succeed<7>(0, 2));
  ASSERT_FALSE(quest_succeed<7>(0, 1));
  ASSERT_TRUE(quest_succeed<7>(1, 3));
  ASSERT_FALSE(quest_succeed<7>(1, 2));
  ASSERT_TRUE(quest_succeed<7>(2, 3));
  ASSERT_FALSE(quest_succeed<7>(2, 2));
  ASSERT_TRUE(quest_succeed<7>(3, 4));
  ASSERT_TRUE(quest_succeed<7>(3, 3));
  ASSERT_FALSE(quest_succeed<7>(3, 2));
  ASSERT_TRUE(quest_succeed<7>(4, 4));
  ASSERT_FALSE(quest_succeed<7>(4, 3));
}

template <player_count_t TCount>
GameStatus<TCount> create_game_auto(round_index_t round,
                                    player_index_t merlin) {
  GameStatus<TCount> status;
  status.setup(merlin);

  Team<5> team;
  for (player_index_t i = 0; i < team.max_team_size; ++i) {
    team.members[i] = i;
  }

  bool success = true;
  for (round_index_t i = 0; i < round_count; ++i) {
    status.start_next_round();
    team.count = status.team_build_requirement[i];
    status.assign_team(team);
    for (player_index_t p = 0; p < TCount; ++p) {
      status.vote(p, Ballot::Approve);
    }
    status.end_vote();
    auto q = success ? QuestCard::Success : QuestCard::Fail;
    for (player_index_t p = 0; p < team.count; ++p) {
      status.decide(team.members[p], q);
    }
    status.end_quest();
    success = !success;
    std::cout << i << std::endl;
  }

  return status;
}

TEST(game, merlin) {
  GameStatus<5> status;
  status.setup(3);
  EXPECT_EQ(3, status.merlin());
}

TEST(game, team_build_failed) {
  GameStatus<5> status;
  status.setup(1);
  status.start_next_round();
  Team<5> team;
  team.count = status.team_build_requirement[status.round()];
  team.members[0] = 0;
  team.members[1] = 1;
  status.assign_team(team);
  status.vote(0, Ballot::Approve);
  status.vote(1, Ballot::Approve);
  status.end_vote();
  EXPECT_EQ(1, status.voted());
  EXPECT_EQ(GamePhase::RoundEnded, status.phase());
}

TEST(game, full_progress) {
  GameStatus<5> status;
  status.setup(2);
  status.start_next_round();
  EXPECT_EQ(0, status.round());
  EXPECT_EQ(0, status.voted());
  EXPECT_EQ(0, status.succeed_quest_count());
  EXPECT_EQ(GamePhase::TeamBuilding, status.phase());
  Team<5> team;
  team.count = status.team_build_requirement[status.round()];
  team.members[0] = 1;
  team.members[1] = 3;
  ASSERT_TRUE(team.valid(0));
  status.assign_team(team);
  EXPECT_EQ(GamePhase::TeamBuildVoting, status.phase());

  for (player_index_t i = 0; i < status.player_count; ++i) {
    status.vote(i, Ballot::Approve);
  }
  status.vote(4, Ballot::Disapprove);
  status.end_vote();
  EXPECT_EQ(GamePhase::QuestDetermining, status.phase());

  status.decide(1, QuestCard::Success);
  status.decide(3, QuestCard::Success);
  status.end_quest();
  EXPECT_EQ(GamePhase::RoundEnded, status.phase());

  status.start_next_round();
  EXPECT_EQ(GamePhase::TeamBuilding, status.phase());
  EXPECT_EQ(1, status.round());
  EXPECT_EQ(0, status.voted());
  EXPECT_EQ(1, status.succeed_quest_count());
}
