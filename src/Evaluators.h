#ifndef EVALUATORS_H_
#define EVALUATORS_H_

#include "DomineeringState.h"

#include <functional>
#include <utility>

using DS = DomineeringState;

struct Evaluator {
    using score_t = int;

    /**
     * Checks if the given grid is empty or not.
     * Note that this method does boundary checks first, which returns false
     * if any of the given parameter is invalid.
     */
    inline bool grid_empty(const int r, const int c, const DS& state) const {
        if (r < 0 || r >= state.ROWS || c < 0 || c >= state.COLS) {
            return false;
        }

        return state.getCell(r, c) == state.EMPTYSYM;
    }

    /**
     * Checks if a tile is placable on the given location.
     * A tile (i.e. domino) is placable if both grids are empty. Even though
     * this method does boundary checking, it does not check if the two grids
     * are adjacent to each other.
     */
    bool placable(const int r1, const int c1, const int r2, const int c2,
                  const DS& state) const {
        return grid_empty(r1, c1, state) && grid_empty(r2, c2, state);
    }

    /**
     * Checks if HOME has a reserved spot.
     * Home places dominoes horizontally. For example,
     *
     * +---+---+---+---+---+
     * | H | H | 1 | 2 |
     * +---+---+---+---+---+
     * |   |   | A | A |
     *
     * When 1 and 2 are (r1, c1) and (r2, c2) respectively, this method
     * returns true since the spot is "reserved" for HOME.
     */
    bool reserved_for_home(const int r1,
                           const int c1,
                           const int r2,
                           const int c2,
                           const DS& state) const {
        // Check if 1, 2 (in the docs) are actually empty
        if (!placable(r1, c1, r2, c2, state)) {
            return false;
        }

        bool no_space_above = (!grid_empty(r1 + 1, c1, state) &&
                               !grid_empty(r2 + 1, c2, state));

        bool no_space_below = (!grid_empty(r1 - 1, c1, state) &&
                               !grid_empty(r2 - 1, c2, state));

        return no_space_above && no_space_below;
    }

    /**
     * Checks if the given grid is reserved for AWAY.
     * Away places vertical dominoes.
     */
    bool reserved_for_away(const int r1,
                           const int c1,
                           const int r2,
                           const int c2,
                           const DS& state) const {
        if (!placable(r1, c1, r2, c2, state)) {
            return false;
        }

        bool no_space_to_left = (!grid_empty(r1, c1 - 1, state) &&
                                 !grid_empty(r2, c2 - 1, state));

        bool no_space_to_right = (!grid_empty(r1, c1 + 1, state) &&
                                  !grid_empty(r2, c2 + 1, state));

        return no_space_to_left && no_space_to_right;
    }
};

struct EvalReserve : public Evaluator {
    score_t operator()(const DS& state) const {
        score_t home_count = 0;
        score_t away_count = 0;

        for (int r = 0; r < state.ROWS; r++) {
            for (int c = 0; c < state.COLS; c++) {
                // Check if HOME has reserved spot here
                // Note: the method does boundary check
                if (reserved_for_home(r, c, r, c + 1, state)) {
                    home_count++;
                    // Go to the next column to skip over (r, c + 1)
                    c++;
                }
            }
        }
        for (int c = 0; c < state.COLS; c++) {
            for (int r = 0; r < state.ROWS; r++) {
                // Check if AWAY has reserved spot here
                // Note: the method does boundary check
                if (reserved_for_away(r, c, r + 1, c, state)) {
                    away_count++;
                    // Go to the next row to skip over (r + 1, c)
                    r++;
                }
            }
        }

        return home_count - away_count;
    }
};

using EvalScore = std::function<Evaluator::score_t(const DS&)>;
using EvalFactor = std::function<Evaluator::score_t(const DS&)>;
// vector of pairs because std::map requires operator< and std::unordered_map
// requires operator== and hash function for DomineeringState
static const std::vector<std::pair<EvalScore, EvalFactor>> evaluators = {
    {
        std::make_pair(EvalReserve(), [](const DS& state) {
                       return 1;
                       })
    },
};

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
