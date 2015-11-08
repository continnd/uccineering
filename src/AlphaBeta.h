#ifndef ALPHA_BETA_H_
#define ALPHA_BETA_H_

#include "Evaluators.h"
#include "GameState.h"
#include "TranspositionTable.h"

#include <limits>
#include <algorithm>

/**
 * Simple class that gets passed around when doing the alpha-beta search.
 */
struct AlphaBeta {
    using score_t = Evaluator::score_t;
    static constexpr score_t POS_INF = std::numeric_limits<score_t>::max();
    static constexpr score_t NEG_INF = std::numeric_limits<score_t>::min();

    AlphaBeta()
        : alpha{NEG_INF}
        , beta{POS_INF}
    { }

    AlphaBeta(const score_t alpha, const score_t beta)
        : alpha{alpha}
        , beta{beta}
    { }

    AlphaBeta(const AlphaBeta& other)
        : alpha{other.alpha}
        , beta{other.beta}
    { }

    AlphaBeta& operator=(const AlphaBeta& other);

    AlphaBeta& operator=(AlphaBeta&& other);

    /**
     * Updates the alpha or beta according to the team.
     * If the team is HOME then alpha is updated, otherwise, AWAY is updated.
     * No change is made to the alpha/beta values if the current values are
     * better than the one given.
     *
     * \param[in] score the score that `team' got.
     *
     * \param[in] team
     */
    void update_if_needed(const score_t score, const Who team);

    /**
     * Checks if further children nodes can be pruned or not.
     *
     * \param[in] score the score that the node got.
     *
     * \param[in] team the team of the node to be examined. If pruning is valid,
     *                 all of its children are not searched.
     *
     * \return true if children nodes can be pruned, false otherwise.
     */
    bool can_prune(const score_t score, const Who team) const;

    /**
     * Checks if children can be pruned or not by looking at the range given
     * in the transposition table.
     */
    bool can_prune(const TranspositionTable::Entry& entry,
                   const Who team) const;

    /**
     * alpha = best max/home score
     * beta = best min/away score
     */
    score_t alpha, beta;
};

inline AlphaBeta& AlphaBeta::operator=(const AlphaBeta& other) {
    alpha = other.alpha;
    beta = other.beta;
    return *this;
}

inline AlphaBeta& AlphaBeta::operator=(AlphaBeta&& other) {
    alpha = std::move(other.alpha);
    beta = std::move(other.beta);
    return *this;
}

inline void AlphaBeta::update_if_needed(const score_t score, const Who team) {
    if (team == Who::HOME) {
        alpha = std::max(score, alpha);
    }
    else {
        beta = std::min(score, beta);
    }
}

inline bool AlphaBeta::can_prune(const score_t score, const Who team) const {
    return team == Who::HOME
        ? score >= beta || score == POS_INF
        : score <= alpha || score == NEG_INF;
}

inline bool AlphaBeta::can_prune(const TranspositionTable::Entry& entry,
                                 const Who team) const {
    return team == Who::HOME
        ? beta <= entry.lower_limit
        : alpha >= entry.upper_limit;
}

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
