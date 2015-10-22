#ifndef ALPHA_BETA_H_
#define ALPHA_BETA_H_

#include "GameState.h"

#include <limits>
#include <algorithm>

/**
 * Simple class that gets passed around when doing the alpha-beta search.
 */
struct AlphaBeta {
    static constexpr double POS_INF = std::numeric_limits<double>::max();
    static constexpr double NEG_INF = std::numeric_limits<double>::min();

    AlphaBeta()
        : alpha{NEG_INF}
        , beta{POS_INF}
    { }

    AlphaBeta(const double alpha, const double beta)
        : alpha{alpha}
        , beta{beta}
    { }

    AlphaBeta(const AlphaBeta& other)
        : alpha{other.alpha}
        , beta{other.beta}
    { }

    AlphaBeta& operator=(const AlphaBeta& other);

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
    void update_if_needed(const double score, const Who team);

    /**
     * alpha = best max/home score
     * beta = best min/away score
     */
    double alpha, beta;
};

inline AlphaBeta& AlphaBeta::operator=(const AlphaBeta& other) {
    alpha = other.alpha;
    beta = other.beta;
    return *this;
}

inline void AlphaBeta::update_if_needed(const double score, const Who team) {
    if (team == Who::HOME) {
        alpha = std::max(score, alpha);
    }
    else {
        beta = std::min(score, beta);
    }
}

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
