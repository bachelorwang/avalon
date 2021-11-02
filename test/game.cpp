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

TEST(game, team_build_failed_5_times) {
  GameStatus<5> status;
  status.setup(1);
  Team<5> team;
  team.members[0] = 0;
  team.members[1] = 1;
  team.members[2] = 2;
  for (size_t i = 0; i < 5; ++i) {
    status.start_next_round();
    team.count = status.team_build_requirement[status.round()];
    status.assign_team(team);
    status.end_vote();
    EXPECT_EQ(i + 1, status.voted());
  }
  EXPECT_EQ(GamePhase::GameEnded, status.phase());
  ASSERT_EQ(GameResult::EvilWinned, status.result());
}

TEST(game, team_build_failed_4_times_then_reset) {
  GameStatus<5> status;
  status.setup(1);
  Team<5> team;
  team.members[0] = 0;
  team.members[1] = 1;
  team.members[2] = 2;
  for (size_t i = 0; i < 4; ++i) {
    status.start_next_round();
    team.count = status.team_build_requirement[status.round()];
    status.assign_team(team);
    status.end_vote();
    EXPECT_EQ(i + 1, status.voted());
  }
  status.start_next_round();
  team.count = status.team_build_requirement[status.round()];
  status.assign_team(team);
  status.vote(0, Ballot::Approve);
  status.vote(1, Ballot::Approve);
  status.vote(2, Ballot::Approve);
  status.end_vote();
  EXPECT_EQ(0, status.voted());
}

TEST(game, full_progress_assassinated) {
  auto status = create_game_auto<5>(5, 2);
  ASSERT_EQ(GamePhase::Assassinating, status.phase());
  status.assassinate(2);
  ASSERT_EQ(GamePhase::GameEnded, status.phase());
  ASSERT_EQ(GameResult::EvilWinned, status.result());
}

TEST(game, full_progress_assassination_failed) {
  auto status = create_game_auto<5>(5, 1);
  ASSERT_EQ(GamePhase::Assassinating, status.phase());
  status.assassinate(2);
  ASSERT_EQ(GamePhase::GameEnded, status.phase());
  ASSERT_EQ(GameResult::GoodWinned, status.result());
}
