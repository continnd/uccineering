#ifndef EVALUATORS_H_
#define EVALUATORS_H_

#include "DomineeringState.h"

#include <functional>
#include <utility>

using DS = DomineeringState;

struct Evaluator {
    /**
     * Checks if there is space to put a `self' domino when r1 and c1 are the
     * given.
     * Depending on `self', bottom tile or the right tile is checked.
     */
    bool has_space_for(const Who self, const int r1, const int c1,
                       const DS& state) const {
        auto empty = state.EMPTYSYM;
        if (state.getCell(r1, c1) != empty) {
            return false;
        }

        auto r2 = self == Who::HOME ? r1 : r1 + 1;
        auto c2 = self == Who::HOME ? c1 + 1 : c1;

        if ((r1 < 0 || r1 >= state.ROWS || c1 < 0 || c1 >= state.ROWS)
            || (r2 < 0 || r2 >= state.ROWS || c2 < 0 || c2 >= state.ROWS)) {
            return false;
        }

        return state.getCell(r2, c2) == empty;
    }

    bool home_edge(int r1, int c1, int c2, int r2, const DS& state) const {
        return (r1 > 0 && c1 > 0
                && r2 == state.ROWS
                || c1 == state.COLS
                && state.getCell(r1, c2) != state.EMPTYSYM
                && state.getCell(r1, c1) != state.EMPTYSYM)

            || ((r1 < 0 || c1 < 0)
                && r2 < state.ROWS
                && c1 < state.COLS
                && state.getCell(r2, c2) != state.EMPTYSYM
                && state.getCell(r2, c1) != state.EMPTYSYM);
    }

    bool home_mid(int r1, int c1, int r2, int c2, const DS& state) const {
        return (r1 > 0 && c1 > 0
                && state.getCell(r1, c2) != state.EMPTYSYM
                && state.getCell(r1, c1) != state.EMPTYSYM)

            && (r2 < state.ROWS && c1 < state.COLS
                && state.getCell(r2, c2) != state.EMPTYSYM
                && state.getCell(r2, c1) != state.EMPTYSYM);
    }

    bool away_edge(int r1, int c1, int r2, int c2, const DS& state) const {
        return (c1 > 0 && c2 == state.COLS
                && state.getCell(r2, c1) != state.EMPTYSYM
                && state.getCell(r1, c1) != state.EMPTYSYM)

            || (c1 < 0 && c2 < state.COLS
                && state.getCell(r2, c2) != state.EMPTYSYM
                && state.getCell(r1, c2) != state.EMPTYSYM);
    }

    bool away_mid(int r1, int c1, int r2, int c2, const DS& state) const {
        return (r1 > 0 && c1 > 0
                && r1 < state.ROWS
                && c2 < state.COLS
                && state.getCell(r2, c1) != state.EMPTYSYM
                && state.getCell(r1, c1) != state.EMPTYSYM
                && state.getCell(r2, c2) != state.EMPTYSYM
                && state.getCell(r1, c2) != state.EMPTYSYM);
    }
};

struct EvalTakeAway : public Evaluator {
    double operator()(const DS& state) const {
        int count = 0;
        auto self = state.getWho();

        for (int i = 0; i < state.ROWS; i++) {
            for (int j = 0; j < state.COLS; j++) {
                if (!has_space_for(self, i, j, state)) {
                    continue;
                }

                if (self == Who::HOME) {
                    if (home_edge(i-1, j+1, i+1, j, state)) {
                        count++;
                    }
                    else if (home_mid(i-1, j+1, i+1, j, state)) {
                        count++;
                    }
                }
                else {
                    if (away_edge(i + 1, j - 1, i, j + 1, state)) {
                        count++;
                    }
                    else if (away_mid(i + 1, j - 1, i, j + 1, state)) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
};

struct EvalReserve : public Evaluator {
    double operator()(const DS& state) const {
        int count = 0;
        auto self = state.getWho();

        for (int i = 0; i < state.ROWS; i++) {
            for (int j = 0; j < state.COLS; j++) {
                if (!has_space_for(self, i, j, state)) {
                    continue;
                }

                if (state.getCurPlayerSym() == state.AWAYSYM) {
                    if (home_edge(i - 1, j + 1, i + 1, j, state)) {
                        count++;
                    }
                    else if (home_mid(i - 1, j + 1, i + 1, j, state)) {
                        count++;
                    }
                }
                else if (state.getCurPlayerSym() == state.HOMESYM) {
                    if (away_edge(i + 1, j - 1, i, j + 1, state)) {
                        count++;
                    }
                    else if (away_mid(i + 1, j - 1, i, j + 1, state)) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
};

using EvalScore = std::function<double(const DS&)>;
using EvalFactor = std::function<double(const DS&)>;
// vector of pairs because std::map requires operator< and std::unordered_map
// requires operator== and hash function for DomineeringState
static const std::vector<std::pair<EvalScore, EvalFactor>> evaluators = {
    {
        std::make_pair(EvalTakeAway(), [](const DS& state) {
                       return 1;
                       })
    },
    {
        std::make_pair(EvalReserve(), [](const DS& state) {
                       return 1;
                       })
    },
};

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */

