#pragma once
#include <set>
#include "avalon/def.hpp"

namespace avalon {

template <player_count_t>
struct TeamBuildRequirement;

template <player_count_t TCount>
struct Team {
  using Requirement = TeamBuildRequirement<TCount>;
  static constexpr player_count_t max_team_size = Requirement::max_size;
  player_count_t count;
  player_index_t members[max_team_size];

  bool valid(int round) const {
    auto req = Requirement::value[round];
    if (count != req)
      return false;

    std::set<player_index_t> set(members, members + count);
    if (set.size() != req) {
      return false;
    }
    for (auto m : set) {
      if (m < 0 || m >= TCount)
        return false;
    }
    return true;
  }
};

enum class QuestCard { Success, Fail };

template <player_count_t TCount>
struct Quest {
  using Requirement = TeamBuildRequirement<TCount>;
  static constexpr player_count_t max_team_size = Requirement::max_size;
  player_count_t count;
  QuestCard cards[max_team_size];

  void reset(int round) {
    count = Requirement::value[round];
    for (player_index_t i = 0; i < count; ++i) {
      cards[i] = QuestCard::Success;
    }
  }

  bool valid(int round) const {
    auto req = Requirement::value[round];
    if (count != req)
      return false;
    return true;
  }
};

enum class Ballot { Undecided, Disapprove, Approve };

template <player_count_t TCount>
struct BallotBox {
  void reset() {
    voted_ = 0;
    for (player_index_t i = 0; i < TCount; ++i)
      ballots_[i] = Ballot::Undecided;
  }

  void vote(player_index_t i, Ballot b) {
    AVALON_CHECK(i >= 0 && i < TCount, return );
    voted_ = 0;
    ballots_[i] = b;
    for (player_index_t i = 0; i < TCount; ++i) {
      if (Ballot::Undecided != ballots_[i])
        ++voted_;
    }
  }

  inline Ballot ballot(player_index_t i) const { return ballots_[i]; }

  inline player_count_t voted() const { return voted_; }

 private:
  Ballot ballots_[TCount];
  player_count_t voted_;
};

template <>
struct TeamBuildRequirement<5> {
  static constexpr player_count_t max_size = 3;
  static constexpr player_count_t value[round_count] = {2, 3, 2, 3, 3};
};

template <>
struct TeamBuildRequirement<6> {
  static constexpr player_count_t max_size = 4;
  static constexpr player_count_t value[round_count] = {2, 3, 4, 3, 4};
};

template <>
struct TeamBuildRequirement<7> {
  static constexpr player_count_t max_size = 4;
  static constexpr player_count_t value[round_count] = {2, 3, 3, 4, 4};
};

template <>
struct TeamBuildRequirement<8> {
  static constexpr player_count_t max_size = 5;
  static constexpr player_count_t value[round_count] = {3, 4, 4, 5, 5};
};

template <>
struct TeamBuildRequirement<9> {
  static constexpr player_count_t max_size = 5;
  static constexpr player_count_t value[round_count] = {3, 4, 4, 5, 5};
};

template <>
struct TeamBuildRequirement<10> {
  static constexpr player_count_t max_size = 5;
  static constexpr player_count_t value[round_count] = {3, 4, 4, 5, 5};
};

}  // namespace avalon
