#pragma once
#include "avalon/def.hpp"
#include "avalon/game/team_build.hpp"

namespace avalon {

enum class GameResult { StillInProgress, GoodWinned, EvilWinned };

enum class GamePhase {
  TeamBuilding,
  TeamBuildVoting,
  QuestDetermining,
  RoundEnded,
  Assassinating,
  GameEnded
};

template <player_count_t TCount>
class GameStatus {
 public:
  static constexpr player_count_t player_count = TCount;
  static constexpr auto team_build_requirement =
      TeamBuildRequirement<TCount>::value;

 public:
  GameStatus() { reset(); }
  ~GameStatus() = default;

  void reset() {
    ballot_box_.reset();
    result_ = GameResult::StillInProgress;
    phase_ = GamePhase::RoundEnded;
    quest_ = -1;
    learder_ = -1;
    succeed_quest_count_ = 0;
    voted_ = 0;
    merlin_index_ = -1;
  }

  inline GameResult result() const { return result_; }
  inline GamePhase phase() const { return phase_; }
  inline player_index_t leader() const { return learder_; }
  inline player_index_t merlin() const { return merlin_index_; }
  inline quest_index_t quest() const { return quest_; }
  inline quest_index_t round() const { return round_; }
  inline quest_count_t voted() const { return voted_; }
  inline quest_count_t succeed_quest_count() const {
    return succeed_quest_count_;
  }

  void setup(player_index_t merlin) {
    AVALON_CHECK(-1 == quest_, return;);
    merlin_index_ = merlin;
  }

  bool start_next_round() {
    AVALON_CHECK(-1 != merlin_index_, return false;);
    AVALON_CHECK(GamePhase::RoundEnded == phase_, return false;);

    ballot_box_.reset();
    assign_next_leader();
    phase_ = GamePhase::TeamBuilding;
    ++round_;
    if (voted_ == 0)
      ++quest_;
    return true;
  }

  bool assign_team(Team<TCount> team) {
    AVALON_CHECK(GamePhase::TeamBuilding == phase_, return false;);

    if (!team.valid(quest_))
      return false;
    team_ = team;
    quests_.reset(quest_);
    phase_ = GamePhase::TeamBuildVoting;
    return true;
  }

  void end_vote() {
    AVALON_CHECK(GamePhase::TeamBuildVoting == phase_, return;);

    player_count_t approved = 0;
    for (player_index_t i = 0; i < TCount; ++i) {
      auto b = ballot_box_.ballot(i);
      if (Ballot::Approve == b) {
        ++approved;
      }
    }
    if (team_build_passed<TCount>(approved)) {
      phase_ = GamePhase::QuestDetermining;
      voted_ = 0;
    } else {
      phase_ = GamePhase::RoundEnded;
      ++voted_;
    }

    if (max_voting_count == voted_) {
      result_ = GameResult::EvilWinned;
      phase_ = GamePhase::GameEnded;
    }
  }

  void end_quest() {
    AVALON_CHECK(GamePhase::QuestDetermining == phase_, return;);

    player_count_t fail = 0;
    for (player_index_t i = 0; i < quests_.count; ++i) {
      if (QuestCard::Fail == quests_.cards[i])
        ++fail;
    }

    if (fail < quest_failure_threshold<TCount>(quest_)) {
      ++succeed_quest_count_;
    }

    const auto failed_quest_count = quest_ - succeed_quest_count_;
    if (succeed_quest_count_ == min_quest_count) {
      phase_ = GamePhase::Assassinating;
    } else if (failed_quest_count == min_quest_count) {
      phase_ = GamePhase::GameEnded;
      result_ = GameResult::EvilWinned;
    } else {
      phase_ = GamePhase::RoundEnded;
    }
  }

  void assassinate(player_index_t i) {
    AVALON_CHECK(GamePhase::Assassinating == phase_, return;);
    AVALON_CHECK(i >= 0, return;);

    if (i == merlin_index_)
      result_ = GameResult::EvilWinned;
    else
      result_ = GameResult::GoodWinned;
    phase_ = GamePhase::GameEnded;
  }

  bool vote(player_index_t p, Ballot b) {
    if (p >= 0 && p < TCount) {
      ballot_box_.vote(p, b);
      return true;
    }
    return false;
  }

  bool decide(player_index_t p, QuestCard q) {
    for (player_index_t i = 0; i < team_.count; ++i) {
      if (team_.members[i] == p) {
        quests_.cards[i] = q;
        return true;
      }
    }
    return false;
  }

 private:
  void assign_next_leader() {
    ++learder_;
    if (learder_ >= TCount)
      learder_ = 0;
  }

 private:
  GameResult result_;
  GamePhase phase_;
  BallotBox<TCount> ballot_box_;
  quest_index_t quest_;
  round_index_t round_;
  player_index_t learder_;
  player_index_t merlin_index_;
  quest_count_t succeed_quest_count_;
  quest_count_t voted_;

  Team<TCount> team_;
  Quest<TCount> quests_;
};

}  // namespace avalon
